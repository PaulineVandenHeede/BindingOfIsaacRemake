#include "pch.h"
#include "TearManager.h"
#include "NormalTear.h"
#include "InterpolatedTear.h"
#include "Texture.h"
#include "Room.h"
#include "Singleton.h"
#include "Character.h"
#include "Monster.h"
#include "ScreenFlow.h"
#include "Boss.h"

TearManager::TearManager(Type type, int range, float speed, int damage)
	: m_pTexture{ nullptr }
	, m_MaxTime{ 0.5f }
	, m_Time{ 0.6f } // higher than maxTime so that there can be a tear instant created
	, m_Speed{ speed }
	, m_Scale{ 1.75 }
	, m_Type{ type }
	, m_Range{ range }
	, m_IsEven{ false }
	, m_IsHit{ false }
	, m_Damage{ damage }
{
	switch (m_Type)
	{
	case TearManager::Type::water:
		m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/effects/effect_015_tearpoofa.png");
		break;
	case TearManager::Type::blood:
		m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/effects/effect_003_bloodtear.png");
		break;
	}
}

TearManager::~TearManager()
{
	DeleteAllTears();
}

bool TearManager::CreateTear(const Point2f& position, Direction direction, int damage)
{
	if (m_Time > m_MaxTime)
	{
		m_IsEven = !m_IsEven;
		m_pTears.push_back(new NormalTear{ position, direction, m_IsEven, m_pTexture, m_Range, m_Speed, m_Scale, m_Type, m_Damage });

		std::string fileName{ "Resources/Sounds/tear/tear fire 5.wav" };
		Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);
		m_Time = 0.f;
		return true;
	}
	return false;
}
void TearManager::CreateInterpolatedTear(const Point2f& beginPos, const Point2f& endPos, int damage)
{
	m_pTears.push_back(new InterpolatedTear{ beginPos, endPos, m_pTexture, m_Scale, m_Type, m_Damage });

}
void TearManager::CreateTearsInFourDirection(const Point2f& pos, int damage)
{
	if (m_Time > m_MaxTime)
	{
		m_IsEven = true;
		for (int i{}; i < 4; ++i)
		{
			m_pTears.push_back(new NormalTear{ pos, Direction(i), m_IsEven, m_pTexture, m_Range, m_Speed, m_Scale, m_Type, m_Damage });
		}

		m_Time = 0.f;
	}
}
void TearManager::Draw() const
{
	for (Tear * pTear : m_pTears)
	{
		pTear->Draw();
	}
}
void TearManager::Update(float elapsedSec, const Room* room)
{
	m_Time += elapsedSec;
	for (Tear* pTear : m_pTears)
	{
		pTear->Update(elapsedSec, room);
	}

	DeleteTearsOutOfRange();
}

void TearManager::DeleteAllTears()
{
	for (size_t i{}; i < m_pTears.size(); ++i)
	{
		delete m_pTears[i];
		m_pTears[i] = nullptr;
	}
	m_pTears.clear();
}
bool TearManager::GetIsEven() const
{
	return m_IsEven;
}
int TearManager::GetDamage() const
{
	return m_Damage;
}

void TearManager::ChangeRange(int decrement)
{
	m_Range += decrement;
}
void TearManager::ChangeDamage(int decrement)
{
	m_Damage += decrement;
}

void TearManager::HitObject(Character* pActor)
{
	for (Tear* pTear : m_pTears)
	{
		if (pTear->IsOverlapping(pActor->GetShape()))
		{
			// update character
			pActor->HitByObject(pTear->GetDamage());
			// update tear
			pTear->HitSomething();
		}
	}
}

void TearManager::HitObject(Monster * pMonster)
{
	for (Tear* pTear : m_pTears)
	{
		if (pTear->IsOverlapping(pMonster->GetShape()))
		{
			// update monster
			pMonster->HitByTears(pTear->GetDamage());
			// update tear
			pTear->HitSomething();
		}
	}
}

void TearManager::HitObject(Boss* pBoss)
{
	for (Tear* pTear : m_pTears)
	{
		if (pTear->IsOverlapping(pBoss->GetShape()))
		{
			// update monster
			pBoss->HitByTears(pTear->GetDamage());
			// update tear
			pTear->HitSomething();
		}
	}
}



//--> HELPER FUNCTIONS
void TearManager::DeleteTearsOutOfRange()
{
	for (size_t i{}; i < m_pTears.size(); ++i)
	{
		DeleteTear(int(i));
	}
}
void TearManager::DeleteTear(int index) 
{
	if (m_pTears[index]->ReadyToDelete())
	{
		delete m_pTears[index];
		std::swap(m_pTears[index], m_pTears.back());
		m_pTears.pop_back();

		if (m_Type == TearManager::Type::water)
		{
			std::string fileName{ "Resources/Sounds/tear/tear block.wav" };
			Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);
		}
	}
}
