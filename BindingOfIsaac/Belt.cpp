#include "pch.h"
#include "Belt.h"
#include "Singleton.h"
#include "Texture.h"
#include "utils.h"

Belt::Belt(const Point2f& pos, float scale) // SPEED UP
	: PowerUp{ pos, scale }
{
	m_Offset = Offset2f{ 5.f, 6.f };

	m_FileName = "Resources/Images/characters/costumes/costume_015_belt.png";

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_028_thebelt.png");

	Initialise();

	m_Type = TypePowerUp::belt;
}

Belt::Belt(std::stringstream & info, float scale)
	: PowerUp{ info, scale }
{
	m_Offset = Offset2f{ 5.f, 6.f };

	m_FileName = "Resources/Images/characters/costumes/costume_015_belt.png";

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_028_thebelt.png");

	Initialise();

	m_Type = TypePowerUp::belt;
}

void Belt::Draw() const
{
	Rectf srcRect{0.f, m_ClipHeight, m_ClipWidth, m_ClipHeight};
	Rectf destRect{ m_Shape.left - m_Offset.x * m_Scale, m_Shape.bottom - m_Offset.y * m_Scale, m_ClipWidth * m_Scale, m_ClipHeight * m_Scale };

	m_pTexture->Draw(destRect, srcRect);
}
PowerUpInfo Belt::IsOverlapping(const Rectf& actorShape)
{
	PowerUpInfo info{};

	if (m_IsHit)
		return info;

	if (utils::IsOverlapping(m_Shape, actorShape))
	{
		m_IsHit = true;
		info.fileName = m_FileName;
		info.type = TypePowerUp::belt;
	}

	return info;
}
