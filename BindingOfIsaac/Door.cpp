#include "pch.h"
#include "Door.h"
#include "Texture.h"
#include "Sprite.h"
#include "utils.h"
#include "Singleton.h"


Door::Door(const Point2f& pos, Direction direction)
	: m_Vertices{ }
	, m_pTexture{ nullptr }
	, m_pDoor{ nullptr }
	, m_pDoorFrame{ nullptr }
	, m_Shape{ }
	, m_Scale{ 2.5f }
	, m_Direction{ direction }
	, m_Position{ pos }
	, m_State{ Door::State::open }
{
	 m_Shape = Rectf{ pos.x, pos.y, 64.f * m_Scale, 48.f * m_Scale };

	if (m_Direction == Direction::up || m_Direction == Direction::down)
	{
		m_Shape.left = pos.x + 22.f * m_Scale;
		m_Shape.bottom = pos.y + 6.f * m_Scale;
		m_Shape.width = (64.f - 22.f - 21.f) * m_Scale;
		m_Shape.height = (48.f - 10.f) * m_Scale;
	}
	else if (m_Direction == Direction::right || m_Direction == Direction::left)
	{
		m_Shape.left = pos.x + 11.f * m_Scale;
		m_Shape.bottom = pos.y + 14.f * m_Scale;
		m_Shape.width = (64.f - 17.f - 8.f) * m_Scale;
		m_Shape.height = (48.f - 14.f - 13.f) * m_Scale;
	}

	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/grid/door_01_normaldoor.png");
	
	m_pDoorFrame = new Sprite{ m_pTexture, 1, 2, 2, 64.f, 48.f, 0.f, 48.f, 10 };
	m_pDoorFrame->SetActFrame(0);

	m_pDoor = new Sprite{ m_pTexture, 1, 3, 3, 64.f, 48.f, 64.f, 48.f, 10 };
	m_pDoor->SetActFrame(0);
}

Door::~Door()
{
	delete m_pDoorFrame;
	m_pDoorFrame = nullptr;

	delete m_pDoor;
	m_pDoor = nullptr;
}

void Door::Draw() const
{
	switch (m_Direction)
	{
	case Direction::up:
		m_pDoor->Draw(m_Position, m_Scale, m_Scale);
		m_pDoorFrame->Draw(m_Position, m_Scale, m_Scale);
		break;
	case Direction::down:
		DrawRotatedSprite(m_pDoor, m_Position, 180.f);
		DrawRotatedSprite(m_pDoorFrame, m_Position, 180.f);
		break;
	case Direction::left:
		DrawRotatedSprite(m_pDoor, m_Position, 90.f);
		DrawRotatedSprite(m_pDoorFrame, m_Position, 90.f);
		break;
	case Direction::right:
		DrawRotatedSprite(m_pDoor, m_Position, -90.f);
		DrawRotatedSprite(m_pDoorFrame, m_Position, -90.f);
		break;
	}

}

void Door::PrintDoorPosition() const
{
	std::cout << "leftbottom: " << m_Vertices[0].x << "," << m_Vertices[0].y << '\n';
	std::cout << "rightbottom: " << m_Vertices[1].x << "," << m_Vertices[1].y << '\n';
	std::cout << "righttop: " << m_Vertices[2].x << "," << m_Vertices[2].y << '\n';
	std::cout << "lefttop: " << m_Vertices[3].x << "," << m_Vertices[3].y << '\n';
}

bool Door::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	if (m_State == State::closed)return false;

	// you can only walk in one direction through the door + 100.f is a fourth of the max speed
	switch (m_Direction)
	{
	case Direction::up:
		if (actorVelocity.y < 100.f)
			return false;
		break;
	case Direction::down:
		if (actorVelocity.y > -100.f)
			return false;
		break;
	case Direction::right:
		if (actorVelocity.x < 100.f)
			return false;
		break;
	case Direction::left:
		if (actorVelocity.x > -100.f)
			return false;
		break;
	}

	if (utils::IsOverlapping(m_Shape, actorShape))
	{
		float offset{ 47.f };
		float extraOffset{ 20.f };
		switch (m_Direction)
		{
		case Direction::up:
			actorShape.bottom += (offset + extraOffset) * m_Scale * 2.f;
			break;
		case Direction::down:
			actorShape.bottom -= (offset + extraOffset) * m_Scale * 2.f;
			break;
		case Direction::right:
			actorShape.left += (offset + extraOffset) * m_Scale * 2.f;
			break;
		case Direction::left:
			actorShape.left -= (offset + extraOffset) * m_Scale * 2.f;
			break;
		}

		return true;
	}
	return false;
}

Rectf Door::GetShape() const
{
	return m_Shape; // of m_Vertices? --> voor wat is het nodig? collision of texture?
}
Direction Door::GetDirection() const
{
	return m_Direction;
}

void Door::SetActionState(const Door::State state)
{
	m_State = state;
	switch (m_State)
	{
	case Door::State::open:
		m_pDoor->SetActFrame(0);
		break;
	case Door::State::closed:
		m_pDoor->SetActFrame(1);
		break;
	case Door::State::locked:
		m_pDoor->SetActFrame(2);
		break;
	}
}

void Door::DrawRotatedSprite(const Sprite* sprite, const Point2f & pos, float angle) const
{
	Point2f translation{ pos.x + (sprite->GetClipWidth() * m_Scale) / 2, pos.y + (sprite->GetClipHeight() * m_Scale) / 2 };
	glPushMatrix();

	glTranslatef(translation.x, translation.y, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef( - translation.x, - translation.y, 0);
	sprite->Draw(pos, m_Scale, m_Scale);

	glPopMatrix();
}