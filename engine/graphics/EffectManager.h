
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_
#include "../core/singleton.h"
#include <map>
#include <d3dx9effect.h>

namespace ITP485
{

struct MeshData;

class EffectManager : public Singleton<EffectManager>
{
	DECLARE_SINGLETON(EffectManager);
public:
	
	void Setup();
	
	
	void Cleanup();


	LPD3DXEFFECT LoadEffectFile(const char*);

private:
	
	unsigned int HashString(const char* str);

	std::map<unsigned int, LPD3DXEFFECT> m_EffectMap;
};

} // namespace
#endif // _EFFECTMANAGER_H_
