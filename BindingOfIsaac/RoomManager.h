#pragma once
#include "Stage.h"
#include "Door.h"
#include <unordered_map>
#include "MiniMap.h"


class Room;
// class Door;
class Camera;
class Character;

class RoomManager
{
public:
	using MapIterator = std::unordered_map<Room*, std::vector<std::pair<Door*, Room*>>>::iterator;
	using MapConstIterator = std::unordered_map<Room*, std::vector<std::pair<Door*, Room*>>>::const_iterator;

	RoomManager(float scale, float roomWidth, float roomHeight);
	RoomManager(std::stringstream& info, bool allCleared, float scale, float roomWidth, float roomHeight, bool rerun);
	~RoomManager();

	// rule of 5
	RoomManager(const RoomManager& other) = delete;
	RoomManager(RoomManager&& other) = delete;
	RoomManager& operator=(const RoomManager& other) = delete;
	RoomManager& operator=(RoomManager&& other) = delete;

	void AddRoom(Room* pCurrentRoom);
	void AddDoor(Room* pCurrentRoom, Door* pCurrentDoor, Room* pAdjacentRoom);

	// read from file
	void LoadRoom(Room* pRoom);
	void SetActiveRoom(Room* pActiveRoom);
	
	void FindAdjacentRooms();
	void AdjacentRoom(const std::pair< Room*, std::vector<std::pair<Door*, Room*>>>& current, const std::pair<int, int>& searchPos, Direction direction);
	MapConstIterator FindAjacentRoom(std::pair<int, int> searchPos);
	Point2f PosDoor(Direction direction, std::pair<int, int> currentPos);

	bool FindRoom(const std::pair<int,int>& searchPos);


	void Draw() const;
	void DrawDoors() const;

	void HandleCollisionDoors(Character* pCharacter, Camera* pCamera);

	Room* GetActiveRoom() const;
	void ChangeDoorState(const Door::State state);

	bool IsAllCleared();

	// minimap
	MiniMap::Type UpdateMapRoom(const std::pair<int, int>& gridPos) const;
	void InitialiseMap(MiniMap& miniMap);

	friend std::ostream& operator<<(std::ostream& out, const RoomManager& rm);

private:
	// --> MEMBER VARIABLES
	std::unordered_map<Room*, std::vector<std::pair<Door*, Room*>>> m_pDungeonComplex;
	Room* m_pActiveRoom;

	float m_RoomWidth;
	float m_RoomHeight;
	float m_Scale;

	bool m_AllCleared;

	//--> HELPER FUNCTIONS
	void SetActiveRoom(std::pair<int, int> gridPos);
};