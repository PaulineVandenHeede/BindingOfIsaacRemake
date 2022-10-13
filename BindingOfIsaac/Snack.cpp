#include "pch.h"
#include "Snack.h"
#include "Singleton.h"
#include "Texture.h"
#include "utils.h"

Snack::Snack(const Point2f& pos, float scale) // HPMAX UP
	: PowerUp{ pos, scale }
{
	m_Offset = Offset2f{ 5.f, 7.f };

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_346_asnack.png");
	
	Initialise();

	m_Type = TypePowerUp::snack;
}

Snack::Snack(std::stringstream & info, float scale)
	: PowerUp{ info, scale }
{
	m_Offset = Offset2f{ 5.f, 7.f };

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_346_asnack.png");

	Initialise();

	m_Type = TypePowerUp::snack;
}

void Snack::Draw() const
{
	Rectf srcRect{ 0.f, m_ClipHeight, m_ClipWidth, m_ClipHeight };
	Rectf destRect{ m_Shape.left - m_Offset.x * m_Scale, m_Shape.bottom - m_Offset.y * m_Scale, m_ClipWidth * m_Scale, m_ClipHeight * m_Scale };

	m_pTexture->Draw(destRect, srcRect);
}

PowerUpInfo Snack::IsOverlapping(const Rectf& actorShape)
{
	PowerUpInfo info{};

	if (m_IsHit)
		return info;

	if (utils::IsOverlapping(m_Shape, actorShape))
	{
		m_IsHit = true;
		info.fileName = m_FileName;
		info.type = TypePowerUp::snack;
	}

	return info;
}

