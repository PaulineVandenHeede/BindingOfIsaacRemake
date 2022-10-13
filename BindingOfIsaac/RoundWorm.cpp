#include "pch.h"
#include "RoundWorm.h"
#include "Texture.h"
#include "Sprite.h"
#include "Singleton.h"
#include "InterpolatedTear.h"
#include "Tear.h"
#include "Room.h"
#include "utils.h"
#include "Character.h"
#include "ScreenFlow.h"
#include <iostream>

RoundWorm::RoundWorm(const Point2f& pos, float scale, int maxHealth, int health, int damage)
	: Monster{ pos, scale, maxHealth, health, damage }
	, m_TearManager{ TearManager::Type::blood, 100, 300.f, 1 }
	, m_pSprite{ nullptr }
	, m_pTexture{ nullptr }
	, m_pTearTexture{ nullptr }
	, m_ScaleTear{ 1.75f }
	, m_GapX{ 8.f }
	, m_GapY{ 4.f }
	, m_NewPosition{ false }
	, m_CreateTear{ true }
{
	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/monsters/monster_199_parabite_dirt.png");
	m_pTearTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/effects/effect_003_bloodtear.png");

	m_pSprite = new Sprite{ m_pTexture, 3, 4, 11, m_ClipWidth, m_ClipHeight, 0.f, m_ClipHeight, 5 };

	m_Shape.left += m_GapX * m_Scale;
	m_Shape.bottom += m_GapY * m_Scale;
	m_Shape.width -= m_GapX * m_Scale * 2;
	m_Shape.height -= m_GapY * m_Scale;

}
RoundWorm::~RoundWorm()
{
	delete m_pSprite;
	m_pSprite = nullptr;

	m_pTearTexture = nullptr;
	m_pTexture = nullptr;
}

void RoundWorm::Draw() const
{
	Point2f pos{ m_Shape.left - m_GapX * m_Scale, m_Shape.bottom - m_GapY * m_Scale };
	m_pSprite->Draw(pos, m_Scale, m_Scale);

	m_TearManager.Draw();
}
void RoundWorm::Update(float elapsedSec, Character* pActor, const Room* pRoom)
{
	m_pSprite->Update(elapsedSec);
	if (m_pSprite->GetActFrame() == (m_pSprite->GetNrOfFrames() - 1))
	{
		if (m_NewPosition)
		{
			Point2f pos{ RandomPosition(pRoom->GetShape()) };
			m_Shape.left = pos.x;
			m_Shape.bottom = pos.y;
			m_NewPosition = false;
			m_CreateTear = true;
		}
	}
	if (m_pSprite->GetActFrame() == (m_pSprite->GetNrOfFrames() / 2 + 1))
	{
		if (m_CreateTear)
		{
			Rectf shape{ pActor->GetShape() };

			Point2f beginPos{ m_Shape.left + m_Shape.width / 2 , m_Shape.bottom + m_Shape.height / 2 };
			Point2f endPos{ shape.left + shape.width / 2, shape.bottom + shape.height / 2 };

			m_TearManager.CreateInterpolatedTear( beginPos, endPos, m_Damage);

			m_NewPosition = true;
			m_CreateTear = false;
		}
	}

	m_TearManager.Update(elapsedSec, pRoom);

	// check collision actor - roundworm
	if (RoundWorm::TouchedByCharacter(pActor->GetShape()))
	{
		pActor->HitByObject(m_Damage);
		std::cout << "Touched by roundworm\n";
	}

	// check collision actor - tears
	m_TearManager.HitObject(pActor);

	// check collision tears actor - roundworm
	pActor->ObjectHit(this);
}

void RoundWorm::HitByTears(int damage)
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
		
		// sound
		fileName = "Resources/Sounds/monsters/meaty deaths 0.wav";
		Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);

	}
}
bool RoundWorm::ReadyToDelete() const
{
	return m_IsHit;
}


// --> HELPER FUNCTIONS
Point2f RoundWorm::RandomPosition(const Rectf& roomShape)
{
	int width = int (roomShape.width - (m_ClipWidth * m_Scale) * 2); // minus the half of the width of the texture
	int height = int (roomShape.height - (m_ClipHeight * m_Scale) * 2);

	Point2f pos{ };
	pos.x = float(rand() % width) + roomShape.left + (m_ClipWidth * m_Scale);
	pos.y = float(rand() % height) + roomShape.bottom + (m_ClipHeight * m_Scale);

	return pos;
}
bool RoundWorm::TouchedByCharacter(const Rectf& actorShape)
{
	if (m_pSprite->GetActFrame() <= 1 || m_pSprite->GetActFrame() >= (m_pSprite->GetNrOfFrames() - 2)) // when rising and sinking it can do no harm
		return false;
	return Monster::TouchedByCharacter(actorShape);
}
