// The mesh component is used by any game objects which have a renderable mesh.
#ifndef _MESHCOMPONENT_H_
#define _MESHCOMPONENT_H_
#include "../core/poolalloc.h"
#include "../core/math.h"
#include <d3dx9effect.h>

namespace ITP485
{

typedef PoolAllocator<128, 1024> MeshComponentPool;
struct MeshData;

class MeshComponent
{
public:
	// This ensures that we use the mesh component pool on new/delete
	DECLARE_POOL_NEW_DELETE(MeshComponentPool);

	// Constructor takes the filename of the mesh.
	// It will then request MeshData from the MeshManager, and save off that pointer.
	// It also should set the WorldTransform matrix to Matrix4::Identity.
	// Sets m_bIsVisible to true.
	// Once the MeshComponent is ready, it should add itself to the GraphicsDevice's
	// MeshComponentSet.
	MeshComponent(const char* szFileName);

	// Makes the appropriate Direct3D calls to Draw this MeshComponent
	// if m_bIsVisible is true.
	void Draw();

	// Removes this MeshComponent from GraphicsDevice's MeshComponentSet
	~MeshComponent();

	// Returns WorldTransform by reference, so you can modify it
	Matrix4& GetWorldTransform() { return m_WorldTransform; }

	bool GetVisible() const { return m_bIsVisible; }
	void SetVisible(bool bValue) { m_bIsVisible = bValue; }
	
	Quaternion GetRotation() {return m_rotation;} const
	void SetRotation(const Quaternion *qValue) { m_rotation = *qValue;}
	Vector3 GetTranslation() {return m_translation;} const
	void SetTranslation(const Vector3 *vValue) { m_translation = *vValue;}
	float GetScale() {return m_scale;} const
	void SetScale(const float fValue) { m_scale = fValue;}
	LPD3DXEFFECT GetEffect() {return m_effect;} const
	void SetEffect(LPD3DXEFFECT feffect) { m_effect = feffect;}

private:
	// Disallow default constructor
	MeshComponent() { }
	// World Transform Matrix
	Matrix4 m_WorldTransform;
	Quaternion m_rotation;
	Vector3 m_translation;
	float m_scale;
	// Our particular model information
	MeshData* m_pMeshData;
	// Whether or not this guy is visible
	bool m_bIsVisible:1;
	LPD3DXEFFECT m_effect;
};

}
#endif // _MESHCOMPONENT_H_
