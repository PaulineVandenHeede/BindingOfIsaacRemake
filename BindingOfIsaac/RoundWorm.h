#pragma once
#include "TearManager.h"
#include "Monster.h"

class Texture;
class Sprite;
class InterpolatedTear;
class Room;

class RoundWorm final : public Monster
{
public:
	RoundWorm(const Point2f& pos, float scale, int maxHealth = 10, int health = 10, int damage = 1);
	virtual ~RoundWorm();

	// RULE OF 5
	RoundWorm(const RoundWorm& other) = delete;
	RoundWorm(RoundWorm&& other) = default;
	RoundWorm& operator=(const RoundWorm& other) = delete;
	RoundWorm& operator=(RoundWorm&& other) = default;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, Character* pActor, const Room* room) override;

	virtual void HitByTears(int damage) override;
	virtual bool ReadyToDelete() const override;
	
private:
	// --> MEMBER VARIABLES
	TearManager m_TearManager;

	Sprite* m_pSprite;
	Texture* m_pTexture;
	Texture* m_pTearTexture;

	float m_ScaleTear;
	float m_GapX, m_GapY;

	bool m_NewPosition;
	bool m_CreateTear;

	// --> HELPER FUCNTIONS
	Point2f RandomPosition(const Rectf& roomShape);
	bool TouchedByCharacter(const Rectf& actorShape);
};