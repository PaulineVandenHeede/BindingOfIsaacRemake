#include "pch.h"
#include "TearMonster.h"
#include "utils.h"
#include "Texture.h"
#include "Sprite.h"

TearMonster::TearMonster(const Point2f& position,Direction direction, Texture* pTexture, int range)
	: m_Radius{ 10.f }
	, m_Speed{ 500.f }
	, m_Acceleration{ -981.f }
	, m_Velocity{ 0.f, 0.f }
	, m_Range{ range }
	, m_Angle{ 0.f }
	, m_RotatingSpeed{ 360 }
	, m_pTearSprite{ nullptr }
	, m_ClipSize{ 64.f }
	, m_NrOfFrames{ 16 }
	, m_Scale{ 1.75f }
	, m_IsHit{ false }
	, m_pTexture{ pTexture }
{
	m_Shape.center = Point2f{ position.x, position.y };
	m_Shape.radius = 15.f * m_Scale;
	m_OriginalPosition = Point2f{ position.x - (m_ClipSize * m_Scale) / 2, position.y - (m_ClipSize * m_Scale) / 2 };

	m_pTearSprite = new Sprite{ m_pTexture, 4, 4, 16, m_ClipSize, m_ClipSize, 0, m_ClipSize, 10 };

}

TearMonster::~TearMonster()
{
	delete m_pTearSprite;
	m_pTearSprite = nullptr;
}
TearMonster& TearMonster::operator= (const TearMonster& other)
{
	m_Radius = other.m_Radius;
	m_Shape = other.m_Shape;
	m_OriginalPosition = other.m_OriginalPosition;
	m_Speed = other.m_Speed;

	return *this;
}

bool TearMonster::OutOfRange() const
{
	switch (m_Direction)
	{
	case Direction::up:
		return (m_Shape.center.y > m_OriginalPosition.y + m_Range);
		break;
	case Direction::down:
		return (m_Shape.center.y < m_OriginalPosition.y - m_Range);
		break;
	case Direction::right:
		return (m_Shape.center.x > m_OriginalPosition.x + m_Range);
		break;
	case Direction::left:
		return (m_Shape.center.x < m_OriginalPosition.x - m_Range);
		break;
	}
	return false;
}

void TearMonster::Update(float elapsedSec, const Stage& stage)
{
	m_Angle += m_RotatingSpeed * elapsedSec;

	if (m_IsHit || stage.HandleCollision(m_Shape, m_Velocity))
	{
		m_IsHit = true;
		m_pTearSprite->Update(elapsedSec);
		return;
	}

	if (OutOfRange())
	{
		if (m_Direction == Direction::left || m_Direction == Direction::right)
		{
			if ((m_Shape.center.y - ((m_ClipSize * m_Scale) / 2)) > ((m_OriginalPosition.y - (m_ClipSize * m_Scale) / 2)))
			{
				m_Velocity.y += m_Acceleration * elapsedSec;

				m_Shape.center.x += m_Velocity.x * elapsedSec;
				m_Shape.center.y += m_Velocity.y * elapsedSec;
			}
		}
		m_pTearSprite->Update(elapsedSec);
		return;
	}

	m_Shape.center.x += m_Velocity.x * elapsedSec;
	m_Shape.center.y += m_Velocity.y * elapsedSec;

	m_pTearSprite->SetActFrame(0);

}
void TearMonster::Draw() const
{
	Point2f pos{ m_Shape.center.x - (m_ClipSize * m_Scale) / 2 , m_Shape.center.y - (m_ClipSize * m_Scale) / 2 };
	if (m_IsHit || OutOfRange())
	{
		m_pTearSprite->Draw(pos, m_Scale, m_Scale);
	}
	else
	{
		glPushMatrix();

		glTranslatef(pos.x + (m_ClipSize * m_Scale) / 2, pos.y + (m_ClipSize * m_Scale) / 2, 0);
		glRotatef(m_Angle, 0.f, 0.f, 1.f); // rotate tear
		glTranslatef(-pos.x - (m_ClipSize * m_Scale) / 2, -pos.y - (m_ClipSize * m_Scale) / 2, 0);

		m_pTearSprite->Draw(pos, m_Scale, m_Scale);

		glPopMatrix();
	}
}
bool TearMonster::TouchedGround()
{
	return (m_pTearSprite->GetActFrame() == (m_NrOfFrames - 1));
}
void TearMonster::HitMonster()
{
	m_IsHit = true;
}

Direction TearMonster::GetDirectionTear() const
{
	return m_Direction;
}
Circlef TearMonster::GetShape() const
{
	return m_Shape;
}
bool TearMonster::GetIsHit() const
{
	return m_IsHit;
}

// --> HELPER FUNCTIONS
