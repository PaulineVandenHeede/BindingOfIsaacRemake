#pragma once

class Texture;
class Sprite;

class OptionsScreen
{
public:
	enum class Option
	{
		sfx = 0,
		music = 1,
		controls = 2,
		end = 4
	};
	OptionsScreen();
	~OptionsScreen();

	// rule of 5
	OptionsScreen(const OptionsScreen& other) = delete;
	OptionsScreen(OptionsScreen&& other) = delete;
	OptionsScreen& operator=(const OptionsScreen& other) = delete;
	OptionsScreen& operator=(OptionsScreen&& other) = delete;

	void Draw();
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

private:
	Texture* m_pTexture;
	Sprite* m_pSFXBar;
	Sprite* m_pMusicBar;

	float m_Scale;

	Option m_CurrentOption;

	int m_MaxFrames;
	int m_CurrentFrameSFX;
	int m_CurrentFrameMusic;
	int m_MaxVolume;

	bool m_ShowControls;
};

