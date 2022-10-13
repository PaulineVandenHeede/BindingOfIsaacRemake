#pragma once
#include "GameOverScreen.h"
#include "Room.h"
#include "MenuScreen.h"
#include "Camera.h"
#include "SoundManager.h"
#include "MiniMap.h"

class Stage;
class BossRoom;
class Character;
class EffectManager;

class Game
{
public:
	explicit Game( const Window& window );
	explicit Game(std::stringstream& info, const Window& window, bool rerun);
	~Game();

	// rule of 5
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

	bool GetIsFinished() const;

	friend std::ostream& operator<<(std::ostream& out, const Game& game);
private:
	// DATA MEMBERS
	const Window m_Window;

	EffectManager* m_pEffectManager;

	Stage* m_pStage;
	Character* m_pIsaac;

	Camera m_Camera;
	MiniMap m_MiniMap;
	
	float m_Scale;
	float m_ScaleBoss;

	unsigned int m_NrOfTimesDeath;

	bool m_Lost;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
};
