#pragma once
#include <vector>
#include "Vector2f.h"
#include "Room.h"

class Boss;
class Texture;
class Tear;
class RoomManager;

class BossRoom final : public Room
{
public:
	BossRoom(const Point2f& pos, std::pair<int, int> gridPos, State state, float scale);
	BossRoom(std::stringstream& info, float scale);
	~BossRoom();

	// rule of 5
	BossRoom(const BossRoom& other) = delete;
	BossRoom(BossRoom&& other) = default;
	BossRoom& operator=(const BossRoom& other) = delete;
	BossRoom& operator=(BossRoom&& other) = default;

	// needs to be defined because else it becomes an abstract class
	virtual void Draw(const RoomManager* pRoomManager) const override;
	virtual void Update(float elapsedSec, RoomManager* pRoomManager, Character* pActor, Camera* pCamera) override;

	// overloading of the two functions
	void Draw() const;
	void Update(float elapsedSec, Room* pRoom, Character* pActor);

	// reference because of chaining
	HealthBar& GetHealthBar();

	bool DidYouWin();

	friend std::ostream& operator<<(std::ostream& out, const BossRoom& room);

private:
	// --> MEMBER VARIABLES
	Boss* m_pBoss;

	// --> HELPER FUNCTIONS
};