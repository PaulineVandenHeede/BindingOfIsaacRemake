#include "pch.h"
#include "LoadAndSaveScreen.h"
#include "Singleton.h"
#include "Texture.h"
#include "Sprite.h"
#include "ScreenFlow.h"
#include "AreYouSureScreen.h"

LoadAndSaveScreen::LoadAndSaveScreen(float scale)
	: m_pAreYouSure{ new AreYouSureScreen{ scale } }
	, m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/load_and_save_menu.png") }
	, m_Scale{ scale }
	, m_Chosen{ false }
	, m_File{ File::fileOne }
{
	m_pSprite = new Sprite{ m_pTexture, 2, 2, 4, 452.f, 296.f, 0.f, 296.f, 0 };
}

LoadAndSaveScreen::~LoadAndSaveScreen()
{
	delete m_pSprite;
	m_pSprite = nullptr;

	delete m_pAreYouSure;
	m_pAreYouSure = nullptr;
}

void LoadAndSaveScreen::Draw() const
{
	m_pSprite->Draw(Point2f{ 0.f,0.f }, m_Scale, m_Scale);
	if (m_Chosen)
		m_pAreYouSure->Draw();
}

std::pair<bool,File> LoadAndSaveScreen::ProcessKeyUpEvent(const SDL_KeyboardEvent & e)
{
	if (m_Chosen)
	{
		int choice = m_pAreYouSure->ProcessKeyUpEvent(e);
		if (choice == 1)
		{
			std::pair<bool, File> temp{false, File::end};
			if (m_File == File::none)
			{
				temp.first = false;
				temp.second = m_PrevFile;
			}
			else
			{
				temp.first = true;
				temp.second = m_File;
			}

			ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::loading;
			m_Chosen = false;

			return temp;
		}
		else if (choice == -1)
		{
			m_Chosen = false;
		}

		return std::make_pair(false,File::end);
	}

	switch (e.keysym.sym)
	{
	case SDLK_LEFT:
		if (m_File == File::none) // when delete frame is active don't be able to go left or right
			break;

		m_File = File(int(m_File) - 1);
		if (int (m_File) < 0)
			m_File = File::fileThree;

		m_pSprite->SetActFrame(int(m_File));
		break;
	case SDLK_RIGHT:
		if (m_File == File::none)
			break;

		m_File = File(int(m_File) + 1);
		if (int(m_File) >= int(File::none))
			m_File = File::fileOne;

		m_pSprite->SetActFrame(int(m_File));
		break;
	case SDLK_DOWN:
		m_PrevFile = m_File;
		m_File = File::none;

		m_pSprite->SetActFrame(int(m_File));
		break;
	case SDLK_UP:
		if (m_File != File::none)
			break;
		m_File = m_PrevFile;

		m_pSprite->SetActFrame(int(m_File));
		break;
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		if (!m_Chosen)
			m_Chosen = true;
	}

	return std::make_pair(false, File::end);
}
