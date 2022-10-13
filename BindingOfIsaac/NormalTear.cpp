#include "pch.h"
#include "NormalTear.h"
#include "Texture.h"
#include "RoomManager.h"
#include "Singleton.h"
#include "Sprite.h"
#include "Room.h"

NormalTear::NormalTear(const Point2f& position, Direction direction, bool isEven, Texture* pTexture, int range, float speed, float scale, TearManager::Type type, int damage)
	: Tear{ position, pTexture, type, scale, damage }
	, m_Velocity{ 0.f, 0.f }
	, m_Speed{ speed }
	, m_Acceleration{ -981.f }
	, m_Direction{ direction }
	, m_Range{ range }
{
	// initialize m_Velocity
	switch (m_Direction)
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

	m_Shape.center = Point2f{ position.x, position.y };
	m_Shape.radius = 15.f * m_Scale;
	m_BeginPos = Point2f{ position.x - (m_ClipSize * m_Scale) / 2, position.y - (m_ClipSize * m_Scale) / 2 };
}

void NormalTear::Draw() const
{
	Point2f pos{ m_Shape.center.x - (m_ClipSize * m_Scale) / 2 , m_Shape.center.y - (m_ClipSize * m_Scale) / 2 };
	glPushMatrix();

	glTranslatef(pos.x + (m_ClipSize * m_Scale) / 2, pos.y + (m_ClipSize * m_Scale) / 2, 0);
	glRotatef(m_Angle, 0.f, 0.f, 1.f); // rotate tear
	glTranslatef(-pos.x - (m_ClipSize * m_Scale) / 2, -pos.y - (m_ClipSize * m_Scale) / 2, 0);

	m_pTearSprite->Draw(pos, m_Scale, m_Scale);

	glPopMatrix();
}
void NormalTear::Update(float elapsedSec, const Room* pRoom)
{
	m_Angle += m_RotatingSpeed * elapsedSec;

	if (m_IsHit || pRoom->HandleCollisionTears(m_Shape, m_Velocity))
	{
		AddEffect();

		m_IsHit = true;

		return;
	}

	if (OutOfRange())
	{
		if (m_Direction == Direction::left || m_Direction == Direction::right)
		{
			if ((m_Shape.center.y - ((m_ClipSize * m_Scale) / 2)) > ((m_BeginPos.y - (m_ClipSize * m_Scale) / 2)))
			{
				m_Velocity.y += m_Acceleration * elapsedSec;

				m_Shape.center.x += m_Velocity.x * elapsedSec;
				m_Shape.center.y += m_Velocity.y * elapsedSec;
			}
			else
			{
				AddEffect();
				m_IsHit = true;
			}
		}
		else
		{
			AddEffect();
			m_IsHit = true;
		}
		return;
	}

	m_Shape.center.x += m_Velocity.x * elapsedSec;
	m_Shape.center.y += m_Velocity.y * elapsedSec;

	m_pTearSprite->SetActFrame(0);
}

// --> HELPER FUNCTIONS
bool NormalTear::OutOfRange() const
{
	switch (m_Direction)
	{
	case Direction::up:
		return (m_Shape.center.y > m_BeginPos.y + m_Range);
	case Direction::down:
		return (m_Shape.center.y < m_BeginPos.y - m_Range);
	case Direction::right:
		return (m_Shape.center.x > m_BeginPos.x + m_Range);
	case Direction::left:
		return (m_Shape.center.x < m_BeginPos.x - m_Range);
	default:
		return false;
	}
}