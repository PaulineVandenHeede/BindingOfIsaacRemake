#include "pch.h"
#include "ScreenFlow.h"
#include "MenuScreen.h"
#include "Singleton.h"
#include "Texture.h"


MenuScreen::MenuScreen(const float width, const float height)
	: m_pMenu{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/game_menu.png") }
	, m_Width{ width }
	, m_Height{ height }
	, m_Scale{ 2.5f }
	, m_CurrentOption{ Option::newRun }
	, m_MaxOptions{ int(Option::end) }
{
}


void MenuScreen::Draw() const
{
	// draw menu
	Rectf srcRect{ 0.f, m_pMenu->GetHeight() / 2, m_pMenu->GetWidth(), m_pMenu->GetHeight() / 2 };
	Rectf destRect{ 0.f, 0.f, m_Width, m_Height };
	Point2f pos{ 0.f,0.f };

	m_pMenu->Draw(destRect, srcRect);

	// draw arrow
	srcRect = Rectf{ 0.f , 336.f, 32.f, 32.f };
	
	destRect = Rectf{ (126.f + (5.f * int(m_CurrentOption))) * m_Scale, (210.f - (29.f * int(m_CurrentOption) ))* m_Scale, 32.f * m_Scale, 32.f * m_Scale };
	pos = Point2f{ 150.f + 5.f * int(m_CurrentOption), 184.f - 29.f * int(m_CurrentOption)};
	m_pMenu->Draw(destRect, srcRect);
}

MenuScreen::Option MenuScreen::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch ( e.keysym.sym )
	{
	case SDLK_w:
	case SDLK_UP: // arrow goes up --> if that's possible (or can go in circles)
		if ((int(m_CurrentOption) - 1) < 0)
			m_CurrentOption = Option::options;
		else
			m_CurrentOption = Option(int(m_CurrentOption) - 1);
		break;
	case SDLK_s:
	case SDLK_DOWN: // arrow goes down --> if that's possible
		if ( (int(m_CurrentOption) + 1) >= int(Option::end) )
			m_CurrentOption = Option::newRun;
		else
			m_CurrentOption = Option(int(m_CurrentOption) + 1);
		break;
	case SDLK_RETURN: // go further
	case SDLK_KP_ENTER:
		switch (m_CurrentOption)
		{
		case Option::newRun:
		case Option::rerun:
		case Option::continueRun:
			ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::loading;
			break;
		case Option::options:
			ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::options;
			break;
		case Option::end:
			break;
		default:
			break;
		}
		
		// sound
		std::string fileName{ "Resources/Sounds/menu/book page turn.wav" };
		Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);


		return m_CurrentOption;
	}

	return Option::end;
}
