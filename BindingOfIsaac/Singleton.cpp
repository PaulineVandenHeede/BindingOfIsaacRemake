#include "pch.h"
#include "Singleton.h"

Singleton* Singleton::m_pSingleton{ nullptr };

Singleton* Singleton::GetInstance()
{
	if (m_pSingleton == nullptr)
	{
		m_pSingleton = new Singleton{};
	}

	return m_pSingleton;
}

EffectManager * Singleton::GetEffectManager()
{
	return m_pEffectManager;
}

TextureManager * Singleton::GetTextureManager()
{
	return m_pTextureManager;
}

PowerUpManager * Singleton::GetPowerUpManager()
{
	return m_pPowerUpManager;
}

SoundManager* Singleton::GetSoundManager()
{
	return m_pSoundManager;
}

PowerUpManager* Singleton::GetNewPowerUpManager(std::stringstream& info)
{
	delete m_pPowerUpManager;
	m_pPowerUpManager = nullptr;

	m_pPowerUpManager = new PowerUpManager{ info };
	return m_pPowerUpManager;
}

PowerUpManager* Singleton::GetNewEmptyPowerUpManager()
{
	delete m_pPowerUpManager;
	m_pPowerUpManager = nullptr;

	m_pPowerUpManager = new PowerUpManager{};
	return m_pPowerUpManager;
}

Singleton::Singleton()
	: m_pEffectManager{ new EffectManager{} }
	, m_pPowerUpManager{ new PowerUpManager{} }
	, m_pTextureManager{ new TextureManager{} }
	, m_pSoundManager{ new SoundManager{} }
{

}

Singleton::~Singleton()
{
	delete m_pEffectManager;
	m_pEffectManager = nullptr;

	delete m_pPowerUpManager;
	m_pPowerUpManager = nullptr;

	delete m_pTextureManager;
	m_pTextureManager = nullptr;

	delete m_pSoundManager;
	m_pSoundManager = nullptr;
}
