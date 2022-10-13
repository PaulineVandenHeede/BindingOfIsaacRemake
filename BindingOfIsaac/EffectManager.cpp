#include "pch.h"
#include "EffectManager.h"
#include "ScreenFlow.h"
#include <algorithm>

using mapIt = std::unordered_map<std::string, SoundEffect*>::iterator;
using mapCit = std::unordered_map<std::string, SoundEffect*>::const_iterator;


EffectManager::EffectManager()
	: m_pEffects{  }
{
}

EffectManager::~EffectManager()
{
	DeleteAllEffects();
	DeleteSoundEffects();
}

void EffectManager::Draw() const
{
	for (Effect* pEffect : m_pEffects)
	{
		pEffect->Draw();
	}
}

void EffectManager::Update(float elapsedSec)
{
	for (Effect* pEffect : m_pEffects)
	{
		pEffect->Update(elapsedSec);
	}

	ReadyToDelete();
}

void EffectManager::AddEffect(const std::string & fileName, const Point2f & pos,
	float clipWidth, float clipHeight, float offsetX, float offsetY, float scale, bool temporary, int nrCols,
	int nrRows, int numberOfFrames, int framesPerSec)
{
	m_pEffects.push_back(new Effect{ fileName, pos, clipWidth, clipHeight, offsetX, offsetY, scale, temporary, nrCols, nrRows, numberOfFrames, framesPerSec });
}

void EffectManager::AddSoundEffect(const std::string & fileName, bool loop)
{
	std::pair<mapIt, bool> it;

	it = m_pSoundEffects.emplace(fileName, nullptr);
	if (it.second)
	{
		m_pSoundEffects[fileName] = new SoundEffect{ fileName };
	}

	if (loop)
	{
		it.first->second->Play(-1);
		it.first->second->SetVolume(ScreenFlow::GetVolume());
	}
	else
	{
		it.first->second->Play(0);
		it.first->second->SetVolume(ScreenFlow::GetVolume());
	}
}

void EffectManager::StopAllSounds()
{
	mapIt it{ m_pSoundEffects.begin() };
	it->second->StopAll();
}



void EffectManager::SetVolume(int value)
{
	if (!m_pSoundEffects.empty())
	{
		for (std::pair<std::string, SoundEffect*> pSound : m_pSoundEffects)
		{
			pSound.second->SetVolume(value);
		}
	}
}

void EffectManager::SetPause()
{
	if (!m_pSoundEffects.empty())
	{
		m_pSoundEffects.begin()->second->PauseAll();
	}
}

void EffectManager::SetResume()
{
	if (!m_pSoundEffects.empty())
	{
		m_pSoundEffects.begin()->second->ResumeAll();
	}
}

void EffectManager::SetStop()
{
	if (!m_pSoundEffects.empty())
	{
		m_pSoundEffects.begin()->second->StopAll();
	}
}

void EffectManager::ReadyToDelete()
{
	// check if ready to delete
	std::vector<Effect*>::iterator it{};

	it = std::remove_if(m_pEffects.begin(), m_pEffects.end(), [](Effect* pEffect) {
		if (pEffect->ReadyToDelete())
		{
			delete pEffect;
			pEffect = nullptr;
			return true;
		}

		return false;
	});

	m_pEffects.erase(it, m_pEffects.end());
}

void EffectManager::DeleteSoundEffects()
{
	for (std::pair<std::string, SoundEffect*> pSEffect : m_pSoundEffects)
	{
		delete pSEffect.second;
		pSEffect.second = nullptr;
	}

	m_pSoundEffects.clear();
}

void EffectManager::DeleteAllEffects()
{
	for (size_t i{}; i < m_pEffects.size(); ++i)
	{
		delete m_pEffects[i];
	}
	m_pEffects.clear();
}