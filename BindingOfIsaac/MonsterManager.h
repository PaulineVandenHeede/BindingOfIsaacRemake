#pragma once
#include <vector>
#include "Stage.h"
#include <unordered_map>

class Tear;
class InterpolatedTear;
class Monster;
class RoomManager;

enum class TypeMonster
{
	fly = 0,
	gusher = 1,
	roundWorm = 2,
	none = 3
};

class MonsterManager
{
public:
	MonsterManager(float scale, const Rectf& roomBoundaries, bool initialiseMonsters);
	MonsterManager(std::stringstream& info, float scale, const Rectf& roomBoundaries);
	~MonsterManager(); 
	
	// rule of 5
	MonsterManager(const MonsterManager& other) = delete;
	MonsterManager(MonsterManager&& other) = delete;
	MonsterManager& operator=(const MonsterManager& other) = delete;
	MonsterManager& operator=(MonsterManager&& other) = delete;

	void Draw() const;
	void Update(float elapsedSec, Character* pActor, const Room* pRoom);
	void DeleteWhenDead();
	void DeleteAll();
	void AddMonsters(const Rectf& roomBoundaries);
	void AddFlies(const Point2f& pos, int howMany);
	int GetNrOfMonsters() const;
	
	inline std::string ToString(TypeMonster v) const;

	friend std::ostream& operator<<(std::ostream& out, const MonsterManager& mm);

private:
	// --> MEMBER VARIABLES
	std::vector<std::pair<TypeMonster, Monster *>> m_pMonsters;
	std::unordered_map<TypeMonster, int> m_MonsterInfo;
	float m_Scale;

	// --> HELPER FUNCTIONS
	void AddAttackFlies(const Rectf& roomBoundaries);
	void AddAttackFlies(const Rectf& roomBoundaries, int howMany);

	void AddGushers(const Rectf& roomBoundaries);
	void AddGushers(const Rectf& roomBoundaries, int howMany);

	void AddRoundWorms(const Rectf& roomBoundaries);
	void AddRoundWorms(const Rectf& roomBoundaries, int howMany);

	TypeMonster StringToMonsterType(const std::string& type) const;
};

