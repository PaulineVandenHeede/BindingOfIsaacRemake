#include "pch.h"
#include "Room.h"
#include "Singleton.h"
#include "RoomManager.h"
#include <sstream>

Room::Room(const Point2f& pos, std::pair<int, int> gridPos, State state, float scale)
	: m_pTexture{ nullptr }
	, m_Position { pos }
	, m_ClipWidth{ 226.f }
	, m_ClipHeight{ 148.f }
	, m_Width{ }
	, m_Height{ }
	, m_WidthWall{ 43.f }
	, m_HeightWall{ 43.f }
	, m_Scale{ scale }
	, m_State{ state }
	, m_GridPos{ gridPos }
{
	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/01_basement.png");
	
	m_Width = m_ClipWidth * m_Scale * 2.f;
	m_Height = m_ClipHeight * m_Scale * 2.f;

	m_WidthWall *= m_Scale;
	m_HeightWall *= m_Scale;
	
	// room boundaries
	m_Shape.bottom = pos.y + m_HeightWall;
	m_Shape.left = pos.x + m_WidthWall;
	m_Shape.width = m_Width - 2 * m_WidthWall;
	m_Shape.height = m_Height - 1.5f * m_HeightWall;
}

Room::Room(std::stringstream& info, float scale)
	: m_pTexture{ nullptr }
	, m_Scale{ scale }
	, m_ClipWidth{ 226.f }
	, m_ClipHeight{ 148.f }
	, m_Width{ }
	, m_Height{ }
	, m_WidthWall{ 43.f }
	, m_HeightWall{ 43.f }
{
	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/01_basement.png");

	m_Width = m_ClipWidth * m_Scale * 2.f;
	m_Height = m_ClipHeight * m_Scale * 2.f;

	m_WidthWall *= m_Scale;
	m_HeightWall *= m_Scale;

	std::string part{};
	// gridpos
	part = utils::GetAttributeValue("GridPos", info.str());
	if(part != "")
		m_GridPos = StringToGridPos(part);

	// state
	part = utils::GetAttributeValue("State", info.str());
	if(part != "")
		m_State = StringToState(part);

	// pos
	m_Position = Point2f {m_GridPos.first * m_Width, -m_GridPos.second * m_Height};

	// room boundaries
	m_Shape.bottom = m_Position.y + m_HeightWall;
	m_Shape.left = m_Position.x + m_WidthWall;
	m_Shape.width = m_Width - 2 * m_WidthWall;
	m_Shape.height = m_Height - 1.5f * m_HeightWall;
}


void Room::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	if (actorShape.left < m_Shape.left) // check left side
	{
		actorShape.left = m_Shape.left;
		actorVelocity.x = 0.f;
	}
	// if left is true, right can't be true
	else if ((actorShape.left + actorShape.width) > (m_Shape.left + m_Shape.width)) // check right side
	{
		actorShape.left = m_Shape.left + m_Shape.width - actorShape.width;
		actorVelocity.x = 0.f;
	}
	if (actorShape.bottom < m_Shape.bottom) // check bottom
	{
		actorShape.bottom = m_Shape.bottom;
		actorVelocity.y = 0.f;
	}
	// if bottom is true, top can't be true
	else if ((actorShape.bottom + actorShape.height) > (m_Shape.bottom + m_Shape.height)) // check top side
	{
		actorShape.bottom = m_Shape.bottom + m_Shape.height - actorShape.height;
		actorVelocity.y = 0.f;
	}
}
void Room::HandleCollisionMonster(Rectf& actorShape, Vector2f& actorVelocity) const
{
	if (actorShape.left < m_Shape.left) // check left side
	{
		actorShape.left = m_Shape.left;
		actorVelocity.x = 0.f;
	}
	// if left is true, right can't be true
	else if ((actorShape.left + actorShape.width) > (m_Shape.left + m_Shape.width)) // check right side
	{
		actorShape.left = m_Shape.left + m_Shape.width - actorShape.width;
		actorVelocity.x = 0.f;
	}
	if (actorShape.bottom < m_Shape.bottom) // check bottom
	{
		actorShape.bottom = m_Shape.bottom;
		actorVelocity.y = 0.f;
	}
	// if bottom is true, top can't be true
	else if ((actorShape.bottom + actorShape.height) > (m_Shape.bottom + m_Shape.height - (m_HeightWall * 0.5f))) // check top side
	{
		actorShape.bottom = m_Shape.bottom + m_Shape.height - actorShape.height - (m_HeightWall * 0.5f);
		actorVelocity.y = 0.f;
	}
}
bool Room::HandleCollisionTears(Circlef& tearShape, Vector2f& tearVelocity) const
{
	float offset{ 0.5f * m_HeightWall };

	if ((tearShape.center.x - tearShape.radius) < (m_Shape.left - offset)) // check left side
	{
		tearShape.center.x = m_Shape.left - offset + tearShape.radius;
		tearVelocity.x = 0.f;
		return true;
	}
	// if left is true, right can't be true
	else if ((tearShape.center.x + tearShape.radius) > (m_Shape.left + m_Shape.width + offset)) // check right side
	{
		tearShape.center.x = m_Shape.left + m_Shape.width + offset - tearShape.radius;
		tearVelocity.x = 0.f;
		return true;
	}
	if ((tearShape.center.y - tearShape.radius) < (m_Shape.bottom - offset)) // check bottom
	{
		tearShape.center.y = m_Shape.bottom - offset + tearShape.radius;
		tearVelocity.x = 0.f;
		return true;
	}
	// if bottom is true, top can't be true
	else if ((tearShape.center.y + tearShape.radius) > (m_Shape.bottom + m_Shape.height)) // check top side
	{
		tearShape.center.y = m_Shape.bottom + m_Shape.height - tearShape.radius;
		tearVelocity.x = 0.f;
		return true;
	}

	// if it didn't hit room border
	return false;
}

bool Room::ChangeState(Room::State state) 
{
	// returns false when room is cleared and returns true in all other states
	switch (state)
	{
	case State::closed:
	case State::cleared:
		m_State = state;
	case State::none:
		if (m_State == State::cleared)
			return false;
		return true;
	default:
		return false;
	}
}

Room::State Room::GetState() const
{
	return m_State;
}

std::pair<int, int> Room::GetGridPosition() const
{
	return m_GridPos;
}

Rectf Room::GetShape() const
{
	return m_Shape;
}

// --> HELPER FUNCTIONS
void Room::DrawRoom() const
{
	DrawRoomHelper();
}
void Room::DrawRoomHelper() const
{
	Rectf destRect{};
	destRect.left = m_Position.x;
	destRect.bottom = m_Position.y;
	destRect.width = m_Width / 2;
	destRect.height = m_Height / 2;

	int scaleX{ 1 };
	int scaleY{ -1 };
	int scaleZ{ 1 };

	DrawQuarterRoom(destRect, m_SrcRect, scaleX, scaleY, scaleZ); // Draw bottom left room

	destRect.left = m_Position.x + destRect.width;
	destRect.bottom = m_Position.y;
	scaleX = -1;

	DrawQuarterRoom(destRect, m_SrcRect, scaleX, scaleY, scaleZ); // Draw bottom right room


	destRect.left = m_Position.x + destRect.width;
	destRect.bottom = m_Position.y + destRect.height;
	scaleY = 1;

	DrawQuarterRoom(destRect, m_SrcRect, scaleX, scaleY, scaleZ); // Draw top right room

	destRect.left = m_Position.x;
	destRect.bottom = m_Position.y + destRect.height;

	m_pTexture->Draw(destRect, m_SrcRect); // Draw top left room

}
void Room::DrawQuarterRoom(const Rectf& destRect, const Rectf& srcRect, int scaleX, int scaleY, int scaleZ) const
{
	glPushMatrix();
	glTranslatef(destRect.left + destRect.width / 2, destRect.bottom + destRect.height / 2, 0);
	glScalef(float (scaleX), float(scaleY), float (scaleZ)); // keer afbeelding om
	glTranslatef(-destRect.left - destRect.width / 2, -destRect.bottom - destRect.height / 2, 0);

	m_pTexture->Draw(destRect, srcRect);

	glPopMatrix();
}

std::pair<int, int> Room::StringToGridPos(const std::string & pos)
{
	std::pair<int, int> temp{};
	std::stringstream posCopy{ pos };
	std::string part{};

	std::getline(posCopy, part, ',');
	temp.first = std::stoi(part);
	std::getline(posCopy, part);
	temp.second = std::stoi(part);

	return temp;
}

Room::State Room::StringToState(const std::string & state)
{
	State temp{};

	if (state == "none")
		temp = State::none;
	else if (state == "cleared")
		temp = State::cleared;
	else if (state == "closed")
		temp = State::closed;

	return temp;
}
