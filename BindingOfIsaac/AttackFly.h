#pragma once
#include "Vector2f.h"
#include "Monster.h"

class Sprite;
class Texture;
class Tear;

class AttackFly final : public Monster
{
public:
	enum class Type
	{
		normal,
		boss
	};
	AttackFly(const Point2f& pos, float scale, float speed, Type type = Type::normal, float radius = 0.f, int maxHealth = 5, int health = 5, int damage = 1);
	AttackFly(std::stringstream& info, float scale, Type type = Type::normal, float radius = 0.f, int damage = 1);

	virtual ~AttackFly();

	//RULE OF 5
	AttackFly(const AttackFly& other) = delete;
	AttackFly(AttackFly&& other) = default;
	AttackFly& operator=(const AttackFly& other) = delete;
	AttackFly& operator=(AttackFly&& other) = default;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, Character* pActor, const Room* room) override;

	void Draw(const Point2f& pos) const;

	virtual void HitByTears(int damage) override;
	virtual bool ReadyToDelete() const override;

	void SetType(Type type);

private:
	// --> MEMBER VARIABLES
	Sprite* m_pFly;
	Texture* m_pTexture;

	Vector2f m_Velocity;

	float m_ClipWidthDie;
	float m_ClipHeightDie;
	float m_Speed;
	float m_GapX, m_GapY;

	float m_Angle;
	const float m_RotSpeed;
	float m_Radius;

	Type m_Type;

	// --> HELPER FUNCTIONS
	void SetVelocity();
	void SetVelocity(const Rectf& actorShape);

	// void UpdateBossFly(float elapsedSec, const Rectf& actorShape);
	void DrawBossFly(const Point2f& centerPos) const;
};