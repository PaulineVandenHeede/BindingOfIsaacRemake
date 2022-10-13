#include "pch.h"
#include "ControlsScreen.h"
#include "ScreenFlow.h"
#include "Singleton.h"
#include "Texture.h"
#include "utils.h"

ControlsScreen::ControlsScreen()
	: m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/controls_menu.png") }
	, m_Scale{ 2.5f }
{
}


void ControlsScreen::Draw() const
{
	Rectf srcRect{ 0.f, m_pTexture->GetHeight(), m_pTexture->GetWidth(), m_pTexture->GetHeight() / 2 };
	Rectf destRect{0.f,0.f, srcRect.width * m_Scale, srcRect.height * m_Scale};

	m_pTexture->Draw(destRect, srcRect);
}

void ControlsScreen::ProcessKeyUpEvent(const SDL_KeyboardEvent & e)
{
	switch (e.keysym.sym)
	{
	case SDLK_i:
		Singleton::GetInstance()->GetSoundManager()->SetResume();
		Singleton::GetInstance()->GetEffectManager()->SetResume();
		ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::game;
		break;
	default:
		break;
	}
}
