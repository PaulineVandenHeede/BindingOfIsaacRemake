#include "pch.h"
#include "Info.h"
#include "Texture.h"

Info::Info(const Point2f& pos)
	: m_Position{pos}
	, m_Instructions{ "W,A,S,D to move.\n Arrows to shoot\n" }
	, m_NameGame{ "The Binding of Isaac: Rebirth" }
	, m_Name{ "1DAE21 - Vanden Heede, Pauline" }
{
	m_pTextureInstructions = new Texture{ m_Instructions, "Resources/DIN-Light.otf", 40, Color4f{ 1.f,1.f,1.f,1.f } };
	m_pTextureNameGame = new Texture{ m_NameGame, "Resources/DIN-Light.otf", 34, Color4f{ 1.f,1.f,1.f,1.f } };
	m_pTextureName = new Texture{ m_Name, "Resources/DIN-Light.otf", 32, Color4f{ 1.f,1.f,1.f,1.f } };
}


Info::~Info()
{
	delete m_pTextureInstructions;
	m_pTextureInstructions = nullptr;

	delete m_pTextureName;
	m_pTextureName = nullptr;

	delete m_pTextureNameGame;
	m_pTextureNameGame = nullptr;
}

void Info::Draw() const
{
	float gap{ 30.f };

	m_pTextureName->Draw(m_Position);
	m_pTextureInstructions->Draw(Point2f{ m_Position.x, m_Position.y + 2 * gap });
	m_pTextureNameGame->Draw(Point2f{ m_Position.x, m_Position.y + 5 * gap });
}