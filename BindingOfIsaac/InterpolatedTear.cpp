#include "pch.h"
#include "InterpolatedTear.h"
#include "Texture.h"
#include "Sprite.h"
#include "Room.h"
#include <iostream>

InterpolatedTear::InterpolatedTear(const Point2f& beginPos, const Point2f& endPos, Texture* pTexture, float scale, TearManager::Type type, int damage)
	: Tear{beginPos, pTexture, type, scale, damage }
	, m_EndPoint{ endPos }
	, m_LerpFactor{ 0.f }
	, m_DecrementFactor{ 0.01f }
{

}

void InterpolatedTear::Draw() const
{
	Point2f pos{ m_Shape.center };
	pos.x -= (m_ClipSize * m_Scale) / 2;
	pos.y -= (m_ClipSize * m_Scale) / 2;
	if (m_IsHit)
	{
		m_pTearSprite->Draw(pos, m_Scale, m_Scale);
		return;
	}

	glPushMatrix();

	glTranslatef(pos.x + (m_ClipSize * m_Scale) / 2, pos.y + (m_ClipSize * m_Scale) / 2, 0);
	glRotatef(m_Angle, 0.f, 0.f, 1.f); // rotate tear
	glTranslatef(-pos.x - (m_ClipSize * m_Scale) / 2, -pos.y - (m_ClipSize * m_Scale) / 2, 0);

	m_pTearSprite->Draw(pos, m_Scale, m_Scale);

	glPopMatrix();
	
}
void InterpolatedTear::Update(float elapsedSec, const Room* pRoom)
{
	if (m_IsHit)
	{
		AddEffect();
		return;
	}

	m_LerpFactor += m_DecrementFactor;
	if (m_LerpFactor > 1)
	{
		m_IsHit = true;

		AddEffect();
		return;
		// m_pTearSrite->Update(elapsedSec);
	}
	m_Shape.center = Lerp(m_BeginPos, m_EndPoint, m_LerpFactor);
	m_Angle += m_RotatingSpeed * elapsedSec;
}

// --> HELPER FUNCNTIONS
Point2f InterpolatedTear::Lerp(const Point2f& beginPoint, const Point2f& endPoint, float lerpFactor) const
{
	Point2f newPos{  };
	newPos.x = (1 - lerpFactor) * beginPoint.x + lerpFactor * endPoint.x;
	newPos.y = (1 - lerpFactor) * beginPoint.y + lerpFactor * endPoint.y;

	return newPos;
		// ((1 - lerpFactor) * beginPoint + lerpFactor * endPoint);
}