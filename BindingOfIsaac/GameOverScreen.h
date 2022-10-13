#pragma once

class Texture;

class GameOverScreen
{
public:
	GameOverScreen(const Rectf& destRect);
	~GameOverScreen();

	// rule of 5
	GameOverScreen(const GameOverScreen& other) = delete;
	GameOverScreen(GameOverScreen&& other) = default;
	GameOverScreen& operator=(const GameOverScreen& other) = delete;
	GameOverScreen& operator=(GameOverScreen&& other) = default;

	void Draw(const Rectf& rect) const;

private:
	Texture* m_pControlsMenu;

	Rectf m_DestRect;
	float m_ScaleX;
	float m_ScaleY;
};