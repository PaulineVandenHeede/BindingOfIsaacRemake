#pragma once
#include "PowerUp.h"

class Snack final : public PowerUp
{
public:
	Snack(const Point2f& pos, float scale);
	Snack(std::stringstream& info, float scale);
	virtual ~Snack() = default;

	virtual void Draw() const override;
	virtual PowerUpInfo IsOverlapping(const Rectf& actorShape) override;

private:


};