#pragma once
#include "Vector2f.h"
#include "Stage.h"
#include "Sprite.h"
#include "InterpolatedTear.h"
#include "HealthBar.h"
#include "PowerUpManager.h"

class Texture;
class Body;
class Head;
class Tear;
class Room;
class Boss;
class Monster;
class ChargeBar;

class Character
{
public:
	// --> ENUMERATIONS
	enum class ActionState
	{
		waiting = 1,
		walking = 2,
		looking = 3,
		shooting = 4
	};

	Character(const Point2f& position, int maxHealth = 6, int health = 6);
	Character(std::stringstream& info);
	~Character();

	// RULE OF 5
	Character(const Character& other) = delete;
	Character(Character&& other) = delete;
	Character& operator=(const Character& other) = delete;
	Character& operator=(Character&& other) = delete;

	void Draw() const;
	void Update(float elapsedSec, const Room* room);

	void ChangeCollidedWithDoor();
	Rectf& GetShape();
	Vector2f& GetVelocity();
	HealthBar& GetHealthBar();
	ChargeBar* GetChargeBar();

	bool IsDead();

	void HitByObject(int damage);

	void ObjectHit(Monster* pMonster);
	void ObjectHit(Boss* pBoss);
	void PowerUpHit(const PowerUpInfo& powerUpInfo);

	void SetCostume(std::string filename, bool body, bool head);
	void SetPosition(Point2f& leftBottom);

	friend std::ostream& operator<<(std::ostream& out, const Character& character);

private:
	// --> MEMBER VARIABLES
	Texture* m_pTexture;
	Body* m_pBody;
	Head* m_pHead;
	ChargeBar* m_pChargeBar;

	HealthBar m_HealthBar;
	Rectf m_Shape;
	Vector2f m_Velocity;

	float m_Scale;
	float m_ClipSize;
	float m_Overlap;
	float m_AnimTime;
	float m_MaxAnimTime;
	float m_Speed;
	float m_Angle;
	float m_MaxAngle;

	ActionState m_ActionState;
	Direction m_Direction;

	bool m_CollidedWithDoor;
	bool m_IsHit;
	bool m_ShowCharacter;
	bool m_DrawHitTexture;
	bool m_DrawPickUpTexture;
	bool m_Dead;

	// --> HELPER FUNCTIONS
	void ChangeDirectionAndActionState(ActionState actionState, Direction direction);
	
	void ChangeDamage(int decrement);
	void ChangeTearRange(int decrement);
	void ChangeSpeed(float factor);
	void CreateChargeBar();
};
