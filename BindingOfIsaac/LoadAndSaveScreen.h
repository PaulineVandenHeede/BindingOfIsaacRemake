#pragma once

class Texture;
class Sprite;
class AreYouSureScreen;

enum class File
{
	fileOne = 0,
	fileTwo = 1,
	fileThree = 2,
	none = 3,
	end = 4
};

class LoadAndSaveScreen
{
public:
	LoadAndSaveScreen(float scale);
	~LoadAndSaveScreen();

	// rule of 5
	LoadAndSaveScreen(const LoadAndSaveScreen& other) = delete;
	LoadAndSaveScreen(LoadAndSaveScreen&& other) = delete;
	LoadAndSaveScreen& operator=(const LoadAndSaveScreen& other) = delete;
	LoadAndSaveScreen& operator=(LoadAndSaveScreen&& other) = delete;

	void Draw() const;
	std::pair<bool,File> ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

private:
	AreYouSureScreen* m_pAreYouSure;
	Sprite* m_pSprite;

	Texture* m_pTexture;

	float m_Scale;
	
	File m_File;
	File m_PrevFile;

	bool m_Chosen;
};

