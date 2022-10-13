#include "pch.h"
#include "TearManagerMonster.h"
#include "Texture.h"

TearManagerMonster::TearManagerMonster(Type type, int range, float speed)
	: m_pTextureManager{ TextureManager::GetInstance() }
	, m_pTexture{ nullptr }
	, m_MaxTime{ 0.5f }
	, m_Time{ 0.6f } // higher than maxTime so that there can be a tear instant created
	, m_Speed{ speed }
	, m_Type{ type }
	, m_Range{ range }
	, m_IsEven{ false }
	, m_IsHit{ false }
{
	switch (m_Type)
	{
	case TearManagerMonster::Type::water:
		m_pTexture = m_pTextureManager->GetTexture("Resources/Images/effects/effect_015_tearpoofa.png");
		break;
	case TearManagerMonster::Type::blood:
		m_pTexture = m_pTextureManager->GetTexture("Resources/Images/effects/effect_003_bloodtear.png");
		break;
	}
}

TearManagerMonster::~TearManagerMonster()
{
	DeleteAllTears();
}

bool TearManagerMonster::CreateTear(const Point2f& position, Direction direction)
{
	if (m_Time > m_MaxTime)
	{
		m_IsEven = !m_IsEven;
		m_pTears.push_back(new Tear{ position, direction, m_IsEven, m_pTexture, m_Range, m_Speed });

		m_Time = 0.f;
		return true;
	}
	return false;
}
void TearManagerMonster::CreateTearsInFourDirection(const Point2f& pos, bool diagonal)
{
	if (diagonal)
		CreateTearsDiagonal(pos);
	else
		CreateTearsNonDiagonal(pos);
}
void TearManagerMonster::Draw() const
{
	for (Tear * pTear : m_pTears)
	{
		pTear->Draw();
	}
}
void TearManagerMonster::Update(float elapsedSec, const Stage& stage)
{
	m_Time += elapsedSec;
	for (Tear* pTear : m_pTears)
	{
		pTear->Update(elapsedSec, stage);
	}

	DeleteTearsOutOfRange();
}
void TearManagerMonster::DeleteAllTears()
{
	for (size_t i{}; i < m_pTears.size(); ++i)
	{
		delete m_pTears[i];
		m_pTears[i] = nullptr;
	}
}
bool TearManagerMonster::GetIsEven() const
{
	return m_IsEven;
}
std::vector<Tear*> TearManagerMonster::GetTears() const
{
	return m_pTears;
}

//--> HELPER FUNCTIONS
void TearManagerMonster::DeleteTearsOutOfRange()
{
	for (size_t i{}; i < m_pTears.size(); ++i)
	{
		DeleteTear(int(i));
	}
}
void TearManagerMonster::DeleteTear(int index)
{
	if (m_pTears[index]->TouchedGround())
	{
		delete m_pTears[index];
		std::swap(m_pTears[index], m_pTears.back());
		m_pTears.pop_back();
	}
}

void TearManagerMonster::CreateTearsDiagonal(const Point2f& pos)
{
	
}
void TearManagerMonster::CreateTearsNonDiagonal(const Point2f& pos)
{
	if (m_Time > m_MaxTime)
	{
		m_IsEven = true;
		for (int i{}; i < 4; ++i)
		{
			m_pTears.push_back(new Tear{ pos, Direction(i), m_IsEven, m_pTexture, m_Range, m_Speed });
		}
		// m_pTears.push_back(new Tear{ pos, Direction::right, m_IsEven, m_pTexture, m_Range, m_Speed });
		// m_pTears.push_back(new Tear{ pos, Direction::up, m_IsEven, m_pTexture, m_Range, m_Speed });
		// m_pTears.push_back(new Tear{ pos, Direction::down, m_IsEven, m_pTexture, m_Range, m_Speed });

		m_Time = 0.f;
	}
}
