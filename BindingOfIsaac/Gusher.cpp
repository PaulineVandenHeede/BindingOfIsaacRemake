#include "pch.h"
#include "Gusher.h"
#include "Texture.h"
#include "Singleton.h"
#include "Room.h"
#include "DoubleSprite.h"
#include "Sprite.h"
#include "utils.h"
#include "Tear.h"
#include "ScreenFlow.h"
#include "Character.h"
#include <iostream>


Gusher::Gusher(const Point2f& pos, float scale, int maxHealth, int health, int damage)
	: Monster{ pos, scale, maxHealth, health, damage }
	, m_TearManager{ TearManager::Type::blood, 100, 300.f, 1 }
	, m_pSpriteMonster{ nullptr }
	, m_pBloodSpatters{ nullptr }
	, m_pTexture{ nullptr }
	, m_Velocity{ 0.f, 0.f }
	, m_MaxAnimTime{ 0.2f }
	, m_Speed{ 200.f }
	, m_AnimTime{ 2.f }
	, m_GapX{ 6.f }
	, m_GapY{ 6.f }
	, m_Direction{ Direction(rand() % 4) }
{
	m_Shape.left += m_GapX * m_Scale;
	m_Shape.bottom += m_GapY * m_Scale;
	m_Shape.width -= m_GapX * m_Scale * 2;
	m_Shape.height -= (m_GapY + 11.f) * m_Scale;

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/monsters/monster_000_bodies02.png");

	m_pSpriteMonster = new DoubleSprite{ m_pTexture, 4, 3, 10, 20, m_ClipWidth, m_ClipHeight, 0.f, m_ClipHeight};
	m_pBloodSpatters = new Sprite{ m_pTexture, 2, 4, 8, m_ClipWidth, m_ClipHeight, m_ClipWidth * 4, m_ClipHeight, 10 };

	SetVelocity(); // after texture and sprite is initialised
}

Gusher::~Gusher()
{
	delete m_pSpriteMonster;
	m_pSpriteMonster = nullptr;

	delete m_pBloodSpatters;
	m_pBloodSpatters = nullptr;

	m_pTexture = nullptr;
}

void Gusher::Draw() const
{
	Point2f pos{m_Shape.left - m_GapX * m_Scale, m_Shape.bottom - m_GapY * m_Scale};

	m_TearManager.Draw();

	switch (m_Direction)
	{
	case Direction::down:
	case Direction::right:
		m_pSpriteMonster->Draw(pos, m_Scale, m_Scale);
		break;
	case Direction::up:
	case Direction::left:
		DrawMirroredTexture(pos);
		break;
	}

	m_pBloodSpatters->Draw(pos, m_Scale, m_Scale);
}
void Gusher::Update(float elapsedSec, Character* pActor, const Room* pRoom)
{
	m_AnimTime -= elapsedSec;

	if (m_AnimTime < 0)
	{
		// need to do this with steering behaviour: wander
		m_Direction = Direction(rand() % 4);
		SetVelocity();
		m_AnimTime = m_MaxAnimTime;
	}

	m_pSpriteMonster->Update(elapsedSec);
	m_pBloodSpatters->Update(elapsedSec);

	// update position
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	int chance{ 300 };

	if ((rand() % chance) == 9) // 1/300 chance that it spawns bullets
	{
		m_TearManager.CreateTearsInFourDirection(Point2f{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 }, m_Damage);
	}

	m_TearManager.Update(elapsedSec, pRoom);

	// check if position is still in room boundaries
	pRoom->HandleCollisionMonster(m_Shape, m_Velocity);

	// check collision actor - gusher
	if (TouchedByCharacter(pActor->GetShape()))
	{
		pActor->HitByObject(m_Damage);
		std::cout << "Touched by gusher\n";
	}

	// check collision actor - tears
	m_TearManager.HitObject(pActor);

	// check collision tears actor - gusher
	pActor->ObjectHit(this);
}

void Gusher::HitByTears(int damage)
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

		fileName = "Resources/Images/monsters/monster_000_bodies02.png";
		pos = Point2f{ m_Shape.left - m_GapX * m_Scale, m_Shape.bottom - m_GapY * m_Scale };

		Monster::AddEffect(fileName, pos, m_ClipWidth, m_ClipHeight, m_ClipWidth * 4, m_ClipHeight, m_Scale, true, 2, 4, 8);

		// sound
		fileName = "Resources/Sounds/monsters/meaty deaths 0.wav";
		Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);

	}
}

bool Gusher::ReadyToDelete() const
{
	return (m_IsHit);
}

// --> HELPER FUNCTIONS

void Gusher::DrawMirroredTexture(const Point2f & pos) const
{
	Point2f newPos{ pos.x + (m_ClipWidth * m_Scale) / 2 , pos.y + (m_ClipHeight * m_Scale) };
	glPushMatrix();

	glTranslatef(newPos.x, newPos.y, 0.f);
	glScalef(-1.f, 1.f, 1.f);
	glTranslatef(-newPos.x, -newPos.y, 0.f);
	m_pSpriteMonster->Draw(pos, m_Scale, m_Scale);

	glPopMatrix();
}
void Gusher::SetVelocity()
{
	if (m_Direction == Direction::up || m_Direction == Direction::down) // going up or down
	{
		m_pSpriteMonster->SetActFrame(0);
		if (m_Direction == Direction::up)
			m_Velocity.y = m_Speed;
		else
			m_Velocity.y = -m_Speed;
		m_Velocity.x = 0.f;
	}
	else if (m_Direction == Direction::right || m_Direction == Direction::left) // going left or right
	{
		m_pSpriteMonster->SetActFrame(11);
		if (m_Direction == Direction::right)
			m_Velocity.x = m_Speed;
		else
			m_Velocity.x = -m_Speed;
		m_Velocity.y = 0.f;
	}
}
