#include "pch.h"
#include "Tear.h"
#include "Sprite.h"
#include "Singleton.h"

Tear::Tear(const Point2f& position, Texture * pTexture, TearManager::Type type, float scale, int damage)
	: m_pTexture{ pTexture }
	, m_pTearSprite{ nullptr }
	, m_BeginPos{ position }
	, m_ClipSize{ 64.f }
	, m_Angle{ 0.f }
	, m_Scale{ scale }
	, m_Type{ type }
	, m_RotatingSpeed{ 300 }
	, m_Damage{ damage }
	, m_IsHit{ false }
{
	m_Shape.center = position;
	m_Shape.radius = 15.f * m_Scale;

	m_pTearSprite = new Sprite{ m_pTexture, 4, 4, 16, m_ClipSize, m_ClipSize, 0.f, m_ClipSize, 10 };
}

Tear::~Tear()
{
	delete m_pTearSprite;
	m_pTearSprite = nullptr;

	m_pTexture = nullptr;
}

bool Tear::IsOverlapping(const Rectf & actorShape) const
{
	return utils::IsOverlapping(actorShape, m_Shape);
}

void Tear::HitSomething()
{
	m_IsHit = true;
}

bool Tear::ReadyToDelete() const
{
	return m_IsHit;
}

Circlef Tear::GetShape() const
{
	return m_Shape;
}

int Tear::GetDamage() const
{
	return m_Damage;
}

void Tear::AddEffect() const
{
	std::string fileName{ "Resources/Images/effects/effect_" };
	switch (m_Type)
	{
	case TearManager::Type::water:
		fileName += "015_tearpoofa.png";
		break;
	case TearManager::Type::blood:
		fileName += "003_bloodtear.png";
		break;
	default:
		break;
	}

	Point2f pos{ m_Shape.center.x - (m_ClipSize * m_Scale) / 2, m_Shape.center.y - (m_ClipSize * m_Scale) / 2 };
	Singleton::GetInstance()->GetEffectManager()->AddEffect(fileName, pos, m_ClipSize, m_ClipSize, 0, m_ClipSize, m_Scale, true, 4, 4, 16);
}
