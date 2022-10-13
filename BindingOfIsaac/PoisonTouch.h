#pragma once
#include "PowerUp.h"

class PoisonTouch final : public PowerUp
{
public:
	PoisonTouch(const Point2f& pos, float scale);
	PoisonTouch(std::stringstream& info, float scale);
	virtual ~PoisonTouch() = default;

	virtual void Draw() const override;
	virtual PowerUpInfo IsOverlapping(const Rectf& actorShape) override;

private:

};