#include "pch.h"
#include "Character.h"
#include "Texture.h"
#include "Body.h"
#include "Head.h"
#include "Tear.h"
#include "utils.h"
#include "Monster.h"
#include "Boss.h"
#include "Room.h"
#include "Singleton.h"
#include "ChargeBar.h"
#include "ScreenFlow.h"
#include <sstream>
#include <iomanip>

Character::Character(const Point2f& position, int maxHealth, int health)
	: m_pTexture{ nullptr }
	, m_pBody{ nullptr }
	, m_pHead{ nullptr }
	, m_pChargeBar{ nullptr }
	, m_HealthBar{ maxHealth, health, 2.5f }
	, m_Velocity{ 0.f, 0.f }
	, m_Scale{ 3.f }
	, m_ClipSize{ 32.f }
	, m_Overlap{ 22.f }
	, m_AnimTime{ 0.f }
	, m_MaxAnimTime{ 0.3f }
	, m_Speed{ 400.f }
	, m_Angle{ 0.f }
	, m_MaxAngle{ 90.f }
	, m_ActionState{ ActionState::waiting }
	, m_Direction{ Direction::down }
	, m_CollidedWithDoor{ false }
	, m_IsHit{ false }
	, m_ShowCharacter{ true }
	, m_DrawHitTexture{ false }
	, m_Dead{ false }
{
	m_Shape.width = m_ClipSize * m_Scale;
	m_Shape.height = (m_ClipSize * 2 - m_Overlap - 7.f) * m_Scale; // 7.f?
	m_Shape.left = position.x - m_Shape.width / 2;
	m_Shape.bottom = position.y - m_Shape.height / 2;

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/characters/character_001_isaac.png");

	m_pBody = new Body{ Point2f { m_Shape.left, m_Shape.bottom }, m_pTexture };
	m_pHead = new Head{ Point2f { m_Shape.left, m_Shape.bottom + (m_ClipSize - m_Overlap) * m_Scale} , m_pTexture };
}

Character::Character(std::stringstream& info)
	: m_pTexture{ nullptr }
	, m_pBody{ nullptr }
	, m_pHead{ nullptr }
	, m_pChargeBar{ nullptr }
	, m_HealthBar{ 0, 0, 2.5f }
	, m_Velocity{ 0.f, 0.f }
	, m_Scale{ 3.f }
	, m_ClipSize{ 32.f }
	, m_Overlap{ 22.f }
	, m_AnimTime{ 0.f }
	, m_MaxAnimTime{ 0.3f }
	, m_Speed{ 400.f }
	, m_Angle{ 0.f }
	, m_MaxAngle{ 90.f }
	, m_ActionState{ ActionState::waiting }
	, m_Direction{ Direction::down }
	, m_CollidedWithDoor{ false }
	, m_IsHit{ false }
	, m_ShowCharacter{ true }
	, m_DrawHitTexture{ false }
{
	// get position out of file
	std::string part{};
	part = utils::GetAttributeValue("Position", info.str());
	Point2f position{ utils::StringToPos(part) };

	part = utils::GetAttributeValue("HealthBar", info.str());
	std::pair<int, int> health{ utils::StringToHealthBar(part) };

	m_HealthBar.IncreaseMaxHealth(health.second);
	m_HealthBar.DoHealing(health.first);

	m_Dead = (health.first == 0);

	// shape
	m_Shape.width = m_ClipSize * m_Scale;
	m_Shape.height = (m_ClipSize * 2 - m_Overlap - 7.f) * m_Scale; // 7.f?
	m_Shape.left = position.x - m_Shape.width / 2;
	m_Shape.bottom = position.y - m_Shape.height / 2;

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/characters/character_001_isaac.png");

	m_pBody = new Body{ Point2f { m_Shape.left, m_Shape.bottom }, m_pTexture };
	m_pHead = new Head{ Point2f { m_Shape.left, m_Shape.bottom + (m_ClipSize - m_Overlap) * m_Scale} , m_pTexture };

	// chargebar
	part = utils::GetPartValue("ChargeBar", info.str());
	if (part != "")
	{
		info.str(std::string{});
		info << part;
		m_pChargeBar = new ChargeBar{ info, m_Scale };
	}
}

Character::~Character()
{
	delete m_pBody;
	m_pBody = nullptr;

	delete m_pHead;
	m_pHead = nullptr;

	delete m_pChargeBar;
	m_pChargeBar = nullptr;

	m_pTexture = nullptr;
}

void Character::Draw() const
{
	if (m_HealthBar.GetHealth() <= 0)
	{
		Rectf srcRect{ 202.f, 203.f, 42.f , 35.f };
		Rectf destRect{ m_Shape.left, m_Shape.bottom, srcRect.width * m_Scale, srcRect.height * m_Scale };

		glPushMatrix();

		// sandwich rotator
		// place it back on right spot
		glTranslatef(destRect.left, destRect.bottom, 0.f);

		// rotate
		glRotatef(m_MaxAngle - m_Angle, 0.f, 0.f, 1.f);

		// make sure the origin of srcRect is in origin world
		glTranslatef(-destRect.left, -destRect.bottom , 0.f); 
		
		m_pTexture->Draw(destRect, srcRect);

		glPopMatrix();

		return;
	}

	// draw lemon attack
	if (m_pChargeBar != nullptr)
	{
		m_pChargeBar->Draw();
	}

	if (m_CollidedWithDoor)
		return;

	m_pHead->DrawTears();
		
	if (!m_ShowCharacter)
		return;

	if (m_DrawHitTexture)
	{
		Rectf srcRect{ 146.f, 261.f, m_Shape.width / m_Scale, m_Shape.height / m_Scale };
		Rectf destRect{m_Shape};
		m_pTexture->Draw(destRect, srcRect);
		return;
	}
	if (m_DrawPickUpTexture)
	{
		Rectf srcRect{ 80.f, 264.f, 32.f, 40.f };
		Rectf destRect{ m_Shape };
		m_pTexture->Draw(destRect, srcRect);
		return;
	}
	m_pBody->Draw();
	m_pHead->Draw();
}

void Character::Update(float elapsedSec, const Room* room)
{
	if (m_HealthBar.GetHealth() <= 0)
	{
		m_AnimTime += elapsedSec;

		if (m_AnimTime > (m_MaxAnimTime * 2 / 2))
			m_Angle = m_MaxAngle;

		if (m_AnimTime > (m_MaxAnimTime * 2))
		{
			m_Dead = true;
			ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::dead;

			// stop playing sounds
			Singleton::GetInstance()->GetSoundManager()->SetStop();
			Singleton::GetInstance()->GetEffectManager()->StopAllSounds();
		}
		return;
	}


	const Uint8 *pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_A])
	{
		m_Velocity.x = -m_Speed;
		m_Velocity.y = 0.f;

		ChangeDirectionAndActionState(ActionState::walking, Direction::left);
	}
	else if (pStates[SDL_SCANCODE_D]) // no 2 directions at the same time
	{
		m_Velocity.x = m_Speed;
		m_Velocity.y = 0.f;

		ChangeDirectionAndActionState(ActionState::walking, Direction::right);
	}
	else if (pStates[SDL_SCANCODE_W])
	{
		m_Velocity.x = 0.f;
		m_Velocity.y = m_Speed;

		ChangeDirectionAndActionState(ActionState::walking, Direction::up);
	}
	else if (pStates[SDL_SCANCODE_S])
	{
		m_Velocity.x = 0.f;
		m_Velocity.y = -m_Speed;

		ChangeDirectionAndActionState(ActionState::walking, Direction::down);
	}
	else
	{
		m_Velocity = Vector2f{ 0.f,0.f };
		
		ChangeDirectionAndActionState(ActionState::waiting, Direction::down);
	}

	m_pBody->Update(elapsedSec);
	m_pHead->Update(elapsedSec, room);

	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	m_pBody->SetPosition(Point2f{ m_Shape.left, m_Shape.bottom });
	m_pHead->SetPosition(Point2f{ m_Shape.left, m_Shape.bottom + (m_ClipSize - m_Overlap) * m_Scale });

	if (m_IsHit)
	{
		m_AnimTime += elapsedSec;

		m_ShowCharacter = !m_ShowCharacter;

		if (m_ActionState == Character::ActionState::waiting)
			m_DrawHitTexture = true;
		else
			m_DrawHitTexture = false;

		if (m_AnimTime > m_MaxAnimTime)
		{
			m_AnimTime = 0.f;
			m_IsHit = false;
			m_ShowCharacter = true;
			m_DrawHitTexture = false;
		}
	}
	if (m_DrawPickUpTexture)
	{
		m_AnimTime += elapsedSec;
		if (m_AnimTime > (m_MaxAnimTime ))
		{
			m_DrawPickUpTexture = false;
			m_AnimTime = 0.f;
		}
	}

	room->HandleCollision(m_Shape, m_Velocity);

	m_CollidedWithDoor = false;

	// update hearts in hud
	m_HealthBar.Update(elapsedSec);

	// chargebar
	if (m_pChargeBar != nullptr)
	{
		m_pChargeBar->Update(elapsedSec, m_Shape);
	}
}
void Character::ChangeCollidedWithDoor()
{
	m_CollidedWithDoor = !m_CollidedWithDoor;
}
Rectf& Character::GetShape()
{
	return m_Shape;
}
Vector2f& Character::GetVelocity()
{
	return m_Velocity;
}

HealthBar& Character::GetHealthBar()
{
	return m_HealthBar;
}
ChargeBar* Character::GetChargeBar()
{
	return m_pChargeBar;
}
bool Character::IsDead()
{
	return m_Dead;
}

void Character::HitByObject(int damage)
{
	if (m_IsHit)
		return;

	m_HealthBar.DoDamage(damage);

	if (m_HealthBar.GetHealth() <= 0)
	{
		m_AnimTime = 0;
		m_IsHit = true;

		std::string filename{"Resources/Sounds/character/isaac dies new.wav"};
		Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(filename, false);

		return;
	}

	std::string filename{ "Resources/Sounds/character/hurt grunt.wav" };
	Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(filename, false);



	m_IsHit = true;
}

void Character::ObjectHit(Monster* pMonster)
{
	if (m_pChargeBar != nullptr)
	{
		if (m_pChargeBar->IsOverlapping(pMonster->GetShape()))
		{
			// instant kill
			pMonster->HitByTears(pMonster->GetHealthBar().GetHealth());
		}
	}
	
	m_pHead->ObjectHit(pMonster);
}
void Character::ObjectHit(Boss* pBoss)
{
	if (m_pChargeBar != nullptr)
	{
		if (m_pChargeBar->IsOverlapping(pBoss->GetShape()))
		{
			// no instant kill
			pBoss->HitByTears(15);
		}
	}

	m_pHead->ObjectHit(pBoss);
}
void Character::PowerUpHit(const PowerUpInfo& powerUpInfo)
{
	std::string fileNameOne{ "Resources/Images/effects/effect_024_streak_" };
	std::string fileNameTwo{ "Resources/Images/items/collectibles_" };
	Point2f pos{ m_Shape.left - (200.f * 1.5f) + m_Shape.width / 2, m_Shape.bottom + m_Shape.height * 2};
	
	switch (powerUpInfo.type)
	{
	case TypePowerUp::belt:
		SetCostume(powerUpInfo.fileName, true, false);
		ChangeSpeed(1.04f);
		fileNameOne += "thebelt.png";
		fileNameTwo += "028_thebelt.png";
		break;
	case TypePowerUp::heart:
		GetHealthBar().DoHealing(2);
		fileNameOne += "aheart.png";
		fileNameTwo += "001_theheart.png";
		break;
	case TypePowerUp::snack:
		GetHealthBar().IncreaseMaxHealth(2);
		fileNameOne += "asnack.png";
		fileNameTwo += "346_asnack.png";
		break;
	case TypePowerUp::poison:
		ChangeSpeed(0.96f);
		ChangeTearRange(-100);
		SetCostume(powerUpInfo.fileName, true, true);
		fileNameOne += "poison.png";
		fileNameTwo += "013_thevirus.png";
		break;
	case TypePowerUp::lemon:
		CreateChargeBar();
		fileNameOne += "lemon.png";
		fileNameTwo += "056_lemonmishap.png";
	case TypePowerUp::end:
	default:
		break;
	}

	Singleton::GetInstance()->GetEffectManager()->AddEffect(fileNameOne, pos, 400.f, 64.f, 0.f, 64.f, 1.5f, true, 1, 1, 1, 1);

	pos = Point2f{ m_Shape.left, m_Shape.bottom + m_Shape.height };

	Singleton::GetInstance()->GetEffectManager()->AddEffect(fileNameTwo, pos, 32.f, 32.f, 0.f, 32.f, m_Scale, true, 1, 1, 1, 1);

	m_DrawPickUpTexture = true;
	m_AnimTime = 0.f;

	// sound
	std::string soundName{"Resources/Sounds/pickup/power up1.wav"};
	Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(soundName, false);

}

void Character::SetCostume(std::string filename, bool body, bool head)
{
	if (body)
	{
		m_pBody->SetCostumeBody(filename);
	}
	if (head)
	{
		m_pHead->SetCostumeHead(filename);
	}
}
void Character::SetPosition(Point2f& leftBottom)
{
	m_Shape.left = leftBottom.x;
	m_Shape.bottom = leftBottom.y;

	m_pBody->SetPosition(Point2f{ m_Shape.left, m_Shape.bottom });
	m_pHead->SetPosition(Point2f{ m_Shape.left, m_Shape.bottom + (m_ClipSize - m_Overlap) * m_Scale });

}

// --> HELPER FUNCTIONS
void Character::ChangeDirectionAndActionState(ActionState actionState, Direction direction)
{
	m_pBody->SetActionState(actionState);
	m_pBody->SetDirection(direction);

	m_pHead->SetActionState(actionState);
	m_pHead->SetDirection(direction);
}

void Character::ChangeDamage(int decrement)
{
	m_pHead->ChangeDamage(decrement);
}
void Character::ChangeTearRange(int decrement)
{
	m_pHead->ChangeTearRange(decrement);
}
void Character::ChangeSpeed(float factor)
{
	m_Speed *= factor;
}

void Character::CreateChargeBar()
{
	if (m_pChargeBar != nullptr)
		return;

	m_pChargeBar = new ChargeBar{ m_Scale };
}

// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const Character& character)
{
	out << std::setfill(' ') << std::setw(3) << "" << "<Character\n";

	out << std::setw(3) << "" << " Position=\"" << character.m_Shape.left << ',' << character.m_Shape.bottom << "\"\n";
	out << std::setw(3) << "" << " HealthBar=\"" << character.m_HealthBar << "\"\n";

	if (character.m_pChargeBar != nullptr)
	{
		out << *character.m_pChargeBar;
	}

	out << std::setw(3) << "" << "/Character>\n";

	return out;
}
