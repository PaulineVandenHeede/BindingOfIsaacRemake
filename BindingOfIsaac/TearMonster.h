#pragma once
#include "Stage.h"
#include "structs.h"
#include "Vector2f.h"

class Sprite;
class Texture;

class TearMonster
{
public:
	TearMonster(const Point2f& position, Direction direction, Texture * pTexture, int range);
	~TearMonster(); // --> RULE OF 5
	TearMonster& operator= (const TearMonster& other);

	bool OutOfRange() const;

	void Draw() const;
	void Update(float elapsedSec, const Stage& stage);
	bool TouchedGround();
	void HitMonster();

	Direction GetDirectionTear() const;
	Circlef GetShape() const;
	bool GetIsHit() const;

private:
	float m_Radius;
	// const int m_Range; --> TearManager
	Point2f m_OriginalPosition;
	Circlef m_Shape;
	Vector2f m_Velocity;
	float m_Speed;
	float m_Acceleration;

	Texture* m_pTexture;
	Sprite* m_pTearSprite;

	Direction m_Direction;

	const float m_ClipSize;
	const int m_NrOfFrames;

	float m_Angle;
	int m_RotatingSpeed;
	int m_Range;

	float m_Scale;

	bool m_IsHit;

	// --> HELPER FUNCTIONS
};