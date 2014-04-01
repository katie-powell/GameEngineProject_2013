// Defines the entire game world
#ifndef _GAMEWORLD_H_
#define _GAMEWORLD_H_
#include "../core/singleton.h"
#include <set>

class minIni;
namespace ITP485
{

class GameObject;

class GameWorld : public Singleton<GameWorld>
{
	DECLARE_SINGLETON(GameWorld);
public:
	// Setup data
	void Setup();

	// Cleanup will delete all the GameObjects
	void Cleanup();

	// Update all GameObjects if not paused
	void Update(float fDelta);

	// Load in the level file
	// Returns true if succeeded
	bool LoadLevel(const char* szLevelFile);

	bool IsPaused() const { return m_bPaused; }
	void SetPaused(bool bValue) { m_bPaused = bValue; }
	
private:
	// Stores all the game objects in the world
	std::set<GameObject*> m_GameObjects;
	// This is our level ini file pointer
	minIni* m_pLevelFile;
	// Master bool for pausing/unpausing the game
	bool m_bPaused;
};

}

#endif // _GAMEWORLD_H_
