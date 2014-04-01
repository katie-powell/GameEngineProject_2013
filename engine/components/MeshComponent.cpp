#include "MeshComponent.h"
#include "../graphics/MeshManager.h"
#include "../graphics/GraphicsDevice.h"
#include "../graphics/MeshData.h"

namespace ITP485
{

// Constructor takes the filename of the mesh.
// It will then request MeshData from the MeshManager, and save off that pointer.
// It also should set the WorldTransform matrix to Matrix4::Identity.
// Sets m_bIsVisible to true.
// Once the MeshComponent is ready, it should add itself to the GraphicsDevice's
// MeshComponentSet.
MeshComponent::MeshComponent(const char* szFileName)
{
	m_pMeshData = MeshManager::get().GetMeshData(szFileName);
	m_WorldTransform = Matrix4::Identity;
	m_rotation = Quaternion::Identity;
	m_translation = Vector3::Zero;
	m_scale = 1.0f;
	//SetVisible(true);
	m_bIsVisible = true;
	GraphicsDevice::get().m_MeshComponentSet.insert(this);
}

// Makes the appropriate Direct3D calls to Draw this MeshComponent
// if m_bIsVisible is true.
void MeshComponent::Draw()
{
	if(m_bIsVisible)
	{
		//compute a new World Transform Matrix based on m_translate, m_rotate, and m_scale
		Matrix4 temp;
		//temp.CreateTranslation(m_translation);	
		//m_WorldTransform.Multiply(temp);
		m_WorldTransform.CreateTranslation(m_translation);
		temp.CreateFromQuaternion(m_rotation);
		m_WorldTransform.Multiply(temp);
		temp.CreateScale(m_scale);
		m_WorldTransform.Multiply(temp);
		GraphicsDevice::get().GetD3DDevice();
		GraphicsDevice::get().m_pDebugEffect->SetMatrix("gWorld", static_cast<D3DXMATRIX*>(m_WorldTransform.ToD3D()));
		D3DXHANDLE hTechnique = GraphicsDevice::get().m_pDebugEffect->GetTechniqueByName("DefaultTechnique");
		m_pMeshData->Draw(m_effect, hTechnique); 
	}
}

// Removes this MeshComponent from GraphicsDevice's MeshComponentSet
MeshComponent::~MeshComponent()
{
	GraphicsDevice::get().m_MeshComponentSet.erase(this);
}

}
