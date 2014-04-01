// Implementation for our MeshManager
#include "MeshManager.h"
#include "MeshData.h"

namespace ITP485
{

// Does nothing of note for now
void MeshManager::Setup()
{

}

// Iterates through the Mesh Map, and deletes all MeshData pointers.
// Then clears out Mesh Map.
void MeshManager::Cleanup()
{
	//m_MeshMap[i].delete();
	m_MeshMap.clear();
}

// Searches the std::map for the requested mesh. If it exists, that
// MeshData is returned.
// If the MeshData isn't already loaded for it, will construct a MeshData
// using new, add that pointer to the hash map, and then return that pointer
MeshData* MeshManager::GetMeshData(const char* szMeshFile)
{
	// std::map<unsigned int, MeshData*> m_MeshMap;
	std::map<unsigned int, MeshData*>::iterator it;
	it = m_MeshMap.find(HashString(szMeshFile));
	if(it == m_MeshMap.end()) //if szMeshFile is not found
	{
		MeshData* newMesh = new MeshData(szMeshFile);
		m_MeshMap[HashString(szMeshFile)] = newMesh;
		return newMesh;
	}
	else
	{
		return m_MeshMap[HashString(szMeshFile)];
	}
}

// Helper function which hashes the passed string using djb2 algorithm
unsigned int MeshManager::HashString(const char* str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

}
