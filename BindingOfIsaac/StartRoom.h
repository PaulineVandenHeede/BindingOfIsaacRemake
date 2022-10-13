#pragma once
#include "Vector2f.h"
#include "Room.h"
#include <vector>

class Room;
class Tear;
class RoomManager;
class Texture;


class StartRoom final : public Room
{
public:
	StartRoom(const Point2f& pos, std::pair<int,int> gridPos, State state, float scale);
	StartRoom(std::stringstream& info, float scale);
	~StartRoom();

	//RULE OF 5
	StartRoom(const StartRoom& other) = delete;
	StartRoom(StartRoom&& other) = default;
	StartRoom& operator= (const StartRoom& other) = delete;
	StartRoom& operator= (StartRoom&& other) = default;

	virtual void Draw(const RoomManager* pRoomManager) const override;
	virtual void Update(float elapsedSec, RoomManager* pRoomManager, Character* pActor, Camera* pCamera) override;

	friend std::ostream& operator<<(std::ostream& out, const StartRoom& room);

private:
	// --> MEMBER VARIABLES
	Texture* m_pControls;

	// --> HELPER FUNCTIONS
};

