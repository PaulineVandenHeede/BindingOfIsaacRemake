#include "pch.h"
#include "DeadScreen.h"
#include "Singleton.h"
#include "Texture.h"

DeadScreen::DeadScreen(float scale)
	: m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/died_menu.png") }
	, m_Scale{ scale }
{
}


DeadScreen::~DeadScreen()
{
}

void DeadScreen::Draw() const
{
	Rectf destRect{ 0.f,0.f, m_pTexture->GetWidth() * m_Scale ,m_pTexture->GetHeight() * m_Scale };
	m_pTexture->Draw(destRect);
}

void DeadScreen::ProcessKeyUpEvent(const SDL_KeyboardEvent & e)
{
	switch (e.keysym.sym)
	{
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::menu;
		break;
	default:
		break;
	}

}
