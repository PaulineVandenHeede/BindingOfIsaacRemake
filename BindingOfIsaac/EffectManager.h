#pragma once
#include "Effect.h"
#include "SoundEffect.h"
#include <vector>
#include <unordered_map>

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	// rule of 5
	EffectManager(const EffectManager& other) = delete;
	EffectManager(EffectManager&& other) = delete;
	EffectManager& operator=(const EffectManager& other) = delete;
	EffectManager& operator=(EffectManager&& other) = delete;

	void Draw() const;
	void Update(float elapsedSec);

	void AddEffect(const std::string& fileName, const Point2f& pos,
		float clipWidth, float clipHeight, float offsetX, float offsetY, float scale, bool temporary = false, int nrCols = 1,
		int nrRows = 1, int numberOfFrames = 1, int framesPerSec = 10);

	void AddSoundEffect(const std::string& fileName, bool loop);
	void StopAllSounds();
	void SetVolume(int value);
	void SetPause();
	void SetResume();
	void SetStop();

	void DeleteAllEffects();

private:
	std::vector<Effect*> m_pEffects;
	std::unordered_map<std::string, SoundEffect*> m_pSoundEffects;

	// --> HELPER FUNCTIONS
	void ReadyToDelete();
	void DeleteSoundEffects();
};

