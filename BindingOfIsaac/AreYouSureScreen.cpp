#include "pch.h"
#include "AreYouSureScreen.h"
#include "Singleton.h"
#include "ScreenFlow.h"
#include "Texture.h"

AreYouSureScreen::AreYouSureScreen(const float scale)
	: m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/are_you_sure_menu.png") }
	, m_Scale{ scale }
	, m_OffsetY{ 32.f }
	, m_Pos{ 0 }
{
}

void AreYouSureScreen::Draw() const
{
	Rectf srcRect{ 0.f, m_pTexture->GetHeight() - 32.f, m_pTexture->GetWidth(), m_pTexture->GetHeight() - m_OffsetY};
	Rectf destRect{ 0.f, 0.f, srcRect.width * m_Scale, srcRect.height * m_Scale };
	Point2f pos{ 0.f,0.f };

	m_pTexture->Draw(destRect, srcRect);

	srcRect = Rectf{ 0.f, m_pTexture->GetHeight(), m_OffsetY, m_OffsetY };
	destRect = Rectf{ (132.f + (m_Pos * 87.f))* m_Scale, 102.f * m_Scale, srcRect.width * m_Scale, srcRect.height * m_Scale };

	m_pTexture->Draw(destRect, srcRect);
}

int AreYouSureScreen::ProcessKeyUpEvent(const SDL_KeyboardEvent & e)
{
	// return 0 if the state didn't change
	// return -1 if arrow is pointing at no
	// return 1 if arrow is pointing at yes
	switch (e.keysym.sym)
	{
	case SDLK_ESCAPE:
		return 0;
	case SDLK_LEFT:
		if (--m_Pos < 0)
			m_Pos = 1;
		return 0;
	case SDLK_RIGHT: 
		++m_Pos %= 2;
		return 0;
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		if (m_Pos == 0)
		{
			// sound
			std::string fileName{ "Resources/Sounds/menu/book page turn.wav" };
			Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);

			return 1;
		}
		else
		{
			m_Pos = 0;
			return -1;
		}
	default:
		return 0;
	}
	return 0;
}

