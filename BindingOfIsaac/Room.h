#pragma once
#include <vector>
#include "Texture.h"
#include "Vector2f.h"
#include "Door.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "PowerUpManager.h"
#include "Stage.h"
#include "InterpolatedTear.h"

class RoomManager;

class Room
{
public:
	enum class State
	{
		none,
		closed,
		cleared
	};

	Room(const Point2f& pos, std::pair<int, int> gridPos, State State, float scale);
	Room(std::stringstream& info, float scale);
	virtual ~Room() = default;

	//RULE OF 5
	Room(const Room& other) = delete;
	Room(Room&& other) = default;
	Room& operator=(const Room& other) = delete;
	Room& operator=(Room&& other) = default;
	
	virtual void Draw(const RoomManager* pRoomManager) const = 0;

	virtual void Update(float elapsedSec, RoomManager* pRoomManager, Character* pActor, Camera* pCamera) = 0;

	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	void HandleCollisionMonster(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool HandleCollisionTears(Circlef& tearShape, Vector2f& tearVelocity) const;
	
	bool ChangeState(State previousState);

	State GetState() const;

	std::pair<int, int> GetGridPosition() const;
	Rectf GetShape() const;

protected:
	// --> MEMBER VARIABLES
	Texture* m_pTexture;

	Rectf m_Shape;
	Rectf m_SrcRect;

	Point2f m_Position;

	float m_ClipWidth;
	float m_ClipHeight;
	float m_Width;
	float m_Height;
	float m_WidthWall;
	float m_HeightWall;
	float m_Scale;

	State m_State;

	std::pair<int, int> m_GridPos;

	// --> HELPER FUNCTIONS
	void DrawRoom() const;
	void DrawRoomHelper() const;
	void DrawQuarterRoom(const Rectf& destRect, const Rectf& srcRect, int scaleX, int scaleY, int scaleZ) const;
	std::pair<int, int> StringToGridPos(const std::string& pos);
	State StringToState(const std::string& state);

	// --> INLINE FUNCTIONS
	/* 
		to hint the compiler that inline to include the method body into class
		compiler weet dat inline gewoon op de plek gebruikt gaat worden vanwaar de functie wordt opgeroepen
	*/
	inline std::string StateToString(const State state) const
	{
		std::string output{};
		switch (state)
		{
		case Room::State::none:
			output += "none";
			break;
		case Room::State::closed:
			output += "closed";
			break;
		case Room::State::cleared:
			output += "cleared";
			break;
		default:
			break;
		}

		return output;
	}
};