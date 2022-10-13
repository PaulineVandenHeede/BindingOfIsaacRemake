#include "pch.h"
#include "Screenflow.h"

#include "Singleton.h"

#include "StartScreen.h"
#include "OptionsScreen.h"
#include "ControlsScreen.h"
#include "Game.h"
#include "DeadScreen.h"
#include "EndScreen.h"

#include <string>
#include <iostream>
#include <fstream>

ScreenFlow::ScreenTypes ScreenFlow::m_CurrentScreen{ ScreenFlow::ScreenTypes::startup };
int ScreenFlow::m_Volume{ 20 };

ScreenFlow::ScreenFlow(const Window& window)
	: m_Window{ window }
	, m_pStartScreen{ new StartScreen{ 2.5f } }
	, m_pLnSScreen{ new LoadAndSaveScreen{ 2.5f } }
	, m_pDeadScreen{ new DeadScreen{ 2.5 } }
	, m_LoadingScreen{ }
	, m_pMainMenu{ new MenuScreen{ window.width, window.height } }
	, m_pOptionsScreen{ new OptionsScreen{} }
	, m_pControlsScreen{ new ControlsScreen{} }
	, m_pEndScreen{ new EndScreen{} }
	, m_pGame{ nullptr }
	, m_WhichFile{ false, File::none }
	, m_GameOption{ MenuScreen::Option::end } // so there's nothing happening when it starts
	, m_Loaded{ true }
	, m_FileLoaded{ false }
	, m_Save{ false }
{
	Initialize();
}

ScreenFlow::~ScreenFlow()
{
	Cleanup();
}

void ScreenFlow::Draw() const
{
	ClearBackground();

	switch (m_CurrentScreen)
	{
	case ScreenTypes::startup:
		if (m_pStartScreen == nullptr)
			return;
		m_pStartScreen->Draw();
		break;
	case ScreenTypes::loadSave:
		if (m_pLnSScreen == nullptr)
			return;
		m_pLnSScreen->Draw();
		break;
	case ScreenTypes::loading:
		m_LoadingScreen.Draw();
		break;
	case ScreenTypes::saving:
		m_LoadingScreen.Draw();
		break;
	case ScreenTypes::menu:
		if (m_pMainMenu != nullptr)
			m_pMainMenu->Draw();
		break;
	case ScreenTypes::options:
		if(m_pOptionsScreen != nullptr)
			m_pOptionsScreen->Draw();
		break;
	case ScreenTypes::controls:
		if (m_pControlsScreen != nullptr)
			m_pControlsScreen->Draw();
		break;
	case ScreenTypes::game:
		if (m_pGame != nullptr)
			m_pGame->Draw();
		break;
	case ScreenTypes::dead:
		if (m_pDeadScreen != nullptr)
		{
			if (m_pGame != nullptr)
				m_pGame->Draw();

			m_pDeadScreen->Draw();
		}
		break;
	case ScreenTypes::won:
		if (m_pEndScreen != nullptr)
			m_pEndScreen->Draw();
	default:
		break;
	}
}
void ScreenFlow::Update(const float& elapsedSec)
{
	switch (m_CurrentScreen)
	{
	case ScreenTypes::loading:
		UpdateLoading();
		break;
	case ScreenTypes::saving:
		UpdateSaving();
		break;
	case ScreenTypes::game:
		m_pGame->Update(elapsedSec);
		break;
	default:
		break;
	}
}

// Event handling
void ScreenFlow::ProcessKeyDownEvent(const SDL_KeyboardEvent & e)
{
	switch (m_CurrentScreen)
	{
	case ScreenTypes::game:
		m_pGame->ProcessKeyDownEvent(e);
		break;
	default:
		break;
	}
}
void ScreenFlow::ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& quit)
{
	switch (e.keysym.sym)
	{
	case SDLK_ESCAPE:
		ProcessEscape(quit);
		break;
	}

	switch (m_CurrentScreen)
	{
	case ScreenTypes::startup:
		if(m_pStartScreen != nullptr)
			m_pStartScreen->ProcessKeyUpEvent(e);
		break;
	case ScreenTypes::loadSave:
		if (m_pLnSScreen != nullptr)
		{
			m_WhichFile = m_pLnSScreen->ProcessKeyUpEvent(e);
			if (m_WhichFile.second != File::end)
			{
				m_FileLoaded = false;
			}
		}
		break;
	case ScreenTypes::menu:
		if(m_pMainMenu != nullptr)
			m_GameOption = m_pMainMenu->ProcessKeyUpEvent(e);
		break;
	case ScreenTypes::options:
		if(m_pOptionsScreen != nullptr)
			m_pOptionsScreen->ProcessKeyUpEvent(e);
		break;
	case ScreenTypes::controls:
		if(m_pControlsScreen != nullptr)
			m_pControlsScreen->ProcessKeyUpEvent(e);
		break;
	case ScreenTypes::game:
		if(m_pGame != nullptr)
			m_pGame->ProcessKeyUpEvent(e);
		break;
	case ScreenTypes::dead:
		if(m_pDeadScreen != nullptr)
			m_pDeadScreen->ProcessKeyUpEvent(e);
	case ScreenTypes::won:
		if (m_pEndScreen != nullptr)
			m_pEndScreen->ProcessKeyUpEvent(e);
		break;
	default:
		break;
	}
}
void ScreenFlow::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	switch (m_CurrentScreen)
	{
	case ScreenTypes::game:
		m_pGame->ProcessMouseMotionEvent(e);
		break;
	default:
		break;
	}
}
void ScreenFlow::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	switch (m_CurrentScreen)
	{
	case ScreenTypes::game:
		m_pGame->ProcessMouseDownEvent(e);
		break;
	default:
		break;
	}
}
void ScreenFlow::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	Point2f mousePos{ float(e.x), m_Window.height - (float)e.y };

	switch (m_CurrentScreen)
	{
	case ScreenTypes::game:
		m_pGame->ProcessMouseUpEvent(e);
		break;
	default:
		break;
	}
}

int ScreenFlow::GetVolume()
{
	return m_Volume;
}

void ScreenFlow::SetVolume(int value)
{
	m_Volume = value;
}

// --> HELPER FUNCITONS
void ScreenFlow::Initialize()
{
	switch (m_CurrentScreen)
	{
	case ScreenFlow::ScreenTypes::startup:
		break;
	case ScreenFlow::ScreenTypes::loadSave:
		break;
	case ScreenFlow::ScreenTypes::loading:
		break;
	case ScreenFlow::ScreenTypes::menu:
		break;
	case ScreenFlow::ScreenTypes::game:
		m_pGame = new Game{ m_Window };
		break;
	default:
		break;
	}
}
void ScreenFlow::Cleanup()
{
	delete m_pStartScreen;
	m_pStartScreen = nullptr;

	delete m_pLnSScreen;
	m_pLnSScreen = nullptr;

	delete m_pDeadScreen;
	m_pDeadScreen = nullptr;

	delete m_pGame;
	m_pGame = nullptr;

	delete m_pMainMenu;
	m_pMainMenu = nullptr;

	delete m_pControlsScreen;
	m_pControlsScreen = nullptr;

	delete m_pOptionsScreen;
	m_pOptionsScreen = nullptr;

	delete m_pEndScreen;
	m_pEndScreen = nullptr;

	delete Singleton::GetInstance();
}
void ScreenFlow::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ScreenFlow::ContinueGame() // continue run
{
	if (m_pGame == nullptr || m_pGame->GetIsFinished())
	{
		CreateNewGame();
	}

	//save to file
	SaveToFile("Resources/GameFiles/File_" + std::to_string(int(m_WhichFile.second)) + ".txt");

	m_CurrentScreen = ScreenTypes::game;
	Singleton::GetInstance()->GetSoundManager()->SetResume();
	Singleton::GetInstance()->GetEffectManager()->SetResume();
}
void ScreenFlow::CreateNewGame() // new run
{
	if (m_pGame != nullptr)
	{
		delete m_pGame;
		m_pGame = nullptr;
	}

	m_pGame = new Game{ m_Window };

	// save to file
	SaveToFile("Resources/GameFiles/File_" + std::to_string(int(m_WhichFile.second)) + ".txt");

	m_CurrentScreen = ScreenTypes::game;
	Singleton::GetInstance()->GetSoundManager()->SetResume();
	Singleton::GetInstance()->GetEffectManager()->SetResume();
}
void ScreenFlow::ResetGame() // rerun
{
	m_CurrentScreen = ScreenTypes::game;
	Singleton::GetInstance()->GetSoundManager()->SetResume();
	Singleton::GetInstance()->GetEffectManager()->SetResume();

	// get things from file
	LoadFile("Resources/GameFiles/File_" + std::to_string(int(m_WhichFile.second)) + ".txt", true);

	// save to file
	SaveToFile("Resources/GameFiles/File_" + std::to_string(int(m_WhichFile.second)) + ".txt");
}

void ScreenFlow::LoadFile(const std::string & fileName, bool rerun)
{
	std::ifstream file{ fileName };

	if (!file)
	{
		std::cerr << "File with name " + fileName + " not loaded correctly\n";
		return;
	}

	// get first line from file
	std::string gameInfo{};
	std::getline(file, gameInfo);

	// be sure that there are no memory leaks, and no game already running
	if (m_pGame != nullptr)
	{
		delete m_pGame;
		m_pGame = nullptr;
	}

	// see if there's already a game stored
	if (gameInfo == "")
	{
		// create new game
		m_pGame = new Game{ m_Window };

		// go to menu
		m_CurrentScreen = ScreenTypes::menu;
	}
	else
	{
	// load properties game from file

		 // get length of file:
		file.seekg(0, file.end);
		size_t length = size_t(file.tellg());
		file.seekg(0, file.beg);

		// adapt string capacity to length of file
		std::string info(length, '\0');

		// read file
		file.read(&info[0], length);

		// get part
		info = utils::GetPartValue("Game", info);

		std::stringstream game{ info };
		// construct game
		m_pGame = new Game{ game, m_Window, rerun };

		// set screen to menu
		m_CurrentScreen = ScreenTypes::menu;
		Singleton::GetInstance()->GetSoundManager()->SetStop();
		Singleton::GetInstance()->GetEffectManager()->StopAllSounds();
	}

	file.close();
}
void ScreenFlow::SaveToFile(const std::string & fileName)
{
	std::ofstream file{ fileName };

	if (!file)
	{
		std::cerr << "File with name " + fileName + " not loaded correctly\n";
		return;
	}

	// save game into file
	file << *m_pGame;
}
void ScreenFlow::ClearFile(const std::string & fileName)
{
	std::cout << "Cleared file with name: " + fileName + "\n";
	std::ofstream file{ fileName };

	if (!file)
	{
		std::cerr << "File not found.\n"; // throw exception
		return;
	}
	file << "";
}

void ScreenFlow::UpdateLoading()
{
	if (m_Loaded)
	{
		m_Loaded = false;
		return;
	}
	else
	{
		// load selected file
		if (!m_FileLoaded)
		{
			if (m_WhichFile.first)
			{
				LoadFile("Resources/GameFiles/File_" + std::to_string(int(m_WhichFile.second)) + ".txt", false);
			}
			else
			{
				ClearFile("Resources/GameFiles/File_" + std::to_string(int(m_WhichFile.second)) + ".txt");
			}
			m_FileLoaded = true;
		}
		m_CurrentScreen = ScreenTypes::menu;
		m_Loaded = true;
	}

	// load new run, rerun or continue run
	switch (m_GameOption)
	{
	case MenuScreen::Option::newRun:
		CreateNewGame();
		m_GameOption = MenuScreen::Option::end;
		m_CurrentScreen = ScreenTypes::game;
		break;
	case MenuScreen::Option::rerun:
		ResetGame();
		m_GameOption = MenuScreen::Option::end;
		m_CurrentScreen = ScreenTypes::game;
		break;
	case MenuScreen::Option::continueRun:
		ContinueGame();
		m_GameOption = MenuScreen::Option::end;
		m_CurrentScreen = ScreenTypes::game;
		break;
	default: // if end then it doesn't do anything
		break;
	}
}
void ScreenFlow::UpdateSaving()
{
	if (m_Save)
	{
		SaveToFile("Resources/GameFiles/File_" + std::to_string(int(m_WhichFile.second)) + ".txt");
		m_Save = false;
		m_CurrentScreen = ScreenTypes::loadSave;
	}
	else
	{
		m_Save = true;
	}
}

void ScreenFlow::ProcessEscape(bool& quit)
{
	if (m_CurrentScreen == ScreenTypes::game)
	{
		m_CurrentScreen = ScreenTypes::menu;
		Singleton::GetInstance()->GetSoundManager()->SetStop();
		Singleton::GetInstance()->GetEffectManager()->SetStop();
	}
	else 
	{
		m_CurrentScreen = ScreenTypes(int(m_CurrentScreen) - 1);
	}


	if (int(m_CurrentScreen) < 0)
	{
		m_CurrentScreen = ScreenTypes(0);
		quit = true; // quit game
	}
	if (m_CurrentScreen == ScreenTypes::loadSave)
	{
		m_CurrentScreen = ScreenTypes::saving;
	}
}
