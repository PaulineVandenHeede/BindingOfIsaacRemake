#pragma once
#include "PowerUp.h"

class Lemon final : public PowerUp
{
public:
	Lemon(const Point2f& pos, float scale);
	Lemon(std::stringstream& info, float scale);
	virtual ~Lemon() = default;

	virtual void Draw() const override;
	virtual PowerUpInfo IsOverlapping(const Rectf& actorShape) override;

private:

	// --> HELPER FUNCTIONS
};

