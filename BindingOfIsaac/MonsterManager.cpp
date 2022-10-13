#include "pch.h"
#include "MonsterManager.h"
#include "Room.h"
#include "Monster.h"
#include "Gusher.h"
#include "RoundWorm.h"
#include "AttackFly.h"
#include "InterpolatedTear.h"
#include "Tear.h"
#include <iostream>
#include <iomanip>

using vectorPair = std::pair<TypeMonster, Monster*>;
using mapPair = std::pair<TypeMonster, int>;

MonsterManager::MonsterManager(float scale, const Rectf& roomBoundaries, bool intialiseMonsters)
	: m_Scale{ scale }
{
	if(intialiseMonsters)
		AddMonsters(roomBoundaries);
}

MonsterManager::MonsterManager(std::stringstream& info, float scale, const Rectf& roomBoundaries)
	: m_Scale{ scale }
{
	std::string monster{};
	TypeMonster type{};
	while (!info.eof())
	{
		std::getline(info, monster);
		type = StringToMonsterType(monster);

		// add to unordered map of monster info
		if (type != TypeMonster::none)
		{
			size_t pos = monster.find("=\"") + 2;
			size_t length = monster.find("\"", pos) - pos;
			std::string numberStr{ monster.substr(pos, length) };
			int number = std::stoi(numberStr);
			m_MonsterInfo[type] = number;
		}
	}

	// add monsters
	for (std::pair<TypeMonster, int> pair : m_MonsterInfo)
	{
		switch (pair.first)
		{
		case TypeMonster::fly:
			AddAttackFlies(roomBoundaries, pair.second);
			break;
		case TypeMonster::gusher:
			AddGushers(roomBoundaries, pair.second);
			break;
		case TypeMonster::roundWorm:
			AddRoundWorms(roomBoundaries, pair.second);
			break;
		default:
			break;
		}
	}
}

MonsterManager::~MonsterManager()
{
	std::cout << "MonsterManager destructor called\n";
	DeleteAll();
}

void MonsterManager::DeleteAll()
{
	for (size_t i{}; i < m_pMonsters.size(); ++i)
	{
		delete m_pMonsters[i].second;
	}
	m_pMonsters.clear();
}
void MonsterManager::Draw() const
{
	for (vectorPair monster : m_pMonsters)
	{
		monster.second->Draw();
	}
}
void MonsterManager::Update(float elapsedSec, Character* pActor, const Room* room)
{
	DeleteWhenDead();

	for (vectorPair monster : m_pMonsters)
	{
		monster.second->Update(elapsedSec, pActor, room);
	}
}

void MonsterManager::DeleteWhenDead()
{
	for (size_t i{}; i < m_pMonsters.size(); ++i)
	{
		if (m_pMonsters[i].second->ReadyToDelete())
		{
			delete m_pMonsters[i].second;
			std::swap(m_pMonsters[i], m_pMonsters.back());
			m_pMonsters.pop_back();
		}
	}
}
void MonsterManager::AddMonsters(const Rectf& roomBoundaries)
{
	int differentMonsters{ (rand() % 2) + 1 }; // how many sorts of monsters there are
	int previousMonster{ -1 }; // number that whichMonster can't be
	for (int i{}; i < differentMonsters; ++i)
	{
		TypeMonster whichMonster{TypeMonster::none};

		// choose another monster
		do {
			previousMonster = int (whichMonster);
			whichMonster = TypeMonster (rand() % 3);
		} while (int(whichMonster) == previousMonster); // so that you don't have 2 tiimes the same monster
		
		switch (whichMonster)
		{
		case TypeMonster::fly:
			AddAttackFlies(roomBoundaries);
			break;
		case TypeMonster::gusher:
			AddGushers(roomBoundaries);
			break;
		case TypeMonster::roundWorm:
			AddRoundWorms(roomBoundaries);
			break;
		}
	}
}
void MonsterManager::AddFlies(const Point2f & pos, int howMany)
{
	for (int i{}; i <= howMany; ++i)
	{
		m_pMonsters.push_back(std::make_pair(TypeMonster::fly, new AttackFly{ pos, 2.5f, 150.f, AttackFly::Type::boss, 50.f }));
		++m_MonsterInfo[TypeMonster::fly];
	}
}
int MonsterManager::GetNrOfMonsters() const
{
	return int(m_pMonsters.size());
}


// --> HELPER FUNCTIONS
void MonsterManager::AddAttackFlies(const Rectf& roomBoundaries)
{
	int nrOfFlies{ rand() % 6 + 1 };
	AddAttackFlies(roomBoundaries, nrOfFlies);
	m_MonsterInfo[TypeMonster::fly] = nrOfFlies;

}
void MonsterManager::AddAttackFlies(const Rectf& roomBoundaries, int howMany)
{
	for (int i{}; i < howMany; ++i)
	{
		Point2f pos{ roomBoundaries.left, roomBoundaries.bottom };
		float gapX{ (roomBoundaries.width) / 3 };
		float gapY{ (roomBoundaries.height) / 3 };

		pos.x += gapX + rand() % int(gapX);
		pos.y += gapY + rand() % int(gapY);

		m_pMonsters.push_back(std::make_pair(TypeMonster::fly, new AttackFly{ pos, m_Scale, 100.f }));
	}
}
void MonsterManager::AddGushers(const Rectf& roomBoundaries)
{
	int nrOfGushers{ rand() % 2 + 1 };
	AddGushers(roomBoundaries, nrOfGushers);
	m_MonsterInfo[TypeMonster::gusher] = nrOfGushers;
}
void MonsterManager::AddGushers(const Rectf & roomBoundaries, int howMany)
{
	for (int i{}; i < howMany; ++i)
	{
		Point2f pos{ roomBoundaries.left, roomBoundaries.bottom };
		float gapX{ (roomBoundaries.width) / 3 };
		float gapY{ (roomBoundaries.height) / 3 };

		pos.x += gapX + rand() % int(gapX);
		pos.y += gapY + rand() % int(gapY);

		m_pMonsters.push_back(std::make_pair(TypeMonster::gusher, new Gusher{ pos, m_Scale }));
	}
}
void MonsterManager::AddRoundWorms(const Rectf& roomBoundaries)
{
	int nrOfWorms{ rand() % 2 + 1 };
	AddRoundWorms(roomBoundaries, nrOfWorms);
	m_MonsterInfo[TypeMonster::roundWorm] = nrOfWorms;

}

void MonsterManager::AddRoundWorms(const Rectf & roomBoundaries, int howMany)
{
	for (int i{}; i < howMany; ++i)
	{

		Point2f pos{ roomBoundaries.left, roomBoundaries.bottom };
		float gapX{ (roomBoundaries.width) / 3 };
		float gapY{ (roomBoundaries.height) / 3 };

		pos.x += gapX + rand() % int(gapX);
		pos.y += gapY + rand() % int(gapY);

		m_pMonsters.push_back(std::make_pair(TypeMonster::roundWorm, new RoundWorm{ pos, m_Scale }));
	}
}

inline std::string MonsterManager::ToString(TypeMonster v) const
{
	switch (v)
	{
	case TypeMonster::fly: return "Fly";
	case TypeMonster::gusher: return "Gusher";
	case TypeMonster::roundWorm: return "RoundWorm";
	default: return "none";
	}
}
TypeMonster MonsterManager::StringToMonsterType(const std::string& type) const
{
	TypeMonster temp{};

	if (type.find("Fly=") != std::string::npos)
		temp = TypeMonster::fly;
	else if (type.find("Gusher=") != std::string::npos)
		temp = TypeMonster::gusher;
	else if (type.find("RoundWorm=") != std::string::npos)
		temp = TypeMonster::roundWorm;
	else
		temp = TypeMonster::none;

	return temp;
}

// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const MonsterManager& mm)
{
	out << std::setfill(' ') << std::setw(12) << "" << "<MonsterManager\n";
	for (mapPair monster : mm.m_MonsterInfo)
	{
		std::string monsterName = mm.ToString(monster.first);
		if (monsterName == "none")
			continue;
		out << std::setw(12) << "" << ' ' << monsterName << "=\"" << monster.second << "\" \n";
	}
	out << std::setw(12) << "" << "/MonsterManager>\n";
	return out;
}