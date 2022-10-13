#pragma once

class Texture;

class AreYouSureScreen
{
public:
	AreYouSureScreen(const float scale);
	void Draw() const;
	int ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

private:
	Texture* m_pTexture;

	float m_Scale;
	float m_OffsetY;

	int m_Pos;
};

