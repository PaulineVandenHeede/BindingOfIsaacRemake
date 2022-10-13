#pragma once
#include "BossRoom.h"

class AttackFly;
class RoomManager;
class Tear;

class Boss
{
public:
	enum class ActionState
	{
		inhaling,
		blowing,
		happy,
		exhausted
	};

	Boss(const Point2f& pos, float scale, int maxHealth = 110, int health = 110);
	Boss(std::stringstream& info, float scale);
	~Boss();

	// RULE OF 5
	Boss(const Boss& other) = delete;
	Boss(Boss&& other) = delete;
	Boss& operator=(const Boss& other) = delete;
	Boss& operator=(Boss&& other) = delete;

	void Draw() const;
	void Update(float elapsedSec, Character* pActor, const Room* room);

	void HitByTears(int damage);

	Rectf GetShape() const;
	Vector2f GetVelocity() const;
	HealthBar& GetHealthBar();

	bool DidYouWin();

	friend std::ostream& operator<<(std::ostream& out, const Boss& boss);

private:
	MonsterManager* m_pMonsterManager;

	Texture* m_pTexture;

	HealthBar m_HealthBar;

	Rectf m_Shape;
	Vector2f m_Velocity;

	float m_ClipHeight;
	float m_ClipWidth;
	float m_Scale;
	float m_OffsetX;
	float m_OffsetY;
	float m_Speed;
	float m_Radius;
	float m_AnimTime;
	float m_MaxAnimTime;

	ActionState m_ActionState;
	int m_Damage;

	bool m_IsDead;

	// --> HELPER FUNCTIONS
	bool TouchedByCharacter(const Rectf& actorShape);

	Rectf determineSrcRect() const;
	void AttackOne();
	void AttackTwo();
	void AttackThree();
};