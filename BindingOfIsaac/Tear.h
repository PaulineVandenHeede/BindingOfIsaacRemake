#pragma once
#include "Vector2f.h"
#include "TearManager.h"

class Texture;
class Sprite;
class Room;

class Tear
{
public:
	Tear(const Point2f& position, Texture* pTexture, TearManager::Type type, float scale, int damage = 1);
	virtual ~Tear(); 
	
	// rule of 5
	Tear(const Tear& other) = delete;
	Tear(Tear&& other) = default;
	Tear& operator=(const Tear& other) = delete;
	Tear& operator=(Tear&& otehr) = default;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, const Room* pRoom) = 0;

	bool IsOverlapping(const Rectf& actorShape) const;

	void HitSomething();
	bool ReadyToDelete() const;

	Circlef GetShape() const;
	int GetDamage() const;

protected:
	// --> MEMBER VARIABLES
	Texture* m_pTexture;
	Sprite* m_pTearSprite;

	Point2f m_BeginPos;
	Circlef m_Shape;

	float m_ClipSize;
	float m_Angle;
	float m_Scale;

	TearManager::Type m_Type;

	int m_RotatingSpeed;
	int m_Damage;

	bool m_IsHit;

	// --> HELPER FUNCTIONS
	void AddEffect() const;
};