#include "pch.h"
#include "ScreenFlow.h"
#include "Game.h"
#include "Stage.h"
#include "Character.h"
#include "BossRoom.h"
#include "Boss.h"
#include "Tear.h"
#include "EffectManager.h"
#include "Singleton.h"
#include "ChargeBar.h"

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_pEffectManager{ Singleton::GetInstance()->GetEffectManager() }
	, m_pStage{ new Stage{1} }
	, m_pIsaac{ nullptr }
	, m_Camera{ window.width, window.height }
	, m_Scale{ 2.5f }
	, m_ScaleBoss{ 3.f }
	, m_Lost{ false }
	, m_MiniMap{ 2.5f }
{
	Initialize( );

	std::pair<int, int> pos = m_pStage->GetActiveRoom()->GetGridPosition();
	float roomWidth{ 226.f * m_Scale * 2.f};
	float roomHeight{ 148.f * m_Scale * 2.f };

	m_pIsaac = new Character{ Point2f{ pos.first * roomWidth + (roomWidth / 2), -pos.second * roomHeight + (roomHeight / 2) } };

	m_Camera.SetLevelBoundaries(Point2f{ pos.first * roomWidth, -pos.second * roomHeight });

	// add rooms to minimap
	m_pStage->InitialiseMiniMap(m_MiniMap);
}

Game::Game(std::stringstream& info, const Window& window, bool rerun)
	: m_Window{ window }
	, m_pEffectManager{ Singleton::GetInstance()->GetEffectManager() }
	, m_pStage{ nullptr }
	, m_pIsaac{ nullptr }
	, m_Camera{ window.width, window.height }
	, m_Scale{ 2.5f }
	, m_ScaleBoss{ 3.f }
	, m_MiniMap{ 2.5 }
{
	float roomWidth{ 226.f * m_Scale * 2.f };
	float roomHeight{ 148.f * m_Scale * 2.f };

	std::string stage{};
	std::string character{};
	std::string pum{};

	// get all parts
	stage = utils::GetPartValue("Stage", info.str());
	character = utils::GetPartValue("Character", info.str());
	pum = utils::GetPartValue("PowerUpManager", info.str());

	// initialise stage
	// clear info

	info.str(std::string{});
	if (stage != "")
	{
		info << stage;
		m_pStage = new Stage{ info, rerun };
	}

	// initialise character
	// clear info
	info.str(std::string{});
	std::stringstream characterCopy{};
	characterCopy << character;
	m_pIsaac = new Character{ characterCopy };

	m_Lost = (m_pIsaac->GetHealthBar().GetHealth() == 0);


	std::pair<int, int> pos{};
	if (m_pStage->GetStageCleared())
	{
		pos = m_pStage->GetBossRoom()->GetGridPosition();

		// set camera boundaries
		float width{ roomWidth / m_Scale * m_ScaleBoss };
		float height{ roomHeight / m_Scale * m_ScaleBoss };
		m_Camera.SetLevelBoundaries(Rectf{ float(pos.first), float(pos.second), width, height });

	}
	else
	{
		pos = m_pStage->GetActiveRoom()->GetGridPosition();
		Point2f newPos{ pos.first * roomWidth, -pos.second * roomHeight };

		// set camera boundaries
		m_Camera.SetLevelBoundaries(newPos);

		// add rooms to minimap
		m_pStage->InitialiseMiniMap(m_MiniMap);

		// powerup manager
		if (rerun)
		{
			Singleton::GetInstance()->GetNewEmptyPowerUpManager();

			// max health
			m_pIsaac->GetHealthBar().DoHealing(6);
		}
		else
		{
			// make new power up manager with the info of the file
			std::stringstream pumCopy{};
			info.str(std::string{});
			pumCopy << pum;

			Singleton::GetInstance()->GetNewPowerUpManager(pumCopy);
		}

		//set character
		Point2f newPosCharacter{ newPos.x + roomWidth / 2, newPos.y + roomHeight / 2 };
		m_pIsaac->SetPosition(newPosCharacter);
	}

	
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
}

void Game::Cleanup( )
{
	delete m_pIsaac;
	m_pIsaac = nullptr;

	delete m_pStage;
	m_pStage = nullptr;
}

void Game::Update( float elapsedSec )
{
	m_Camera.Update(elapsedSec);

	if (m_pEffectManager != nullptr)
		m_pEffectManager->Update(elapsedSec);

	if (m_Camera.GetZoomIn())
		return;

	if (m_pIsaac != nullptr && m_pIsaac->IsDead())
	{
		m_Lost = true;

		ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::dead;

		// stop sounds
		Singleton::GetInstance()->GetEffectManager()->StopAllSounds();
		Singleton::GetInstance()->GetSoundManager()->SetStop();
		return;
	}

	if (m_pStage != nullptr)
	{
		m_pStage->Update(elapsedSec, m_pIsaac, &m_Camera, m_MiniMap);
	}

	Singleton::GetInstance()->GetSoundManager()->Update();
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();

	if (m_pIsaac != nullptr)
	{
		m_Camera.Transform(m_pIsaac->GetShape()); // enkel transformen als de room clear is
	}

	if (m_pStage != nullptr)
	{
		m_pStage->Draw(m_pIsaac);
	}

	glPopMatrix();

	// draw HUD
	if (m_pIsaac != nullptr)
	{
		m_pIsaac->GetHealthBar().Draw(Point2f{ 40.f, m_Window.height - 70.f });

		ChargeBar * pChargeBar{ m_pIsaac->GetChargeBar() };
		if ( pChargeBar != nullptr)
		{
			m_pIsaac->GetChargeBar()->DrawHUD(Point2f{ 0.f , m_Window.height - 170.f });
		}
	}

	BossRoom* pBossRoom = m_pStage->GetBossRoom();
	if (pBossRoom != nullptr)
	{
		pBossRoom->GetHealthBar().DrawBoss(Point2f{ m_Window.width / 2 - (75.f * m_Scale), 40.f });
	}

	m_MiniMap.Draw(Point2f{ m_Window.width - 150.f, m_Window.height - 150.f });
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_i:
		Singleton::GetInstance()->GetSoundManager()->SetPause();
		Singleton::GetInstance()->GetEffectManager()->SetPause();
		ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::controls;
		break;
	case SDLK_ESCAPE:
		ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::menu;
		Singleton::GetInstance()->GetSoundManager()->SetPause();
		Singleton::GetInstance()->GetEffectManager()->SetPause();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}


bool Game::GetIsFinished() const
{
	return m_Lost;
}

// --> OPERATOR OVERLOADING

std::ostream& operator<<(std::ostream& out, const Game& game)
{
	out << "<Game\n";

	// leaderboard things

	// stage
	out << *game.m_pStage;

	// character
	out << *game.m_pIsaac;

	// powerup manager
	out << *Singleton::GetInstance()->GetPowerUpManager();

	out << "/Game>\n";

	return out;
}