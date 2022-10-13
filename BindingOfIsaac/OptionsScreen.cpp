#include "pch.h"
#include "OptionsScreen.h"
#include "Sprite.h"
#include "Singleton.h"
#include "ScreenFlow.h"
#include "Texture.h"

OptionsScreen::OptionsScreen()
	: m_MaxVolume{ 100 }
	, m_MaxFrames{ 10 }
	, m_Scale{ 2.5f }
	, m_CurrentFrameMusic{ Singleton::GetInstance()->GetSoundManager()->GetVolume()/10 }
	, m_CurrentFrameSFX{ ScreenFlow::GetVolume()/10 }
	, m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/options_menu.png") }
{
	m_pSFXBar = new Sprite{m_pTexture, 1, 10, 10, 64.f, 16.f, 0.f, 344.f, 10};
	m_pMusicBar = new Sprite{ m_pTexture, 1, 10, 10, 64.f, 16.f, 0.f, 344.f, 10 };

	m_pSFXBar->SetActFrame(m_CurrentFrameSFX);
	m_pMusicBar->SetActFrame(m_CurrentFrameMusic);
}

OptionsScreen::~OptionsScreen()
{
	delete m_pSFXBar;
	m_pSFXBar = nullptr;

	delete m_pMusicBar;
	m_pMusicBar = nullptr;

	m_pTexture = nullptr;
}

void OptionsScreen::Draw()
{
	// draw menu
	Rectf srcRect{ 0.f, m_pTexture->GetHeight() / 2, m_pTexture->GetWidth(), m_pTexture->GetHeight() / 2 };
	Rectf destRect{ 0.f, 0.f, m_pTexture->GetWidth() * m_Scale , (m_pTexture->GetHeight() / 2) * m_Scale };

	m_pTexture->Draw(destRect, srcRect);

	// draw arrow
	srcRect = Rectf{ 0.f , 328.f, 32.f, 32.f };
	destRect = Rectf{ (125.f + (7.f * int(m_CurrentOption))) * m_Scale, (195.f - (30.f * int(m_CurrentOption)))* m_Scale, 32.f * m_Scale, 32.f * m_Scale };

	m_pTexture->Draw(destRect, srcRect);

	// draw bars
	// sfx
	m_pSFXBar->Draw(Point2f{ 197 * m_Scale, 206 * m_Scale }, m_Scale, m_Scale);
	// music
	m_pMusicBar->Draw(Point2f{ 210 * m_Scale, 175 * m_Scale }, m_Scale, m_Scale);
}

void OptionsScreen::ProcessKeyUpEvent(const SDL_KeyboardEvent & e)
{
	switch (e.keysym.sym)
	{
	case SDLK_s:
	case SDLK_DOWN:
		m_CurrentOption = Option((int(m_CurrentOption) + 1) % (int(Option::end) - 1));
		break;
	case SDLK_w:
	case SDLK_UP:
		m_CurrentOption = Option(int(m_CurrentOption) - 1);
		if (int(m_CurrentOption) < 0)
			m_CurrentOption = Option::controls;
		break;
	default:
		break;
	}

	switch (m_CurrentOption)
	{
	case OptionsScreen::Option::sfx:
		// change volume
		switch (e.keysym.sym)
		{
		case SDLK_d:
		case SDLK_RIGHT:
			if (++m_CurrentFrameSFX > m_MaxFrames)
				m_CurrentFrameSFX = m_MaxFrames;

			ScreenFlow::SetVolume(m_CurrentFrameSFX * m_MaxFrames);
			Singleton::GetInstance()->GetEffectManager()->SetVolume(m_CurrentFrameSFX * m_MaxFrames);
			m_pSFXBar->SetActFrame(m_CurrentFrameSFX);
			break;
		case SDLK_a:
		case SDLK_LEFT:
			if (--m_CurrentFrameSFX < 0)
				m_CurrentFrameSFX = 0;
			ScreenFlow::SetVolume(m_CurrentFrameSFX * m_MaxFrames);
			Singleton::GetInstance()->GetEffectManager()->SetVolume(m_CurrentFrameSFX * m_MaxFrames);
			m_pSFXBar->SetActFrame(m_CurrentFrameSFX);
			break;
		default:
			break;
		}
		break;
	case OptionsScreen::Option::music:
		switch (e.keysym.sym)
		{
		case SDLK_d:
		case SDLK_RIGHT:
			if (++m_CurrentFrameMusic > m_MaxFrames)
				m_CurrentFrameMusic = m_MaxFrames;
			Singleton::GetInstance()->GetSoundManager()->SetVolume(m_CurrentFrameMusic * m_MaxFrames);
			m_pMusicBar->SetActFrame(m_CurrentFrameMusic);
			break;
		case SDLK_a:
		case SDLK_LEFT:
			if (--m_CurrentFrameMusic < 0)
				m_CurrentFrameMusic = 0;
			Singleton::GetInstance()->GetSoundManager()->SetVolume(m_CurrentFrameMusic * m_MaxFrames);
			m_pMusicBar->SetActFrame(m_CurrentFrameMusic);
			break;
		default:
			break;
		}
		break;
	case OptionsScreen::Option::controls:
		switch (e.keysym.sym)
		{
		case SDLK_RETURN:
		case SDLK_KP_ENTER:
			ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::controls;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

