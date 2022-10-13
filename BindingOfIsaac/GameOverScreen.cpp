#include "pch.h"
#include "GameOverScreen.h"
#include "Texture.h"
#include "Singleton.h"

GameOverScreen::GameOverScreen(const Rectf& destRect)
	: m_DestRect{ destRect }
	, m_pControlsMenu{ nullptr }
{
	m_pControlsMenu = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/died_menu.png");

	m_ScaleY = m_DestRect.height / m_pControlsMenu->GetHeight();
	m_ScaleX = m_ScaleY;
}


GameOverScreen::~GameOverScreen()
{
	m_pControlsMenu = nullptr;
}

void GameOverScreen::Draw(const Rectf& rect) const
{
	utils::SetColor(Color4f{ 0.0f,0.f,0.f,0.5f });
	utils::FillRect(rect);

	Rectf newDestRect{};
	newDestRect.height = m_pControlsMenu->GetHeight() * m_ScaleY;
	newDestRect.width = m_pControlsMenu->GetWidth() * m_ScaleX;
	newDestRect.left = rect.left + rect.width / 2 - newDestRect.width / 2;
	newDestRect.bottom = rect.bottom;

	m_pControlsMenu->Draw(newDestRect);
}