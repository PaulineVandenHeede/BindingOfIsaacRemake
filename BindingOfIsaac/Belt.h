#pragma once
#include "PowerUp.h"

class Belt final : public PowerUp
{
public:
	Belt(const Point2f& pos, float scale);
	Belt(std::stringstream& info, float scale);
	virtual ~Belt() = default;


	virtual void Draw() const override;
	virtual PowerUpInfo IsOverlapping(const Rectf& actorShape) override;

private:

};