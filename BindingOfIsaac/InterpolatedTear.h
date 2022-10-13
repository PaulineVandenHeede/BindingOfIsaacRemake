#pragma once
#include "Tear.h"

class InterpolatedTear final : public Tear
{
public:
	InterpolatedTear(const Point2f& beginPos, const Point2f& endPos, Texture* pTexture, float scale, TearManager::Type type, int damage = 1);
	virtual ~InterpolatedTear() = default;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Room* pRoom) override;

private:
	// --> MEMBER VARIABLES
	Point2f m_EndPoint;

	float m_LerpFactor;
	float m_DecrementFactor;

	// --> HELPER FUNCTIONS
	Point2f Lerp(const Point2f& beginPoint, const Point2f& endPoint, float lerpFactor) const;
};