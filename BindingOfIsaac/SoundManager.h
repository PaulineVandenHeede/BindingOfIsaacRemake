#pragma once

class SoundStream;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	// rule of 5
	SoundManager(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;

	void Update();
	void SetPause();
	void SetResume();
	void SetStop();
	void SetVolume(int value);
	int GetVolume() const;

private:
	std::vector<SoundStream*> m_pPlaylist;
	int m_Index;

	bool m_Stop;

	void DeletePlaylist();
};

