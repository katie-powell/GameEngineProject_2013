// Defines the MeshData class which stores all the data relevant to
// a particular .x file.
#ifndef _MESHDATA_H_
#define _MESHDATA_H_
#include <d3dx9.h>

namespace ITP485
{

struct MeshData
{
public:
	// Load in all the mesh data from the specified .x file.
	// Make sure you include the full path of the file
	MeshData(const char* szFileName);

	// Releases all the mesh data
	~MeshData();

	void Draw(ID3DXEffect* pEffect, D3DXHANDLE hTechnique);
private:
	MeshData() {} // Disallow default constructor
	
	// Mesh data
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;

	int m_iVertexSize;
	int m_iNumVerts;

	int m_iNumTris;
};

} // namespace

#endif // _MESHDATA_H_
