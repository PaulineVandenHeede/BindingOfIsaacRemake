#pragma once
#include "PowerUp.h"

class Heart final : public PowerUp
{
public:
	Heart(const Point2f& pos, float scale);
	Heart(std::stringstream& info, float scale);
	virtual ~Heart() = default;

	virtual void Draw() const override;
	virtual PowerUpInfo IsOverlapping(const Rectf& actorShape) override;

private:
};