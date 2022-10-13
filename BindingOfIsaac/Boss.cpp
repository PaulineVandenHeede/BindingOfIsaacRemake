#include "pch.h"
#include "Boss.h"
#include "AttackFly.h"
#include "Singleton.h"
#include "RoomManager.h"
#include "Texture.h"
#include "Tear.h"
#include "utils.h"
#include "Character.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Boss::Boss(const Point2f& pos, float scale, int maxHealth, int health)
	: m_pMonsterManager{ nullptr  }
	, m_HealthBar{ maxHealth, health, scale, HealthBar::Type::boss }
	, m_ClipWidth{ 80.f }
	, m_ClipHeight{ 64.f }
	, m_Scale{ scale }
	, m_OffsetX{ 17.f }
	, m_OffsetY{ 9.f }
	, m_Velocity{ 0.f, 0.f}
	, m_Speed{ 100.f }
	, m_Radius{ 500.f }
	, m_AnimTime{ 0.f }
	, m_Damage{ 1 }
	, m_MaxAnimTime{ 2.f }
	, m_IsDead{ false }
{
	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/monsters/boss/boss_007_dukeofflies.png");

	m_Shape.left = pos.x - (m_ClipWidth * m_Scale) / 2 + m_OffsetX * m_Scale;
	m_Shape.bottom = pos.y - (m_ClipHeight * m_Scale) + m_OffsetY * m_Scale;
	m_Shape.width = (m_ClipWidth - m_OffsetX - 18.f) * m_Scale;
	m_Shape.height = (m_ClipHeight - m_OffsetY - 19.f) * m_Scale;

	m_pMonsterManager = new MonsterManager{ m_Scale, m_Shape, false };
}

Boss::Boss(std::stringstream& info, float scale)
	: m_pMonsterManager{ nullptr }
	, m_HealthBar{ 0, 0, scale, HealthBar::Type::boss }
	, m_ClipWidth{ 80.f }
	, m_ClipHeight{ 64.f }
	, m_Scale{ scale }
	, m_OffsetX{ 17.f }
	, m_OffsetY{ 9.f }
	, m_Velocity{ 0.f, 0.f }
	, m_Speed{ 100.f }
	, m_Radius{ 500.f }
	, m_AnimTime{ 0.f }
	, m_Damage{ 1 }
	, m_MaxAnimTime{ 2.f }
	, m_IsDead{ false }
{
	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/monsters/boss/boss_007_dukeofflies.png");

	// pos
	std::string part{ utils::GetAttributeValue("Position", info.str()) };
	Point2f pos{};
	if (part != "")
		pos = utils::StringToPos(part);

	m_Shape.left = pos.x - (m_ClipWidth * m_Scale) / 2 + m_OffsetX * m_Scale;
	m_Shape.bottom = pos.y - (m_ClipHeight * m_Scale) + m_OffsetY * m_Scale;
	m_Shape.width = (m_ClipWidth - m_OffsetX - 18.f) * m_Scale;
	m_Shape.height = (m_ClipHeight - m_OffsetY - 19.f) * m_Scale;

	// health
	part = utils::GetAttributeValue("HealthBar", info.str());
	std::pair<int, int> healthBar{};
	if (part != "")
		healthBar = utils::StringToHealthBar(part);

	m_HealthBar.IncreaseMaxHealth(healthBar.second);
	m_HealthBar.DoHealing(healthBar.first);

	m_IsDead = (m_HealthBar.GetHealth() == 0);

	// monstermanager: flies
	std::stringstream mm{};
	mm << utils::GetPartValue("MonsterManager", info.str());

	if (mm.str() != "")
		m_pMonsterManager = new MonsterManager{ mm, m_Scale, m_Shape };
}

Boss::~Boss()
{
	m_pTexture = nullptr;

	delete m_pMonsterManager;
	m_pMonsterManager = nullptr;
}

void Boss::Draw() const
{
	Rectf srcRect{determineSrcRect()};
	Rectf destRect{m_Shape.left - m_OffsetX * m_Scale, m_Shape.bottom - m_OffsetY * m_Scale, m_ClipWidth * m_Scale, m_ClipHeight * m_Scale};
	
	m_pTexture->Draw(destRect,srcRect);

	m_pMonsterManager->Draw();
}

void Boss ::Update(float elapsedSec, Character* pActor, const Room* pRoom)
{
	m_AnimTime += elapsedSec;

	const Uint8 *pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_C])
	{
		m_pMonsterManager->DeleteAll();
	}

	if (m_AnimTime > (m_MaxAnimTime / 2))
	{
		if (m_ActionState == ActionState::inhaling)
		{
			m_ActionState = ActionState::blowing;
		}
	}
	if (m_AnimTime > m_MaxAnimTime)
	{
		int i{ rand() % 10 };
		switch (i)
		{
		case 0:
			m_ActionState = ActionState::blowing;
			AttackOne();
			break;
		case 1:
			m_ActionState = ActionState::happy;
			AttackTwo();
			break;
		case 2:
			m_ActionState = ActionState::inhaling;
			AttackThree();
			break;
		}

		i = rand() % 4;
		switch (i)
		{
		case 0:
			m_Velocity.x = m_Speed;
			break;
		case 1:
			m_Velocity.x = -m_Speed;
			break;
		case 2:
			m_Velocity.y = m_Speed;
			break;
		case 3:
			m_Velocity.y = -m_Speed;
			break;
		}

		m_AnimTime = 0.f;
	}
	
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	if (m_HealthBar.GetHealth() < m_HealthBar.GetMaxHealth() / 2.f)
	{
		m_ActionState = ActionState::exhausted;
	}

	// update flies
	m_pMonsterManager->Update(elapsedSec, pActor, pRoom);

	// handle collision with room
	pRoom->HandleCollision(m_Shape, m_Velocity);

	// check collision actor - boss
	if (Boss::TouchedByCharacter(pActor->GetShape()))
	{
		pActor->HitByObject(m_Damage);
	}

	// check collision actor - flies --> in update flies

	// check tear collision actor - boss
	pActor->ObjectHit(this);
}

void Boss::HitByTears(int damage)
{
	m_HealthBar.DoDamage(damage);

	if (m_HealthBar.GetHealth() <= 0)
		m_IsDead = true;
}

bool Boss::TouchedByCharacter(const Rectf& actorShape)
{
	return utils::IsOverlapping(actorShape, m_Shape);
}


Rectf Boss::GetShape() const
{
	return m_Shape;
}
Vector2f Boss::GetVelocity() const
{
	return m_Velocity;
}

HealthBar& Boss::GetHealthBar()
{
	return m_HealthBar;
}

bool Boss::DidYouWin()
{
	return (m_IsDead && (m_pMonsterManager->GetNrOfMonsters() == 0));
}


// --> HELPER FUNCTIONS
Rectf Boss::determineSrcRect() const
{
	Rectf srcRect{};
	switch (m_ActionState)
	{
	case Boss::ActionState::inhaling:
		srcRect.left = 0.f;
		srcRect.bottom = m_ClipHeight;
		break;
	case Boss::ActionState::blowing:
		srcRect.left = m_ClipWidth;
		srcRect.bottom = m_ClipHeight * 2;
		break;
	case Boss::ActionState::happy:
		srcRect.left = m_ClipWidth;
		srcRect.bottom = m_ClipHeight;
		break;
	case Boss::ActionState::exhausted:
		srcRect.left = 0.f;
		srcRect.bottom = m_ClipHeight * 2;
		break;
	}
	srcRect.width = m_ClipWidth;
	srcRect.height = m_ClipHeight;
	return srcRect;
}

void Boss::AttackOne()
{
	int nrOfMonsters{ rand() % 6 + 1 };
	Point2f pos{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
	m_pMonsterManager->AddFlies(pos, nrOfMonsters);
}
void Boss::AttackTwo()
{
	AttackOne();
}
void Boss::AttackThree()
{
	AttackOne();
}

// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const Boss& boss)
{
	out << std::setfill(' ') << std::setw(9) << "" << "<Boss \n";
	out << std::setw(9) << "" << " Position=\"" << boss.m_Shape.left << ',' << boss.m_Shape.bottom << "\" \n";
	out << std::setw(9) << "" << " HealthBar=\"" << boss.m_HealthBar << "\" \n";
	
	out << *boss.m_pMonsterManager;

	out << std::setw(9) << "" << "/Boss> \n";

	return out;
}
