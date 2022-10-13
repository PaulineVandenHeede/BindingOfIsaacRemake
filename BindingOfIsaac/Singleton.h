#pragma once
#include "EffectManager.h"
#include "TextureManager.h"
#include "PowerUpManager.h"
#include "SoundManager.h"

class Singleton
{
public:
	static Singleton* GetInstance();
	~Singleton();

	// rule of 5 --> there's only one so this isn't a possibility
	// that's why the constructor is private
	// becuase we'll never call it --> create singleton in GetInstance
	Singleton(const Singleton& other) = delete;
	Singleton(Singleton&& other) = delete;
	Singleton& operator=(const Singleton& other) = delete;
	Singleton& operator=(Singleton&& other) = delete;

	EffectManager* GetEffectManager();
	TextureManager* GetTextureManager();
	PowerUpManager* GetPowerUpManager();
	SoundManager* GetSoundManager();

	PowerUpManager* GetNewPowerUpManager(std::stringstream& info);
	PowerUpManager* GetNewEmptyPowerUpManager();

private:
	Singleton();

	static Singleton* m_pSingleton;

	EffectManager* m_pEffectManager;
	PowerUpManager* m_pPowerUpManager;
	TextureManager* m_pTextureManager;
	SoundManager* m_pSoundManager;
};

