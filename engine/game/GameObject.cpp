// Implementations for GameObject
#include "GameObject.h"
#include "../components/MeshComponent.h"
#include "../graphics/EffectManager.h"

namespace ITP485
{
	

// Sets component pointers to NULL
GameObject::GameObject()
{
	m_pMeshComponent = NULL;
}

// Deletes any components which were initialized
GameObject::~GameObject()
{
	delete(m_pMeshComponent);
}

// Spawn this object based on ObjectName
// Returns true if successfully spawned
bool GameObject::Spawn(std::string sObjectName, minIni *LevelFile)
{
	//m_pMeshComponent = new MeshComponent("cube.itpmesh");

	std::string name = (LevelFile->gets(sObjectName, "Mesh"));
	if(name != "")
	{
		m_pMeshComponent = new MeshComponent(name.c_str());

		float x,y,z;

		name = (LevelFile->gets(sObjectName, "Position")); //creating a new string for position would just take up excess data
		if(name.length()>0)
		{
			sscanf_s(name.c_str(), "(%f,%f,%f)", &x, &y, &z);
			m_pMeshComponent->SetTranslation(&Vector3(x,y,z));
		}
		else
		{
			m_pMeshComponent->SetTranslation(&Vector3(0,0,0));
		}
		
		//TODO: Multiply Quaternions

		name = (LevelFile->gets(sObjectName, "Rotation"));
		if(name.length()>0)
		{
			sscanf_s(name.c_str(), "(%f,%f,%f)", &x, &y, &z);
			Quaternion tempQ = Quaternion::Identity;
			Quaternion tempQ2 = Quaternion(Vector3::UnitY,x);
			tempQ.Multiply(tempQ2);
			tempQ2 = Quaternion(Vector3::UnitX,y);
			tempQ.Multiply(tempQ2);
			tempQ2 = Quaternion(Vector3::UnitZ,z);
			tempQ.Multiply(tempQ2);
			m_pMeshComponent->SetRotation(&tempQ);
		}
		else
		{
			m_pMeshComponent->SetRotation(&Quaternion(0,0,0,1.0f));
		}

		name = (LevelFile->gets(sObjectName, "Effect"));
		if(name.length()>0)
		{
			m_pMeshComponent->SetEffect(EffectManager::get().LoadEffectFile(name.c_str()));
		}
		else
		{
			//maybe do other stuffs
		}

		float scaly = LevelFile->getf(sObjectName, "Scale");
		if(scaly == 0)
			scaly = 1.0f;
		m_pMeshComponent->SetScale(scaly);
	}


	return true;
}

// Update this GameObject
void GameObject::Update(float fDelta)
{
	//m_pMeshComponent->GetWorldTransform().CreateRotationY(Pi/2*fDelta);
	//Vector3 tempV = Vector3(0, 1.0f, 0);
	///Quaternion temp = Quaternion::Identity;
	//temp.Set(0, (float)Pi/2.0f*fDelta, 0, 1.0f);
	//m_pMeshComponent->SetRotation(temp);
	//m_pMeshComponent->Draw();
}

}
