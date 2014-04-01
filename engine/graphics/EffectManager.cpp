// Implementation for our EffectManager
#include "EffectManager.h"
#include "MeshData.h"
#include "GraphicsDevice.h"

namespace ITP485
{


void EffectManager::Setup()
{

}


void EffectManager::Cleanup()
{
	std::map<unsigned int, LPD3DXEFFECT>::iterator it;
	for (std::map<unsigned int, LPD3DXEFFECT>::iterator itr = m_EffectMap.begin(); itr != m_EffectMap.end(); ++itr)
	{
		LPD3DXEFFECT effect = itr->second;
		effect->Release();
	}
	m_EffectMap.clear();
}


LPD3DXEFFECT EffectManager::LoadEffectFile(const char* szEffectFile)
{
	std::map<unsigned int, LPD3DXEFFECT>::iterator it;
	it = m_EffectMap.find(HashString(szEffectFile));
	if(it == m_EffectMap.end()) //if File is not found
	{
		LPD3DXEFFECT newEffect = GraphicsDevice::get().LoadEffect(szEffectFile);
		m_EffectMap[HashString(szEffectFile)] = newEffect;
		return newEffect;
	}
	else
	{
		return m_EffectMap[HashString(szEffectFile)];
	}
}


unsigned int EffectManager::HashString(const char* str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

}
