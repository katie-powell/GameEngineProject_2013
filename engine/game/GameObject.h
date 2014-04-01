// Defines the base GameObject class, which has references to all components
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include "../ini/minIni.h"

namespace ITP485
{

class MeshComponent;

class GameObject
{
public:
	// Sets component pointers to NULL
	GameObject();

	// Deletes any components which were initialized
	virtual ~GameObject();

	// Spawn this object based on ObjectName
	// Returns true if successfully spawned
	virtual bool Spawn(std::string sObjectName, minIni *LevelFile);

	// Update this GameObject
	virtual void Update(float fDelta);

protected:
	// We use std::string for ObjectName because of minINI
	std::string m_sObjectName;
	// Component pointers
	MeshComponent* m_pMeshComponent;
};

}

#endif // _GAMEOBJECT_H_
