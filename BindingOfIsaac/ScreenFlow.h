#pragma once
#include "LoadAndSaveScreen.h"
#include "LoadingScreen.h"
#include "MenuScreen.h"

class StartScreen;
class OptionsScreen;
class ControlsScreen;
class Game;
class DeadScreen;
class EndScreen;


class ScreenFlow final
{
public:
	enum class ScreenTypes
	{
		startup = 0,
		loadSave = 1,
		loading = 2,
		saving = 3,
		menu = 4,
		options = 5,
		controls = 6,
		game = 7,
		dead = 8,
		won = 9
	};

	explicit ScreenFlow(const Window& window);
	~ScreenFlow();

	// rule of 5
	ScreenFlow(const ScreenFlow& other) = delete;
	ScreenFlow& operator=(const ScreenFlow& other) = delete;
	ScreenFlow(ScreenFlow&& other) = delete;
	ScreenFlow& operator=(ScreenFlow&& other) = delete;

	void Draw() const;
	void Update(const float& elapsedSec);

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e, bool& quit);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

	static int GetVolume();
	static void SetVolume(int value);

	static ScreenTypes m_CurrentScreen;
private:
	// --> MEMBER VARIABLES
	const Window m_Window;

	StartScreen* m_pStartScreen;
	LoadAndSaveScreen* m_pLnSScreen;
	LoadingScreen m_LoadingScreen;
	DeadScreen* m_pDeadScreen;
	MenuScreen* m_pMainMenu;
	OptionsScreen* m_pOptionsScreen;
	ControlsScreen* m_pControlsScreen;
	EndScreen* m_pEndScreen;
	Game* m_pGame;

	std::pair<bool, File> m_WhichFile;
	MenuScreen::Option m_GameOption;

	static int m_Volume;

	bool m_Loaded;
	bool m_FileLoaded;
	bool m_Save;

	// --> HELPER FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;

	void ContinueGame(); // continue run
	void CreateNewGame(); // new run
	void ResetGame(); // rerun

	void LoadFile(const std::string& fileName, bool rerun);
	void SaveToFile(const std::string& fileName);
	void ClearFile(const std::string& fileName);

	void UpdateLoading();
	void UpdateSaving();

	void ProcessEscape(bool& quit);
};
