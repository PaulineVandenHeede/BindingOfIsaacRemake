#include "pch.h"
#include "Camera.h"
#include "utils.h"

Camera::Camera(float width, float height)
	: m_Width{width}
	, m_Height{height}
	, m_AnimTime{ 0.f }
	, m_MaxAnimTime{ 1.f }
	, m_LevelBoundaries{107.5f, 107.5f, 915.f, 525.f}
{
}
void Camera::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime > m_MaxAnimTime)
	{
		m_ZoomIn = false;
		m_AnimTime -= m_MaxAnimTime;
	}
}
void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::SetLevelBoundaries(const Point2f & pos)
{
	m_LevelBoundaries = Rectf{ pos.x, pos.y, m_Width, m_Height };
}

void Camera::Transform(const Rectf& target) const
{
	Point2f newPos{ Track(target) };

	if (m_ZoomIn)
	{
		glScalef(1.5f, 1.5f, 0.f);
		glTranslatef(-newPos.x - (m_Width / (1.5f * 4)), -newPos.y - (m_Height / (1.5f * 4)), 0);
	}
	else
	{
		Clamp(newPos);

		glTranslatef(-newPos.x, -newPos.y, 0);
	}

}

bool Camera::GetZoomIn() const
{
	return m_ZoomIn;
}

void Camera::SetToZoomIn()
{
	m_ZoomIn = true;
	m_AnimTime = 0.f;
}


// helper functions
Point2f Camera::Track(const Rectf& target) const
{
	Point2f newPos{};
	// newPos.x = target.left + (target.width / 2.f) - (m_Width / 2.f);
	// newPos.y = target.bottom + (target.height / 2.f) - (m_Height / 2.f);

	newPos.x = target.left + (target.width / 2.f) - (m_Width / 2.f);
	newPos.y = target.bottom + (target.height / 2.f) - (m_Height / 2.f);
	return newPos;
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	// check left side --> if it's at left it can't be at right
	if (bottomLeftPos.x < m_LevelBoundaries.left)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	else if ((bottomLeftPos.x + m_Width) > (m_LevelBoundaries.left + m_LevelBoundaries.width)) // check right side
	{
		bottomLeftPos.x = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width;
	}

	// check bottom --> if it's at bottom it can't be at top
	if (bottomLeftPos.y < m_LevelBoundaries.bottom)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
	else if ((bottomLeftPos.y + m_Height) > (m_LevelBoundaries.bottom + m_LevelBoundaries.height)) // check top
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height;
	}
}
