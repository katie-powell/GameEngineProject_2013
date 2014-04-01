// Implementations for GameWorld
#include "GameWorld.h"
#include "GameObject.h"
#include "../ini/minIni.h"
#include "../core/dbg_assert.h"
#include "../core/math.h"
#include "../graphics/GraphicsDevice.h"

namespace ITP485
{

// Setup data
void GameWorld::Setup()
{
	m_bPaused = false;
	m_pLevelFile = NULL;
}

// Cleanup will delete all the GameObjects
void GameWorld::Cleanup()
{
	//Iterate through m_GameObjects std::set, and delete all the GameObjects stored in it. Then clear out the set. Delete m_pLevelFile.
	for (std::set<GameObject*>::iterator itr = m_GameObjects.begin(); itr != m_GameObjects.end(); ++itr)
	{
		GameObject *gc = *itr;
		delete(gc);
	}
	m_GameObjects.clear();
	delete(m_pLevelFile);
}

// Update all GameObjects if not paused
void GameWorld::Update(float fDelta)
{
	if(fDelta > 0.1f)
	{
		fDelta = 0.1f;
	}
	if(!m_bPaused)
	{
		for (std::set<GameObject*>::iterator itr = m_GameObjects.begin(); itr != m_GameObjects.end(); ++itr)
		{
			GameObject *gc = *itr;
			gc->Update(fDelta);
		}
	}
}

// Load in the level file
// Returns true if succeeded
bool GameWorld::LoadLevel(const char* szLevelFile)
{
	
	m_pLevelFile = new minIni(szLevelFile);
	Dbg_Assert(szLevelFile == "level.ini", "Error: level.ini file does not exist!");
	int x = 0;
	while(m_pLevelFile->getsection(x) != "")
	{
		if(m_pLevelFile->getsection(x) == "Camera")
		{	
			std::string name;
			float ex,ey,ez,ax,ay,az,ux,uy,uz;
			name = (m_pLevelFile->gets(m_pLevelFile->getsection(x), "Eye"));
			sscanf_s(name.c_str(), "(%f,%f,%f)", &ex, &ey, &ez);
			
			name = (m_pLevelFile->gets(m_pLevelFile->getsection(x), "At"));
			sscanf_s(name.c_str(), "(%f,%f,%f)", &ax, &ay, &az);

			name = (m_pLevelFile->gets(m_pLevelFile->getsection(x), "Up"));
			sscanf_s(name.c_str(), "(%f,%f,%f)", &ux, &uy, &uz);

			Matrix4 tempM = Matrix4::Identity;
			tempM.CreateLookAt(Vector3(ex,ey,ez),Vector3(ax,ay,az),Vector3(ux,uy,uz));
			GraphicsDevice::get().setCameraMatrix(&tempM);
		}
		else if(m_pLevelFile->getsection(x) == "Projection")
		{
			float fovy,ar,nz,fz;
			std::string name;
			name = (m_pLevelFile->gets(m_pLevelFile->getsection(x), "FOVy"));
			sscanf_s(name.c_str(), "%f", &fovy);
			name = (m_pLevelFile->gets(m_pLevelFile->getsection(x), "AspectRatio"));
			sscanf_s(name.c_str(), "%f", &ar);
			name = (m_pLevelFile->gets(m_pLevelFile->getsection(x), "NearZ"));
			sscanf_s(name.c_str(), "%f", &nz);
			name = (m_pLevelFile->gets(m_pLevelFile->getsection(x), "FarZ"));
			sscanf_s(name.c_str(), "%f", &fz);

			Matrix4 tempM = Matrix4::Identity;
			tempM.CreatePerspectiveFOV(fovy, ar,nz, fz);
			GraphicsDevice::get().setProjectionMatrix(&tempM);
		}
		else
		{
			std::string temp = m_pLevelFile->getsection(x);
			GameObject *go = new GameObject();
			go->Spawn(temp, m_pLevelFile);
			m_GameObjects.insert(go);
		}
		x++;
	}
	return true;
}

}
