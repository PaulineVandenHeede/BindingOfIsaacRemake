#pragma once

class Texture;

class StartScreen
{
public:
	StartScreen(float scale);
	~StartScreen();

	void Draw() const;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

private:
	Texture* m_pTexture;
	float m_Scale;
};
