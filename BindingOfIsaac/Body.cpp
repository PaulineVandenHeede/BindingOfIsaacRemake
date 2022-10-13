#include "pch.h"
#include "Body.h"
#include <iostream>
#include "DoubleSprite.h"
#include "Singleton.h"

using mapIt = std::unordered_map<std::string, DoubleSprite*>::iterator;
using mapCit = std::unordered_map<std::string, DoubleSprite*>::const_iterator;

Body::Body(const Point2f& position, const Texture* texture)
	: m_pUpDownWalk{ nullptr }
	, m_pLeftRightWalk{ nullptr }
	, m_pSpriteBody{ nullptr }
	, m_pCostumes{  }
	, m_ClipSize{ 32.f }
	, m_Scale{ 3.f }
	, m_ActionState{ Character::ActionState::waiting }
	, m_Direction{ Direction::down }
{
	m_pSpriteBody = new DoubleSprite{ texture, 8, 2, 10, 20, m_ClipSize, m_ClipSize, 0.f, 2 * m_ClipSize };

	m_Shape.left = position.x;
	m_Shape.bottom = position.y - 5.f * m_Scale;
	m_Shape.width = m_ClipSize * m_Scale;
	m_Shape.height = m_ClipSize * m_Scale;
}

Body::~Body()
{
	delete m_pSpriteBody;
	m_pSpriteBody = nullptr;

	for (std::pair<std::string, DoubleSprite*> pElement : m_pCostumes)
	{
		delete pElement.second;
	}
	m_pCostumes.clear();

	m_pTextureCostume = nullptr;
}

void Body::Draw() const
{
	switch (m_ActionState)
	{
	case Character::ActionState::waiting:
		m_pSpriteBody->Draw(Point2f{ m_Shape.left, m_Shape.bottom - 5.f * m_Scale }, m_Scale, m_Scale); // 5.f is tov van de shape dat hij meer naar onder moet tekenen
		for (std::pair<std::string, DoubleSprite*> pElement : m_pCostumes)
		{
			pElement.second->Draw(Point2f{ m_Shape.left, m_Shape.bottom - 5.f * m_Scale }, m_Scale, m_Scale);
		}
		break;
	case Character::ActionState::walking:
		DrawWalkingState();
		break;
	}
}
void Body::Update(float elapsedSec)
{
	switch (m_ActionState)
	{
	case Character::ActionState::waiting:
		m_pSpriteBody->SetActFrame(0);
		for (std::pair<std::string, DoubleSprite*> pElement : m_pCostumes)
		{
			pElement.second->SetActFrame(0);
		}
		break;
	case Character::ActionState::walking:
		m_pSpriteBody->Update(elapsedSec);
		for (std::pair<std::string, DoubleSprite*> pElement : m_pCostumes)
		{
			pElement.second->Update(elapsedSec);
		}
		break;
	case Character::ActionState::looking:
		break;
	case Character::ActionState::shooting:
		break;
	default:
		break;
	}
}

void Body::SetPosition(const Point2f& position)
{
	m_Shape.left = position.x;
	m_Shape.bottom = position.y;
}
void Body::SetActionState(const Character::ActionState actionState)
{
	m_ActionState = actionState;
}
void Body::SetDirection(const Direction direction)
{
	if (m_Direction == direction)
		return;

	m_Direction = direction;
	if (m_Direction == Direction::left || m_Direction == Direction::right)
	{
		m_pSpriteBody->SetActFrame(11);
		for (std::pair<std::string, DoubleSprite*> pElement : m_pCostumes)
		{
			pElement.second->SetActFrame(11);
		}
	}
	else
	{
		m_pSpriteBody->SetActFrame(0);
		for (std::pair<std::string,DoubleSprite*> pElement : m_pCostumes)
		{
			pElement.second->SetActFrame(0);
		}
	}
}
void Body::SetCostumeBody(std::string filename)
{
	m_pTextureCostume = Singleton::GetInstance()->GetTextureManager()->GetTexture(filename);
	
	std::pair<mapIt, bool> it{};

	it = m_pCostumes.emplace(filename, nullptr);
	if (it.second)
	{
		m_pCostumes[filename] = new DoubleSprite{ m_pTextureCostume, 8, 2, 10, 20, m_ClipSize, m_ClipSize, 0.f, m_ClipSize };
	}

}

std::pair< Character::ActionState, Direction> Body::GetActionStateAndDirection() const
{
	return std::make_pair(m_ActionState, m_Direction);
}

// --> HELPER FUNCTIONS

void Body::DrawWalkingState() const
{
	Point2f position{ m_Shape.left, m_Shape.bottom - 5.f * m_Scale };

	switch (m_Direction)
	{
	case Direction::up:
		DrawMirroredTexture(position);
		break;
	case Direction::down:
		m_pSpriteBody->Draw(position, m_Scale, m_Scale);
		for (std::pair<std::string, DoubleSprite*> pElement : m_pCostumes)
		{
			pElement.second->Draw(position, m_Scale, m_Scale);
		}
		break;
	case Direction::right:
		m_pSpriteBody->Draw(position, m_Scale, m_Scale);
		for (std::pair<std::string, DoubleSprite*> pElement : m_pCostumes)
		{
			pElement.second->Draw(position, m_Scale, m_Scale);
		}
		break;
	case Direction::left:
		DrawMirroredTexture(position);
		break;
	}
}
void Body::DrawMirroredTexture(const Point2f& pos) const
{
	Point2f newPos{ pos.x + m_Shape.width / 2, pos.y + m_Shape.height / 2 };
	glPushMatrix();

	glTranslatef(newPos.x, newPos.y, 0.f);
	glScalef(-1.f, 1.f, 1.f);
	glTranslatef(-newPos.x, -newPos.y, 0.f);

	m_pSpriteBody->Draw(pos, m_Scale, m_Scale);
	for (std::pair<std::string, DoubleSprite*> pElement : m_pCostumes)
	{
		pElement.second->Draw(pos, m_Scale, m_Scale);
	}

	glPopMatrix();
}