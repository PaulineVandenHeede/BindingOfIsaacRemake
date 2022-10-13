#include "pch.h"
#include "ScreenFlow.h"
#include "StartScreen.h"
#include "Singleton.h"
#include "Texture.h"
#include <string>

StartScreen::StartScreen(float scale)
	: m_Scale{ scale }
	, m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/title_menu.png") }
{

}

StartScreen::~StartScreen()
{
}

void StartScreen::Draw() const
{
	Rectf srcRect{ 0.f, m_pTexture->GetHeight() /2, m_pTexture->GetWidth(), m_pTexture->GetHeight()/2 };
	Rectf destRect{ 0.f,0.f, m_pTexture->GetWidth() * m_Scale, (m_pTexture->GetHeight()/2) * m_Scale };
	m_pTexture->Draw(destRect, srcRect);
}

void StartScreen::ProcessKeyUpEvent(const SDL_KeyboardEvent & e)
{
	switch (e.keysym.sym)
	{
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		{
			ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::loadSave;
			// sound
			std::string fileName{ "Resources/Sounds/menu/book page turn.wav" };
			Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);

			break;
		}
	default:
		break;
	}
}
