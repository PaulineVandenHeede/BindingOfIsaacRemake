#pragma once

class Texture;

class EndScreen
{
public:
	EndScreen();

	void Draw() const;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

private:
	Texture* m_pTexture;
};

