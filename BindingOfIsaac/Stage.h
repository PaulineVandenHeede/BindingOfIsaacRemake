#pragma once
#include <vector>
#include "Vector2f.h"
#include <sstream>
#include "MiniMap.h"

class Room;
class BossRoom;
class Tear;
class Character;
class Hatch;
class RoomManager;
class Camera;

enum class Direction
{
	up,
	down,
	right,
	left
};

class Stage
{
public:
	// --> MEMBER FUNCTIONS
	Stage(int idStage = 1);
	Stage(std::stringstream& info, bool rerun);
	~Stage(); 
	
	// RULE OF FIVE
	Stage(const Stage& other) = delete;
	Stage(Stage&& other) = default;
	Stage& operator=(const Stage& other) = delete;
	Stage& operator=(Stage&& other) = default;

	void Draw(Character* pActor) const;
	void Update(float elapsedSec, Character* pActor, Camera* pCamera, MiniMap& miniMap);

	Room* GetActiveRoom() const;
	BossRoom* GetBossRoom() const;
	bool GetStageCleared() const;

	void SetHatch();
	bool HandleCollisionHatch(const Rectf& actorShape);
	void InitialiseMiniMap(MiniMap& miniMap);

	friend std::ostream& operator<<(std::ostream& out, const Stage& stage);

private:
	// --> MEMBER VARIABLES
	RoomManager* m_pRoomManager;
	BossRoom* m_pBossRoom;

	Hatch* m_pHatch;

	float m_RoomWidth;
	float m_RoomHeight;
	float m_Scale;
	float m_ScaleBoss;

	int m_IdStage;

	bool m_StageCleared;

	// --> HELPER FUNCTIONS
	void InitRooms(int cols, int rows);
	int GetNeighbours(int currentRoom, int cols, int rows);
	void ConnectRooms();

	void CreateBossRoom(Rectf& actorShape, Camera* camera);
};