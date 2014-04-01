// MeshManager handles creating mesh data as needed
#ifndef _MESHMANAGER_H_
#define _MESHMANAGER_H_
#include "../core/singleton.h"
#include <map>

namespace ITP485
{

struct MeshData;

class MeshManager : public Singleton<MeshManager>
{
	DECLARE_SINGLETON(MeshManager);
public:
	// Does nothing of note for now
	void Setup();
	
	// Iterates through the Mesh Map, and deletes all MeshData pointers.
	// Then clears out Mesh Map.
	void Cleanup();

	// Searches the std::map for the requested mesh. If it exists, that
	// MeshData is returned.
	// If the MeshData isn't already loaded for it, will construct a MeshData
	// using new, add that pointer to the hash map, and then return that pointer
	MeshData* GetMeshData(const char* szMeshFile);
private:
	// Helper function which hashes the passed string using djb2 algorithm
	unsigned int HashString(const char* str);

	std::map<unsigned int, MeshData*> m_MeshMap;
};

} // namespace
#endif // _MESHMANAGER_H_
