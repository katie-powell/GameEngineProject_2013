// Defines a pool-based memory allocator, as well as helper structs
#ifndef _POOLALLOC_H_
#define _POOLALLOC_H_
#include "singleton.h"
#include "dbg_assert.h"
#include <memory.h>

namespace ITP485
{

// PoolBlock is a structure that we use as the building block for the pool-based allocator.
// It's templated by size of the block to support different size blocks in separate pools.
// IMPORTANT! block_size must be divisible by 16 to ensure _memory returns a ptr aligned by 16 bytes.
template <size_t block_size>
struct PoolBlock
{
	// This is the actual memory that the caller will be writing to.
	char _memory[block_size];
	
	// This boundary value is used to help find instances where memory is being written
	// beyond the _memory array.
	// 
	// While this is only used in debug, it's always here to ensure sizeof(PoolBlock)
	// is divisible by 16.
	unsigned int _boundary;
	
	// Pointer to the next block in the free list.
	PoolBlock<block_size>* _next;
private:
#if _WIN32
	// Padding to ensure sizeof(PoolBlock) % 16 == 0
	char _padding[8];
#else // running 64 bit, padding only needs to be 4
	char _padding[4];
#endif

public:
	PoolBlock()
	{
		_next = 0;
		Dbg_Assert(block_size % 16 == 0, "Block size isn't divisible by 16.");
	}

	// Disallow single new/delete of a PoolBlock
	void* operator new(size_t size)
	{
		Dbg_Assert(0, "Can't allocate single PoolBlock by itself.");
	}
	void operator delete(void* ptr)
	{
		Dbg_Assert(0, "Can't delete single PoolBlock by itself.");
	}

	// Overloads of array new/delete to ensure the array is 16-byte aligned
	void* operator new[] (size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	void operator delete[] (void* ptr)
	{
		_aligned_free(ptr);
	}
};

// Defines the Pool Allocator
// Templated based on size of block and the number of blocks in the pool.
//
// To define your own pool to be used, it's recommended to typedef as such:
// typedef PoolAllocator<256, 1024> ComponentPool;
// Remember block_size must be divisible by 16.
// IMPORTANT! Because of the way templates work, and this is a Singleton,
// if you have two different typedefs that have the same block_size and
// same num_blocks, both will end up using the SAME pool.
//
// IMPORTANT! StartUp must always be called before starting to use this,
// and ShutDown must be called once you're done with it. Or bad things happen.
template <size_t block_size, unsigned int num_blocks>
class PoolAllocator : public Singleton<PoolAllocator<block_size, num_blocks> >
{
	DECLARE_SINGLETON(PoolAllocator);
public:
	// StartUp dynamically allocates the pool on the heap.
	// 
	// mpPool should be allocated to an array with num_blocks elements
	// It also will correctly initialize the free list and all its _next pointers.
	// By default, you want index 0 of a pool to point to index 1, and so on.
	// Make sure you update iBlocksFree.
	//
	// #ifdef _DEBUG, you should also write to all the _memory arrays the value 0xde over and over,
	// and _boundary should be set to 0xdeadbeef.
	void StartUp();

	// ShutDown deallocates the pool.
	void ShutDown();

	// Allocate returns a pointer to usable memory within the pool.
	// 
	// It will Dbg_Assert size <= block_size.
	// If the size is okay, it will remove a PoolBlock from the free list,
	// and return the pointer to that PoolBlock's _memory member
	// Make sure you update iBlocksFree.
	//
	// If there are no blocks available, it should Dbg_Assert and return 0.
	void* Allocate(size_t size);

	// Free will determine which PoolBlock the ptr belongs to, and then return
	// the block to the front of the free list.
	// Make sure you update iBlocksFree.
	//
	// #ifdef _DEBUG, Dbg_Assert that boundary still == 0xdeadbeef (if not, the bounds were overwritten)
	// Also, write the value 0xde over and over into the _memory array.
	//
	// Note that there is no reasonable way to verify that the pointer actually belongs in the
	// pool, so don't call this on random pointers!
	void Free(void* ptr);

	// Returns the number of blocks free in the pool
	unsigned int GetNumBlocksFree() { return iBlocksFree; }

protected:
	// Default constructor does nothing other than set some pointers to 0
 	PoolAllocator()
 		: m_pPool(0)
 		, m_pFreeList(0)
 	{ }
	
	// This pointer will point to the array of all blocks
	PoolBlock<block_size>* m_pPool;
	
	// This pointer represents the free list. Initially points to element at index 0
	PoolBlock<block_size>* m_pFreeList;

	// This keeps track of how many blocks are left in the pool
	unsigned int iBlocksFree;
};

// IMPLEMENTATIONS for PoolAllocator

// StartUp dynamically allocates the pool on the heap.
// 
// mpPool should be allocated to an array with num_blocks elements
// It also will correctly initialize the free list and all its _next pointers.
// By default, you want index 0 of a pool to point to index 1, and so on.
// Make sure you update iBlocksFree.
//
// #ifdef _DEBUG, you should also write to all the _memory arrays the value 0xde over and over,
// and _boundary should be set to 0xdeadbeef.
template <size_t block_size, unsigned int num_blocks>
void PoolAllocator<block_size, num_blocks>::StartUp()
{
	//create mpPool : array of num_blocks blocks of size block_size
	m_pPool = new PoolBlock<block_size>[num_blocks];
	m_pFreeList = m_pPool;		//now m_pFreeList is same as m_pPool

	//loop through each pool block, setting *_next to next block
	for (int i = 0; i<num_blocks-1; i++)
	{
		m_pPool[i]._next = &m_pPool[i+1];
	}
	
	iBlocksFree = num_blocks;

#ifdef _DEBUG
	for (int i = 0; i<num_blocks; i++)
	{
		//m_pPool[i]._memory = reinterpret_cast<char[16]>(0xde); 
		memset(&m_pPool[i], 0xde, 16);
		m_pPool[i]._boundary = 0xdeadbeef;
	}
#endif


}

// ShutDown deallocates the pool.
template <size_t block_size, unsigned int num_blocks>
void PoolAllocator<block_size, num_blocks>::ShutDown()
{
	delete[] m_pPool;
	m_pFreeList = nullptr;
	iBlocksFree = 0;

}

// Allocate returns a pointer to usable memory within the pool.
// 
// It will Dbg_Assert size <= block_size.
// If the size is okay, it will remove a PoolBlock from the free list,
// and return the pointer to that PoolBlock's _memory member
// Make sure you update iBlocksFree.
//
// If there are no blocks available, it should Dbg_Assert and return 0.
template <size_t block_size, unsigned int num_blocks>
void* PoolAllocator<block_size, num_blocks>::Allocate(size_t size)
{
	if(iBlocksFree > 0 && size > 0)
	{
		Dbg_Assert(size <= block_size, "Requested size larger than block size.");
		{
			void* pointer = m_pFreeList->_memory;
			m_pFreeList = m_pFreeList->_next;
			iBlocksFree--;
			return pointer;
		}
	}
	Dbg_Assert(iBlocksFree > 0, "There are no blocks available.");
	return 0;
}

// Free will determine which PoolBlock the ptr belongs to, and then return
// the block to the front of the free list.
// Make sure you update iBlocksFree.
//
// #ifdef _DEBUG, Dbg_Assert that boundary still =- 0xdeadbeef (if not, the bounds were overwritten)
// Also, write the value 0xde over and over into the _memory array.
//
// Note that there is no reasonable way to verify that the pointer actually belongs in the
// pool, so don't call this on random pointers!
template <size_t block_size, unsigned int num_blocks>
void PoolAllocator<block_size, num_blocks>::Free(void* ptr)
{
	for(int i=0; i<num_blocks; i++)
	{
		if(ptr == m_pPool[i]._memory)
		{
			m_pPool[i]._next = m_pFreeList;
			m_pFreeList = &m_pPool[i];
			iBlocksFree++;
#ifdef _RELEASE
			return;
#endif
		}
	}

#ifdef _DEBUG
	for (int i = 0; i<num_blocks; i++)
	{
		//m_pPool[i]._memory = reinterpret_cast<char[16]>(0xde);
		Dbg_Assert(m_pPool[i]._boundary == 0xdeadbeef, "The bounds have been overwritten.");
		memset(&m_pPool[i], 0xde, 16);
	}
#endif
}

} // namespace ITP485

// Don't mess with this!
// Macro that defines operator new/deletes which use a specific pool.
// Use this inside the definition of a class, when you want said class to have its
// new/deletes place it into a pool.
// This must be defined within the public section of the class or it won't work.
#define DECLARE_POOL_NEW_DELETE(PoolTemplate)		\
	static void* operator new(size_t size)			\
	{												\
		return PoolTemplate::get().Allocate(size);	\
	}												\
	static void operator delete(void* ptr)			\
	{												\
		PoolTemplate::get().Free(ptr);				\
	}

#endif // _POOLALLOC_H_
