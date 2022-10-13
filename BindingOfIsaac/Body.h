#pragma once
#include "Sprite.h"
#include "Character.h"
#include <unordered_map>

class DoubleSprite;

class Body
{
public:

	Body(const Point2f& position, const Texture* texture);
	~Body();

	// rule of 5
	Body(const Body& other) = default;
	Body(Body&& other) = default;
	Body& operator=(const Body& other) = default;
	Body& operator=(Body&& other) = default;

	void Draw() const;
	void Update(float elapsedSec);

	void SetPosition(const Point2f& position);
	void SetActionState(const Character::ActionState actionState);
	void SetDirection(const Direction direction);
	void SetCostumeBody(std::string filename);

	std::pair< Character::ActionState, Direction> GetActionStateAndDirection() const;
	
private:
	// --> MEMBER VARIABLES
	Sprite* m_pUpDownWalk;
	Sprite* m_pLeftRightWalk;
	DoubleSprite* m_pSpriteBody;
	std::unordered_map<std::string, DoubleSprite*> m_pCostumes;

	Texture* m_pTextureCostume;

	Rectf m_Shape;

	const float m_ClipSize;
	const float m_Scale;

	Character::ActionState m_ActionState;
	Direction m_Direction;

	// --> HELPER FUNCTIONS
	void DrawWalkingState() const;
	void DrawMirroredTexture(const Point2f& pos) const;
};