#include "pch.h"
#include "Head.h"
#include <string>
#include "Singleton.h"

using mapIt = std::unordered_map<std::string, Sprite*>::iterator;
using mapCit = std::unordered_map<std::string, Sprite*>::const_iterator;

Head::Head(const Point2f& position, const Texture* texture, int damage)
	: m_pHead{ nullptr }
	, m_TearOffset{ 19.5f, 17.5f }
	, m_ClipSize{ 32.f }
	, m_Scale{ 3.f }
	, m_NrFramesPerSec{ 10 } 
	, m_Damage{ damage }
	, m_ActionState{ Character::ActionState::waiting }
	, m_Direction{ Direction::down }
	, m_TearManager{ TearManager::Type::water, 400, 500.f, 3 }
	, m_pCostumes{  }
{
	m_pHead = new Sprite{ texture, 6, 1, 6, m_ClipSize, m_ClipSize, 0, m_ClipSize, 10 };

	m_Shape.left = position.x;
	m_Shape.bottom = position.y - 5.f * m_Scale;

	m_Shape.width = m_ClipSize * m_Scale;
	m_Shape.height = m_ClipSize * m_Scale;

	m_AnimTime = 1.f / m_NrFramesPerSec;
}

Head::~Head()
{
	delete m_pHead;
	m_pHead = nullptr;

	for (std::pair<std::string,Sprite*> pElement : m_pCostumes)
	{
		delete pElement.second;
	}
	m_pCostumes.clear();

	m_pTextureCostume = nullptr;
}

void Head::Draw() const
{
	if (m_ActionState == Character::ActionState::walking || m_ActionState == Character::ActionState::looking)
	{
		if (m_Direction == Direction::right || m_Direction == Direction::left)
		{
			SetActFrames(2);
		}
		else if (m_Direction == Direction::up)
		{
			SetActFrames(4);
		}
		else if (m_Direction == Direction::down)
		{
			SetActFrames(0);
		}
	}
	else if (m_ActionState == Character::ActionState::waiting)
	{
		SetActFrames(0);
	}
	else if (m_ActionState == Character::ActionState::shooting)
	{
		if (m_Direction == Direction::right || m_Direction == Direction::left)
		{
			SetActFrames(3);
		}
		else if (m_Direction == Direction::up)
		{
			SetActFrames(5);
		}
		else if (m_Direction == Direction::down)
		{
			SetActFrames(1);
		}
	}

	Point2f position{ m_Shape.left, m_Shape.bottom - 5.f * m_Scale };
	switch (m_Direction)
	{
	case Direction::up:
		m_pHead->Draw(position, m_Scale, m_Scale);
		for (std::pair<std::string, Sprite*> pElement : m_pCostumes)
		{
			pElement.second->Draw(position, m_Scale, m_Scale);
		}
		break;
	case Direction::down:
		m_pHead->Draw(position, m_Scale, m_Scale);
		for (std::pair<std::string, Sprite*> pElement : m_pCostumes)
		{
			pElement.second->Draw(position, m_Scale, m_Scale);
		}
		break;
	case Direction::right:
		m_pHead->Draw(position, m_Scale, m_Scale);
		for (std::pair<std::string, Sprite*> pElement : m_pCostumes)
		{
			pElement.second->Draw(position, m_Scale, m_Scale);
		}
		break;
	case Direction::left:
		DrawMirrroredHead(position);
		break;
	}
}
void Head::DrawTears() const
{
	m_TearManager.Draw();
}
void Head::Update(float elapsedSec, const Room* room)
{
	const Uint8 *pStates = SDL_GetKeyboardState(nullptr);
	Point2f pos{};

	if (pStates[SDL_SCANCODE_UP])
	{
		m_Direction = Direction::up;
		m_TearOffset.y = 12.f * m_Scale;
		if (m_TearManager.GetIsEven())
			m_TearOffset.x = 9.f * m_Scale;
		else
			m_TearOffset.x = 23.f * m_Scale;
		HandleKeyboardInput(m_Direction);
	}
	else if (pStates[SDL_SCANCODE_DOWN])
	{
		m_Direction = Direction::down;
		m_TearOffset.y = 12.f * m_Scale;
		if (m_TearManager.GetIsEven())
			m_TearOffset.x = 9.f * m_Scale;
		else
			m_TearOffset.x = 23.f * m_Scale; HandleKeyboardInput(m_Direction);
	}
	else if (pStates[SDL_SCANCODE_LEFT])
	{
		m_Direction = Direction::left;
		m_TearOffset.x = 19.f * m_Scale;
		m_TearOffset.y = 13.f * m_Scale;
		HandleKeyboardInput(m_Direction);
	}
	else if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_Direction = Direction::right;
		m_TearOffset.x = 13.f * m_Scale;
		m_TearOffset.y = 13.f * m_Scale;
		HandleKeyboardInput(m_Direction);
	}

	if (m_ActionState == Character::ActionState::shooting)
	{
		m_AnimTime -= elapsedSec;
	}
	

	m_TearManager.Update(elapsedSec, room);
}

void Head::ObjectHit(Monster * pMonster)
{
	m_TearManager.HitObject(pMonster);
}

void Head::ObjectHit(Boss * pBoss)
{
	m_TearManager.HitObject(pBoss);
}

void Head::SetPosition(const Point2f& position)
{
	m_Shape.left = position.x;
	m_Shape.bottom = position.y;
}
void Head::SetActionState(const Character::ActionState actionState)
{
	m_ActionState = actionState;
}
void Head::SetDirection(const Direction direction)
{
	m_Direction = direction;
}
void Head::SetCostumeHead(std::string fileName)
{
	m_pTextureCostume = Singleton::GetInstance()->GetTextureManager()->GetTexture(fileName);

	std::pair<mapIt, bool> it{};

	it = m_pCostumes.emplace(fileName, nullptr);
	if (it.second)
	{
		m_pCostumes[fileName] = new Sprite{ m_pTextureCostume, 6, 1, 6, m_ClipSize, m_ClipSize, 0.f, m_ClipSize * 5, 10 };
	}
}

void Head::ChangeDamage(int decrement)
{
	m_TearManager.ChangeDamage(decrement);
}

void Head::ChangeTearRange(int decrement)
{
	m_TearManager.ChangeRange(decrement);
}

// --> HELPER FUNCTIONS
void Head::HandleKeyboardInput(Direction direction)
{
	if (m_TearManager.CreateTear(Point2f{ m_Shape.left + m_TearOffset.x , m_Shape.bottom + m_TearOffset.y }, direction, m_Damage) || m_AnimTime > 0 )
	{
		m_ActionState = Character::ActionState::shooting;
		if (m_AnimTime < 0)
		{
			m_AnimTime = 1.f / m_NrFramesPerSec;
		}
	}
	else
	{
		m_ActionState = Character::ActionState::looking;
		m_AnimTime = - 1.f / m_NrFramesPerSec; // negative number
	}
}
void Head::DrawMirrroredHead(const Point2f & pos) const
{
	glPushMatrix();

	glTranslatef(pos.x + m_Shape.width / 2, pos.y + m_Shape.height / 2, 0.f);
	glScalef(-1.f, 1.f, 1.f);
	glTranslatef(-pos.x - m_Shape.width / 2, -pos.y - m_Shape.height / 2, 0.f);

	m_pHead->Draw(pos, m_Scale, m_Scale);
	for (std::pair<std::string, Sprite*> pElement : m_pCostumes)
	{
		pElement.second->Draw(pos, m_Scale, m_Scale);
	}

	glPopMatrix();
}
void Head::SetActFrames(int nrOfFrame) const
{
	m_pHead->SetActFrame(nrOfFrame);
	for (std::pair<std::string, Sprite*> pElement : m_pCostumes)
	{
		pElement.second->SetActFrame(nrOfFrame);
	}
}
