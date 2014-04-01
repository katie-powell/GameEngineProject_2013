#include "MeshData.h"
#include "../core/dbg_assert.h"
#include <cstdlib>
#include <wchar.h>
#include "GraphicsDevice.h"
#include "../../ticpp/ticpp.h"

namespace ITP485
{

// Position and texture coordinates
struct VERTEX_P_T
{
	float fX, fY, fZ;
	float fU, fV;
};

D3DVERTEXELEMENT9 decl_p_t[] =
{
	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};

// Position, normal, and texture coordinates
struct VERTEX_P_N_T
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 UV;
};

D3DVERTEXELEMENT9 decl_p_n_t[] =
{
	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};

// Position, normal, skinning weights, skinning indices, and texture coordinates
struct VERTEX_P_N_S_T
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR4 vWeights;
	D3DXVECTOR4 vIndices;
	D3DXVECTOR2 UV;
};

D3DVERTEXELEMENT9 decl_p_n_s_t[] =
{
	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}, // 12
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0}, // 12
	{0, 24, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0}, // 16
	{0, 40, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0}, // 16
	{0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0}, // 2
	D3DDECL_END()
};

MeshData::MeshData(const char* szFileName)
: m_pTexture(nullptr)
, m_pVertexBuffer(nullptr)
, m_pIndexBuffer(nullptr)
, m_pVertexDecl(nullptr)
, m_iVertexSize(0)
{
// 	VERTEX_P_T aCubeVertices[] = {
// 		{-1.0f,-1.0f,-1.0f,0.0f,1.0f},{-1.0f, 1.0f,-1.0f,0.0f,0.0f},{1.0f, 1.0f,-1.0f,1.0f,0.0f},
// 		{ 1.0f,-1.0f,-1.0f,1.0f,1.0f},{-1.0f,-1.0f, 1.0f,0.0f,0.0f},{1.0f,-1.0f, 1.0f,0.0f,1.0f},
// 		{ 1.0f, 1.0f, 1.0f,0.0f,0.0f},{-1.0f, 1.0f, 1.0f,0.0f,1.0f}};
// 
// 	short aCubeIndices[] = {0,1,2,2,3,0,4,5,6,6,7,4,0,3,5,5,4,0,3,2,6,6,5,3,2,1,7,7,6,2,1,0,4,4,7,1};
	
	HRESULT hr;
	LPDIRECT3DDEVICE9 pDevice = GraphicsDevice::get().GetD3DDevice();
	VOID* pData;

	// Parse the itpmesh file
	ticpp::Document doc(szFileName);
	doc.LoadFile();

	// This is some really ugly XML parsing code
	ticpp::Iterator<ticpp::Element> child;
	for(child = child.begin(doc.FirstChildElement()); child != child.end(); child++)
	{
		// The value of this child identifies the name of this element
		std::string strName;
		std::string strValue;
		std::string strText;

		child->GetValue(&strName);
		if (strName == "format")
		{
			// Select the correct vertex format
			strText = child->GetText();
			if (strText == "pt")
			{
				hr = pDevice->CreateVertexDeclaration(decl_p_t, &m_pVertexDecl);
				m_iVertexSize = sizeof(VERTEX_P_T);
			}
			else if (strText == "pnt")
			{
				hr = pDevice->CreateVertexDeclaration(decl_p_n_t, &m_pVertexDecl);
				m_iVertexSize = sizeof(VERTEX_P_N_T);
			}
			else if (strText == "pnst")
			{
				hr = pDevice->CreateVertexDeclaration(decl_p_n_s_t, &m_pVertexDecl);
				m_iVertexSize = sizeof(VERTEX_P_N_S_T);
			}
			
			Dbg_Assert(hr == D3D_OK, "Vertex declaration did not initialize!");
		}
		else if (strName == "texture")
		{
			// Load the texture
			strText = child->GetText();
			hr = D3DXCreateTextureFromFileA(pDevice, strText.c_str(), &m_pTexture);
			Dbg_Assert(hr == D3D_OK, "Could not load texture!");
		}
		else if (strName == "triangles")
		{
			// Initialize index buffer memory
			strValue = child->GetAttribute("count");
			m_iNumTris = atoi(strValue.c_str());
			short* pIndices = new short[m_iNumTris * 3];
			int i = 0;

			// Now get every triangle
 			ticpp::Iterator<ticpp::Element> tri;
			for(tri = tri.begin(child.Get()); tri != tri.end(); tri++)
 			{
				short v0, v1, v2;
 				sscanf_s(tri->GetText().c_str(), "%hd,%hd,%hd", &v0, &v1, &v2);
				
				pIndices[i] = v0;
				i++;
				pIndices[i] = v1;
				i++;
				pIndices[i] = v2;
				i++;
 			}

			// Now d3d calls to initialize the index buffer
			hr = pDevice->CreateIndexBuffer(sizeof(short) * m_iNumTris * 3, D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIndexBuffer,NULL);
			Dbg_Assert(hr == D3D_OK, "Could not create index buffer!");

			m_pIndexBuffer->Lock(0,sizeof(pData),(void**)&pData,0);
			memcpy(pData, pIndices, sizeof(short) * m_iNumTris * 3);
			m_pIndexBuffer->Unlock();

			// clear memory
			delete[] pIndices;
		}
		else if (strName == "vertices")
		{
			strValue = child->GetAttribute("count");
			m_iNumVerts = atoi(strValue.c_str());
			float* pVerts = new float[m_iNumVerts * m_iVertexSize];
			int i = 0;
			
			// Now get every vertex
			ticpp::Iterator<ticpp::Element> vert;
			for(vert = vert.begin(child.Get()); vert != vert.end(); vert++)
			{
				float x, y, z, w;

				// Get every element of this vertex
				ticpp::Iterator<ticpp::Element> ele;
				for(ele = ele.begin(vert.Get()); ele != ele.end(); ele++)
				{
					ele->GetValue(&strName);
					if (strName == "pos")
					{
						sscanf_s(ele->GetText().c_str(), "%f,%f,%f", &x, &y, &z);
						pVerts[i] = x;
						i++;

						pVerts[i] = y;
						i++;

						pVerts[i] = z;
						i++;
					}
					else if (strName == "norm")
					{
						sscanf_s(ele->GetText().c_str(), "%f,%f,%f", &x, &y, &z);
						pVerts[i] = x;
						i++;

						pVerts[i] = y;
						i++;

						pVerts[i] = z;
						i++;
					}
					else if (strName == "tex")
					{
						sscanf_s(ele->GetText().c_str(), "%f,%f", &x, &y);
						pVerts[i] = x;
						i++;

						pVerts[i] = y;
						i++;
					}
					else if (strName == "sw")
					{
						sscanf_s(ele->GetText().c_str(), "%f,%f,%f,%f", &x, &y, &z, &w);
						pVerts[i] = x;
						i++;

						pVerts[i] = y;
						i++;

						pVerts[i] = z;
						i++;

						pVerts[i] = w;
						i++;
					}
					else if (strName == "si")
					{
						sscanf_s(ele->GetText().c_str(), "%f,%f,%f,%f", &x, &y, &z, &w);
						pVerts[i] = x;
						i++;

						pVerts[i] = y;
						i++;

						pVerts[i] = z;
						i++;

						pVerts[i] = w;
						i++;
					}
				}
			}

			// Load up the vertex buffer
			hr = pDevice->CreateVertexBuffer(sizeof(float) * m_iNumVerts * m_iVertexSize,D3DUSAGE_WRITEONLY,
				0,D3DPOOL_MANAGED,&m_pVertexBuffer,NULL);
			Dbg_Assert(hr == D3D_OK, "Could not create vertex buffer!");

			m_pVertexBuffer->Lock(0,sizeof(pData),(void**)&pData,0);
			memcpy(pData, pVerts, sizeof(float) * m_iNumVerts * m_iVertexSize);
			m_pVertexBuffer->Unlock();
		}
	}
	
	// Create appropriate vertex declaration
// 	HRESULT hr = pDevice->CreateVertexDeclaration(decl_p_t, &m_pVertexDecl);
// 	Dbg_Assert(hr == D3D_OK, "Vertex declaration did not initialize!");
// 	
// 	hr = pDevice->CreateVertexBuffer(sizeof(aCubeVertices),D3DUSAGE_WRITEONLY,
// 		0,D3DPOOL_MANAGED,&m_pVertexBuffer,NULL);
// 	Dbg_Assert(hr == D3D_OK, "Could not create vertex buffer!");
// 	
// 	hr = pDevice->CreateIndexBuffer(sizeof(aCubeIndices),D3DUSAGE_WRITEONLY,
// 		D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIndexBuffer,NULL);
// 	Dbg_Assert(hr == D3D_OK, "Could not create index buffer!");
// 
// 	VOID* pData;
// 
// 	m_pVertexBuffer->Lock(0,sizeof(pData),(void**)&pData,0);
// 	memcpy(pData,aCubeVertices,sizeof(aCubeVertices));
// 	m_pVertexBuffer->Unlock();
// 
// 	m_pIndexBuffer->Lock(0,sizeof(pData),(void**)&pData,0);
// 	memcpy(pData,aCubeIndices,sizeof(aCubeIndices));
// 	m_pIndexBuffer->Unlock();
// 
// 	hr = D3DXCreateTextureFromFileA(pDevice,"zophusX.jpg",&m_pTexture);
// 	Dbg_Assert(hr == D3D_OK, "Could not load texture!");
}

// Cleanup all this crazy mesh data
MeshData::~MeshData()
{

}

void MeshData::Draw(ID3DXEffect* pEffect, D3DXHANDLE hTechnique)
{
	pEffect->SetTexture("DiffuseMapTexture", m_pTexture);

	if (SUCCEEDED(pEffect->SetTechnique(hTechnique)))
	{
		UINT iPasses;
		if (SUCCEEDED(pEffect->Begin(&iPasses, 0)))
		{
			for (UINT pass = 0; pass < iPasses; ++pass)
			{
				if (SUCCEEDED(pEffect->BeginPass(pass)))
				{
					// DRAW THE MESH!
					LPDIRECT3DDEVICE9 pDevice = GraphicsDevice::get().GetD3DDevice();
					pDevice->SetVertexDeclaration(m_pVertexDecl);
					pDevice->SetStreamSource(0,m_pVertexBuffer,0,m_iVertexSize);
					pDevice->SetIndices(m_pIndexBuffer);
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_iNumVerts,0,m_iNumTris);
					
					pEffect->EndPass();
				}
			}

			pEffect->End();
		}
	}
}

} // namespace
