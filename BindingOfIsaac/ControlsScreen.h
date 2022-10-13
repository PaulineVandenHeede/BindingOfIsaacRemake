#pragma once
class Texture;
class TextureManager;

class ControlsScreen
{
public:
	ControlsScreen();

	void Draw() const;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

private:
	Texture* m_pTexture;

	float m_Scale;
};