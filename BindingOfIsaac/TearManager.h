#pragma once
#include <vector>
#include "Stage.h"

class Tear;
class Character;
class Monster;
class Boss;

class TearManager
{
public:
	enum class Type
	{
		water,
		blood
	};

	explicit TearManager(Type type, int range, float speed, int damage = 1);
	~TearManager(); 
	
	// RULE OF 5
	TearManager(const TearManager& other) = delete;
	TearManager(TearManager&& other) = delete;
	TearManager& operator=(const TearManager& other) = delete;
	TearManager& operator=(TearManager&& other) = delete;

	bool CreateTear(const Point2f& position, Direction direction, int damage);
	void CreateInterpolatedTear(const Point2f& beginPos, const Point2f& endPos, int damage);
	void CreateTearsInFourDirection(const Point2f& pos, int damage);

	void Draw() const;
	void Update(float elapsedSec, const Room* room);

	void DeleteAllTears();
	bool GetIsEven() const;
	int GetDamage() const;

	void ChangeRange(int decrement);
	void ChangeDamage(int decrement);

	void HitObject(Character* pActor);
	void HitObject(Monster* pMonster);
	void HitObject(Boss* pBoss);

private:
	std::vector<Tear*> m_pTears;
	Texture* m_pTexture;

	const float m_MaxTime;
	float m_Time;
	float m_Speed;
	float m_Scale;

	Type m_Type;

	int m_Range;
	int m_Damage;

	bool m_IsEven;
	bool m_IsHit;

	// --> HELPER FUNCTIONS
	void DeleteTearsOutOfRange();
	void DeleteTear(int index);
};


