#pragma once
#include "Stage.h"
#include "PowerUpManager.h"
#include "Vector2f.h"
#include "Room.h"
#include <vector>

class Tear;
class InterpolatedTear;
class Room;
class Monster;
class RoomManager;
class MonsterManager;

class NormalRoom final : public Room
{
public:
	NormalRoom(const Point2f& pos, std::pair<int,int> gridPos, State state, float scale);
	NormalRoom(std::stringstream& info, float scale, bool rerun);
	virtual ~NormalRoom();

	//RULE OF 5
	NormalRoom(const NormalRoom& other) = delete;
	NormalRoom(NormalRoom&& other) = default;
	NormalRoom& operator=(const NormalRoom& other) = delete;
	NormalRoom& operator=(NormalRoom&& other) = default;

	virtual void Draw(const RoomManager* pRoomManager) const override;
	virtual void Update(float elapsedSec, RoomManager* pRoomManager, Character* pActor, Camera* pCamera) override;

	friend std::ostream& operator<<(std::ostream& out, const NormalRoom& room);

private:
	// --> MEMBER VARIABLES
	MonsterManager* m_pMonsterManager;

	Point2f m_Position;

	bool m_PowerUpCreated;

	// --> HELPER FUNCTIONS
};

