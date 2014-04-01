// This defines the core rendering class
#ifndef _GRAPHICSDEVICE_H_
#define _GRAPHICSDEVICE_H_
#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>
#include <d3dx9effect.h>
#include "../core/singleton.h"
#include "../core/math.h"
#include <set>

namespace ITP485
{

class GraphicsDevice : public Singleton<GraphicsDevice>
{
	friend class MeshComponent;
	DECLARE_SINGLETON(GraphicsDevice);
public:
	// Sets up our D3D device to the passed window.
	// Make sure you assert that the D3D device initialized.
	void Setup(HWND hWnd);

	// Releases all D3D resources
	void Cleanup();

	// Renders the current frame
	void Render();

	void setCameraMatrix(const Matrix4 *mat){m_CameraMtx = *mat;}
	Matrix4 getCameraMatrix(){return m_CameraMtx;} const

	void setProjectionMatrix(const Matrix4 *mat){m_ProjectionMtx = *mat;}
	Matrix4 getProjectionMatrix(){return m_ProjectionMtx;} const

	// Returns the D3D device pointer
	LPDIRECT3DDEVICE9 GetD3DDevice() { return m_pDevice; }

	// Returns the debug effect
	LPD3DXEFFECT GetDebugEffect() { return m_pDebugEffect; }

	LPD3DXEFFECT LoadEffect(const char* szFileName);

protected:
	// Default constructor does nothing other than set some pointers to 0
	GraphicsDevice()
		: m_pD3D(nullptr)
		, m_pDevice(nullptr)
		, m_pDebugEffect(nullptr)
	{ }

	// Camera matrix
	Matrix4 m_CameraMtx;
	// Projection matrix
	Matrix4 m_ProjectionMtx;

	// Direct3D9 interface pointer
	LPDIRECT3D9 m_pD3D;
	// Direct3D9 device pointer
	LPDIRECT3DDEVICE9 m_pDevice;

	// Global effect we use
	LPD3DXEFFECT m_pDebugEffect;

	// Set of all active MeshComponents
	std::set<MeshComponent*> m_MeshComponentSet;

	// Force alignment so our matrices don't explode unhappily
	void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	void operator delete(void* ptr)
	{
		_aligned_free(ptr);
	}

	
};

}
#endif // _GRAPHICSDEVICE_H_
