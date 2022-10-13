#pragma once
#include <vector>
#include "Stage.h"

class Texture;
class Sprite;


class Door
{
public:
	enum class State
	{
		open,
		closed,
		locked,
		bombed
	};

	Door(const Point2f& pos, Direction direction);
	~Door();

	// RULE OF 5
	Door(const Door& other) = delete;
	Door(Door&& other) = default;
	Door& operator=(const Door& other) = delete;
	Door& operator=(Door&& other) = default;

	void Draw() const;
	void SetActionState(const Door::State state);
	void PrintDoorPosition() const;
	bool HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;

	Rectf GetShape() const;
	Direction GetDirection() const;
private:
	// --> MEMBER VARIABLES
	std::vector<Point2f> m_Vertices;

	Texture* m_pTexture;
	Sprite* m_pDoor;
	Sprite* m_pDoorFrame;

	Rectf m_Shape;
	Point2f m_Position;

	float m_Scale;

	Direction m_Direction;
	State m_State;

	// --> HELPER FUNCTIONS
	void DrawRotatedSprite(const Sprite* sprite, const Point2f & pos, float angle) const;
};