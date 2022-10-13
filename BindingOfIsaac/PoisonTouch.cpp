#include "pch.h"
#include "PoisonTouch.h"
#include "Singleton.h"
#include "Texture.h"
#include "utils.h"


PoisonTouch::PoisonTouch(const Point2f& pos, float scale) // DAMAGE TEARS UP, SPEED DOWN
	: PowerUp{ pos, scale }
{
	m_FileName = "Resources/Images/characters/costumes/costume_071_virus.png";

	m_Offset = Offset2f{ 10.f, 4.f };

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_013_thevirus.png");

	Initialise();

	m_Type = TypePowerUp::poison;
}
PoisonTouch::PoisonTouch(std::stringstream & info, float scale)
	: PowerUp{ info, scale }
{
	m_FileName = "Resources/Images/characters/costumes/costume_071_virus.png";

	m_Offset = Offset2f{ 10.f, 4.f };

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_013_thevirus.png");

	Initialise();

	m_Type = TypePowerUp::poison;
}

void PoisonTouch::Draw() const
{
	Rectf srcRect{ 0.f, m_ClipHeight, m_ClipWidth, m_ClipHeight };
	Rectf destRect{ m_Shape.left - m_Offset.x * m_Scale, m_Shape.bottom - m_Offset.y * m_Scale, m_ClipWidth * m_Scale, m_ClipHeight * m_Scale };

	m_pTexture->Draw(destRect, srcRect);
}
PowerUpInfo PoisonTouch::IsOverlapping(const Rectf& actorShape)
{
	PowerUpInfo info{};

	if (m_IsHit)
		return info;

	if (utils::IsOverlapping(m_Shape, actorShape))
	{
		m_IsHit = true;
		info.fileName = m_FileName;
		info.type = TypePowerUp::poison;
	}

	return info;
}
