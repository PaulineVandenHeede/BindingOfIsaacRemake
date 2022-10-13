#include "pch.h"
#include "AttackFly.h"
#include "Sprite.h"
#include "Texture.h"
#include "Singleton.h"
#include "Room.h"
#include "Tear.h"
#include "Character.h"
#include "ScreenFlow.h"
#include <iostream>


AttackFly::AttackFly(const Point2f& pos, float scale, float speed, AttackFly::Type type, float radius, int maxHealth, int health, int damage)
	: Monster{pos, scale, maxHealth, health, damage}
	, m_pFly{ nullptr }
	, m_pTexture{ nullptr }
	, m_Velocity{ 0.f, 0.f }
	, m_ClipWidthDie{ 64.f }
	, m_ClipHeightDie{ 64.f }
	, m_Speed{ speed }
	, m_GapX{ 8.f }
	, m_GapY{ 12.f }
	, m_Type{ type }
	, m_Angle{ 0.f }
	, m_RotSpeed{ 180.f }
	, m_Radius{ radius }
{
	m_Shape.left += m_GapX * m_Scale;
	m_Shape.bottom += m_GapY * m_Scale;
	m_Shape.width -= (m_GapX + 6.f) * m_Scale;
	m_Shape.height -= (m_GapY + 5.f) * m_Scale;

	SetVelocity();

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/monsters/monster_010_fly.png");

	m_pFly = new Sprite{ m_pTexture, 2, 1, 2, m_ClipWidth, m_ClipHeight, m_ClipWidth, m_ClipHeight * 2, 10};
}

AttackFly::AttackFly(std::stringstream& info, float scale, AttackFly::Type type, float radius, int damage)
	: Monster{ info, scale, damage }
	, m_pFly{ nullptr }
	, m_pTexture{ nullptr }
	, m_Velocity{ 0.f, 0.f }
	, m_ClipWidthDie{ 64.f }
	, m_ClipHeightDie{ 64.f }
	, m_Speed{ 150.f }
	, m_GapX{ 8.f }
	, m_GapY{ 12.f }
	, m_Type{ type }
	, m_Angle{ 0.f }
	, m_RotSpeed{ 180.f }
	, m_Radius{ radius }
{
	m_Shape.left += m_GapX * m_Scale;
	m_Shape.bottom += m_GapY * m_Scale;
	m_Shape.width -= (m_GapX + 6.f) * m_Scale;
	m_Shape.height -= (m_GapY + 5.f) * m_Scale;

	SetVelocity();

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/monsters/monster_010_fly.png");

	m_pFly = new Sprite{ m_pTexture, 2, 1, 2, m_ClipWidth, m_ClipHeight, m_ClipWidth, m_ClipHeight * 2, 10 };
}

AttackFly::~AttackFly()
{
	std::cout << "AttackFly destructor calles\n";
	delete m_pFly;
	m_pFly = nullptr;


	m_pTexture = nullptr;
}

void AttackFly::Draw() const
{
	Point2f pos{ m_Shape.left - m_GapX * m_Scale, m_Shape.bottom - m_GapY * m_Scale };
	m_pFly->Draw(pos, m_Scale, m_Scale);
}
void AttackFly::Draw(const Point2f& centerPos) const // position is center point
{
	if (m_Type == AttackFly::Type::boss)
	{
		DrawBossFly(centerPos);
	}
	Point2f pos{ m_Shape.left - m_GapX * m_Scale, m_Shape.bottom - m_GapY * m_Scale };
	m_pFly->Draw(pos, m_Scale, m_Scale);
}
void AttackFly::Update(float elapsedSec, Character* pActor, const Room* room)
{
	SetVelocity(pActor->GetShape());

	// so that flies don't fly at exactly the same place after a time --> more buzzing feeling
	// x direction
	bool doGap{ bool (rand() % 2) };
	int gap{};
	if (doGap)
		gap = rand() % 10 - 5;
	m_Shape.left += m_Velocity.x * elapsedSec + gap;
	// y direction
	doGap = bool (rand() % 2);
	if (doGap)
		gap = rand() % 10 - 5;
	m_Shape.bottom += m_Velocity.y * elapsedSec + gap;

	// collision with room
	room->HandleCollision(m_Shape, m_Velocity);

	// update sprite
	m_pFly->Update(elapsedSec);

	// check collision actor - fly
	if (TouchedByCharacter(pActor->GetShape()))
	{
		pActor->HitByObject(m_Damage);
	}

	// check tear collision actor - fly
	pActor->ObjectHit(this);

	// sound
	std::string fileName{"Resources/Sounds/monsters/insect swarm.wav"};
	Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);

}

void AttackFly::HitByTears(int damage)
{
	m_HealthBar.DoDamage(damage);

	if (m_HealthBar.GetHealth() <= 0)
	{
		m_IsHit = true;

		std::string fileName{ "Resources/Images/effects/effect_032_bloodstains.png" };
		Point2f pos{ m_Shape.left - m_GapX * m_Scale, m_Shape.bottom - m_GapY * m_Scale };
		Offset2f offset{ 0.f, float(rand() % 6 + 1) * 32.f };
		Monster::AddEffect(fileName, pos, 48.f, 32.f, offset.x, offset.y);
				
		int i{};
		int max{ rand() % 3 };
		while (i < max)
		{
			fileName = "Resources/Images/effects/effect_030_bloodgibs.png";
			pos = Point2f{ m_Shape.left - m_GapX * m_Scale, m_Shape.bottom - m_GapY * m_Scale };

			// x direction
			bool doGap{ bool(rand() % 2) };
			int gap{};
			if (doGap)
				gap = rand() % 10 - 5;
			pos.x += gap * m_Scale;
			// y direction
			doGap = bool(rand() % 2);
			if (doGap)
				gap = rand() % 10 - 5;
			pos.y += gap * m_Scale;

			offset = Offset2f{ float(rand() % 4) * 16.f, float(rand() % 3 + 1) * 16.f };
			Monster::AddEffect(fileName, pos, 16.f, 16.f, offset.x, offset.y);

			++i;
		}

		fileName = "Resources/Images/monsters/monster_010_fly.png";
		pos = Point2f{ m_Shape.left - 21.f * m_Scale, m_Shape.bottom - 28.f * m_Scale };
		Monster::AddEffect(fileName, pos, m_ClipWidthDie, m_ClipHeightDie, 0.f, 2 * m_ClipHeightDie, true, 4, 3, 12);

		// sound
		fileName =  "Resources/Sounds/monsters/meaty deaths 0.wav";
		Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);

	}
}

bool AttackFly::ReadyToDelete() const
{
	return m_IsHit;
}

void AttackFly::SetType(AttackFly::Type type)
{
	m_Type = type;
}

// --> HELPER FUNCTIONS
void AttackFly::SetVelocity()
{
	Direction direction{ Direction(rand() % 4) };
	switch (direction)
	{
	case Direction::up:
		m_Velocity.y = m_Speed;
		break;
	case Direction::down:
		m_Velocity.y = -m_Speed;
		break;
	case Direction::right:
		m_Velocity.x = m_Speed;
		break;
	case Direction::left:
		m_Velocity.x = -m_Speed;
		break;
	}
}
void AttackFly::SetVelocity(const Rectf& actorShape)
{
	if (actorShape.bottom > m_Shape.bottom)
	{
		m_Velocity.y = m_Speed;
		if (actorShape.left > m_Shape.left)
			m_Velocity.x = m_Speed;
		else
			m_Velocity.x = -m_Speed;
	}
	else
	{
		m_Velocity.y = -m_Speed;
		if (actorShape.left > m_Shape.left)
			m_Velocity.x = m_Speed;
		else
			m_Velocity.x = -m_Speed;
	}
}

void AttackFly::DrawBossFly(const Point2f& centerPos) const
{
	Point2f drawPos{ m_Shape.left - m_GapX * m_Scale, m_Shape.bottom - m_GapY * m_Scale };

	m_pFly->Draw(drawPos, m_Scale, m_Scale);
}