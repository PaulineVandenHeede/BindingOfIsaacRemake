#pragma once
#include "TearManager.h"
#include "Vector2f.h"
#include "Stage.h"
#include "Monster.h"

class Texture;
class Sprite;
class DoubleSprite;
class Tear;

class Gusher final : public Monster
{
public:
	Gusher(const Point2f& pos, float scale, int maxHealth = 10, int health = 10, int damage = 1);
	virtual ~Gusher();

	// rule of 5
	Gusher(const Gusher& other) = delete;
	Gusher(Gusher&& other) = default;
	Gusher& operator=(const Gusher& other) = delete;
	Gusher& operator=(Gusher&& other) = default;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, Character* pActor, const Room* pRoom) override;

	virtual void HitByTears(int damage) override;
	virtual bool ReadyToDelete() const override;
	
private:
	// --> MEMBER VARIABLES
	TearManager m_TearManager;

	DoubleSprite* m_pSpriteMonster;
	Sprite* m_pBloodSpatters;
	Texture* m_pTexture;

	Vector2f m_Velocity;

	const float m_MaxAnimTime;
	float m_Speed;
	float m_AnimTime;
	float m_GapX, m_GapY;

	Direction m_Direction;

	// --> HELPER FUNCTIONS
	void DrawMirroredTexture(const Point2f & pos) const;
	void SetVelocity();
};