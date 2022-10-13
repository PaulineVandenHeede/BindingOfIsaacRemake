#include "pch.h"
#include "Lemon.h"
#include "Singleton.h"
#include "Texture.h"

Lemon::Lemon(const Point2f& pos, float scale)
	: PowerUp{ pos, scale }
{
	m_Offset = Offset2f{8.f, 8.f};

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_056_lemonmishap.png");

	Initialise();

	m_Type = TypePowerUp::lemon;
}

Lemon::Lemon(std::stringstream & info, float scale)
	: PowerUp{ info, scale }
{
	m_Offset = Offset2f{ 8.f, 8.f };

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_056_lemonmishap.png");

	Initialise();

	m_Type = TypePowerUp::lemon;
}

void Lemon::Draw() const
{
	Rectf srcRect{ 0.f, m_ClipHeight, m_ClipWidth, m_ClipHeight };
	Rectf destRect{ m_Shape.left - m_Offset.x * m_Scale, m_Shape.bottom - m_Offset.y * m_Scale, m_ClipWidth * m_Scale, m_ClipHeight * m_Scale };

	m_pTexture->Draw(destRect, srcRect);
}

PowerUpInfo Lemon::IsOverlapping(const Rectf & actorShape)
{
	PowerUpInfo info{};

	if (m_IsHit)
		return info;

	if (utils::IsOverlapping(m_Shape, actorShape))
	{
		m_IsHit = true;
		info.fileName = m_FileName;
		info.type = TypePowerUp::lemon;
	}

	return info;
}
