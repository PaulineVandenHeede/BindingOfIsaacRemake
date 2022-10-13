#pragma once
#include "ScreenFlow.h"
class Texture;

class DeadScreen
{
public:
	DeadScreen(float scale);
	~DeadScreen();

	void Draw() const;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

private:
	Texture* m_pTexture;

	float m_Scale;
};

