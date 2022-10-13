#pragma once
#include "PowerUpManager.h"

class TextureManager;
class Texture;

class PowerUp
{
public:
	PowerUp(const Point2f& pos, float scale);
	PowerUp(std::stringstream& info, float scale);
	virtual ~PowerUp() = default;

	virtual void Draw() const = 0;
	virtual PowerUpInfo IsOverlapping(const Rectf& actorShape) = 0;
	
	bool ReadyToDelete();

	friend std::ostream& operator<<(std::ostream& out, const PowerUp& powerUp);

protected:
	Texture* m_pTexture;
	std::string m_FileName;
	Rectf m_Shape;

	Offset2f m_Offset;

	float m_ClipWidth;
	float m_ClipHeight;
	float m_Scale;

	TypePowerUp m_Type;

	bool m_IsHit;

	// --> HELPER FUNCTIONS
	std::string TypeToString(TypePowerUp type) const;
	void Initialise();
};