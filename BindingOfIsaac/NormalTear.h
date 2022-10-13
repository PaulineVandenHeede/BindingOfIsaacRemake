#pragma once
#include "Tear.h"
#include "Vector2f.h"

class NormalTear final : public Tear
{
public:
	NormalTear(const Point2f& position, Direction direction, bool isEven, Texture * pTexture, int range, float speed, float scale, TearManager::Type type, int damage = 1);
	virtual ~NormalTear() = default;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Room* pRoom) override;

private:
	// --> MEMBER VARIABLES
	Vector2f m_Velocity;

	float m_Speed;
	float m_Acceleration;

	Direction m_Direction;

	int m_Range;

	// --> HELPER FUNCTIONS
	bool OutOfRange() const;
};

