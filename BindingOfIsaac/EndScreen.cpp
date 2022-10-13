#include "pch.h"
#include "EndScreen.h"
#include "Texture.h"
#include "Singleton.h"
#include "ScreenFlow.h"

EndScreen::EndScreen()
	: m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/end_menu.png") }
{
}

void EndScreen::Draw() const
{
	glPushMatrix();

	glScalef(2.5f, 2.5f, 0);
	m_pTexture->Draw();

	glPopMatrix();
}

void EndScreen::ProcessKeyUpEvent(const SDL_KeyboardEvent & e)
{
	switch (e.keysym.sym)
	{
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::loadSave;
		break;
	default:
		break;
	}
}

