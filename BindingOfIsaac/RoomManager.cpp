#include "pch.h"
#include "RoomManager.h"
#include "NormalRoom.h"
#include "StartRoom.h"
#include "Room.h"
#include "Camera.h"
#include "Character.h"
#include "Singleton.h"
#include "ScreenFlow.h"
#include <algorithm>
#include <iomanip>

using MapIterator = std::unordered_map<Room*, std::vector<std::pair<Door*, Room*>>>::iterator;
using MapConstIterator = std::unordered_map<Room*, std::vector<std::pair<Door*, Room*>>>::const_iterator;

RoomManager::RoomManager(float scale, float roomWidth, float roomHeight)
	: m_Scale{ scale }
	, m_RoomWidth{ roomWidth }
	, m_RoomHeight{ roomHeight }
	, m_AllCleared{ false }
{

}

RoomManager::RoomManager(std::stringstream& info, bool allCleared, float scale, float roomWidth, float roomHeight, bool rerun)
	: RoomManager{ scale, roomWidth, roomHeight }
{
	m_AllCleared = allCleared;

	std::string part{};
	std::stringstream room{};
	// rooms
	while (!info.eof())
	{
		std::getline(info, part);
		std::string::iterator pos{};

		// delete spaces
		pos = std::remove_if(part.begin(), part.end(), [] (const char c) {
			return (c == ' ');
		});
		part.erase(pos, part.end());

		std::string tempInfo{};

		// we are sure that active room is first --> AddRoom instead of LoadRoom
		
		if (part == "<NormalRoom")
		{
			// read all info
			room.str(std::string{});
			room << utils::GetPartValue("NormalRoom", info.str());
			
			// initialise room
			Room* pRoom = new NormalRoom{ room, m_Scale, rerun };
			// add room to map
			AddRoom(pRoom);

			// shrink string
			std::string find{ "/NormalRoom>" };
			size_t pos = info.str().find(find);
			tempInfo = info.str().substr(pos + find.size());
			info.str(std::string{});
			info << tempInfo;
		}
		else if (part == "<StartRoom")
		{
			// clear string
			room.str(std::string{});

			room << utils::GetPartValue("StartRoom", info.str());

			Room* pRoom = new StartRoom{ room, m_Scale };
			// add room to map
			AddRoom(pRoom);

			// shrink string
			std::string find{ "/StartRoom>" };
			size_t pos = info.str().find(find);
			tempInfo = info.str().substr(pos + find.size());
			info.str(std::string{});
			info << tempInfo;
		}
		else if (part == "<BossRoom")
		{
			// shouldn't be possible 
			room.str() = utils::GetPartValue("BossRoom", info.str());

			// shrink string
			std::string find{ "/BossRoom>" };
			size_t pos = info.str().find(find);
			tempInfo = info.str().substr(pos + find.size());
			info.str(std::string{});
			info << tempInfo;
		}
	}

	// if rerun then active room is start room
	if (rerun)
	{
		SetActiveRoom(std::make_pair(4, 3));
	}
}

RoomManager::~RoomManager()
{
	for (std::pair<Room*, std::vector<std::pair<Door*, Room*>>> room : m_pDungeonComplex)
	{
		for (std::pair<Door*, Room*> door : room.second)
		{
			delete door.first;
			door.first = nullptr;
		}
		room.second.clear();

		delete room.first;
		room.first = nullptr;
	}
	m_pDungeonComplex.clear();
}

void RoomManager::AddRoom(Room* pCurrentRoom)
{
	if (m_pActiveRoom == nullptr)
	{
		m_pActiveRoom = pCurrentRoom;
	}

	if (m_pDungeonComplex.find(pCurrentRoom) == m_pDungeonComplex.end())
	{
		m_pDungeonComplex[pCurrentRoom] = std::vector<std::pair<Door*, Room*>>{};
	}
}
void RoomManager::AddDoor(Room* pCurrentRoom, Door* pCurrentDoor, Room* pAdjacentRoom)
{
	// sure here that currentRoom exists
	m_pDungeonComplex[pCurrentRoom].push_back(std::make_pair(pCurrentDoor, pAdjacentRoom));
}

void RoomManager::LoadRoom(Room* pRoom)
{
	if (m_pDungeonComplex.find(pRoom) == m_pDungeonComplex.end())
	{
		m_pDungeonComplex[pRoom] = std::vector<std::pair<Door*, Room*>>{};
	}
}

void RoomManager::SetActiveRoom(Room * pActiveRoom)
{
	m_pActiveRoom = pActiveRoom;
}

void RoomManager::FindAdjacentRooms()
{
	std::for_each(m_pDungeonComplex.begin(), m_pDungeonComplex.end(), [this] (const std::pair< Room*, std::vector<std::pair<Door*, Room*>>>& current)
	{
		std::pair<int, int> searchPos{ current.first->GetGridPosition() };
		searchPos.second -= 1;
		// up
		this->AdjacentRoom(current, searchPos, Direction::up);

		searchPos.second += 2;
		// down
		this->AdjacentRoom(current, searchPos, Direction::down);

		searchPos.second -= 1;
		searchPos.first -= 1;
		// left
		this->AdjacentRoom(current, searchPos, Direction::left);

		searchPos.first += 2;
		// right
		this->AdjacentRoom(current, searchPos, Direction::right);

	});
}
void RoomManager::AdjacentRoom(const std::pair< Room*, std::vector<std::pair<Door*, Room*>>>& current, const std::pair<int, int>& searchPos, Direction direction)
{
	MapConstIterator cit{};
	cit = FindAjacentRoom(searchPos); // no this pointer needed because this function is called with the this pointer

	if (cit != m_pDungeonComplex.cend())
	{
		// fill vector with door and pointer to adjacent roomPoint2f pos{ currentPos.first * m_RoomWidth, -currentPos.second * m_RoomHeight };
		Point2f pos{ PosDoor(direction, current.first->GetGridPosition()) };
		AddDoor(current.first, new Door{ pos, direction }, cit->first);
	}
}
MapConstIterator RoomManager::FindAjacentRoom(std::pair<int, int> searchPos)
{
	// find a room in the map that is the same as searchPos and return that iterator
	return std::find_if(m_pDungeonComplex.cbegin(), m_pDungeonComplex.cend(), [searchPos] (const std::pair< Room*, std::vector<std::pair<Door*, Room*>>>& other)
	{
		return (searchPos == other.first->GetGridPosition());
	});
}
Point2f RoomManager::PosDoor(Direction direction, std::pair<int,int> currentPos)
{
	float doorWidth{ 64.f * m_Scale };
	float doorHeight{ 48.f * m_Scale };
	float doorOffset{ 5.f * m_Scale };

	Point2f pos{ currentPos.first * m_RoomWidth, -currentPos.second * m_RoomHeight };

	switch (direction)
	{
	case Direction::up:
		pos.x += m_RoomWidth / 2;
		pos.x -= doorWidth / 2;
		pos.y += m_RoomHeight;
		pos.y -= (doorOffset + doorHeight);
		return pos;
	case Direction::down:
		pos.x += m_RoomWidth / 2;
		pos.x -= doorWidth / 2;
		pos.y += doorOffset;
		return pos;
	case Direction::right:
		pos.x += m_RoomWidth;
		pos.x -= (doorOffset + doorHeight + 8.f * m_Scale);
		pos.y += m_RoomHeight / 2;
		pos.y -= doorWidth / 2;
		return pos;
	case Direction::left:
		pos.x -= 2.f * m_Scale;
		pos.y += m_RoomHeight / 2;
		pos.y -= doorWidth / 2;
		return pos;
	}
	return pos;
}

bool RoomManager::FindRoom(const std::pair<int,int>& searchPos)
{
	return (std::find_if(m_pDungeonComplex.cbegin(), m_pDungeonComplex.cend(), [searchPos] (const std::pair< Room*, std::vector<std::pair<Door*, Room*>>>& current)
	{
		return (current.first != nullptr && current.first->GetGridPosition() == searchPos);
	}) != m_pDungeonComplex.cend());

	// if it's at end it didn't found the room so it returns false
}

void RoomManager::Draw() const
{
	// only active room gets drawn
	m_pActiveRoom->Draw(this);
}

void RoomManager::DrawDoors() const
{
	// map: draw doors
	for (std::pair<Door*, Room*> pair : m_pDungeonComplex.at(m_pActiveRoom))
	{
		pair.first->Draw();
	}
}

void RoomManager::HandleCollisionDoors(Character* pCharacter, Camera* pCamera)
{
	for (std::pair<Door*, Room*> pPair : m_pDungeonComplex.at(m_pActiveRoom))
	{
		if (pPair.first->HandleCollision(pCharacter->GetShape(), pCharacter->GetVelocity()))
		{
			// set adjacent room to active room
			m_pActiveRoom = pPair.second;

		
			if (m_pActiveRoom->GetState() != Room::State::cleared)
			{
				// set active room to closed
				m_pActiveRoom->ChangeState(Room::State::closed);

				// set doors to closed in new active room
				for (std::pair<Door*, Room*> pDoor : m_pDungeonComplex.at(m_pActiveRoom))
				{
					pDoor.first->SetActionState(Door::State::closed);
				}

				// sound
				std::string fileName{ "Resources/Sounds/doors/door heavy close.wav" };
				Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);

			}

			std::pair<int, int> pos = m_pActiveRoom->GetGridPosition();

			// set camera
			Rectf shape = m_pActiveRoom->GetShape();
			pCamera->SetLevelBoundaries(Point2f{ pos.first * m_RoomWidth, -pos.second * m_RoomHeight });

			pCharacter->ChangeCollidedWithDoor();
			return;
		}
	}
}

Room* RoomManager::GetActiveRoom() const
{
	return m_pActiveRoom;
}
void RoomManager::ChangeDoorState(const Door::State state)
{
	std::for_each(m_pDungeonComplex[m_pActiveRoom].cbegin(), m_pDungeonComplex[m_pActiveRoom].cend(), [state] (const std::pair<Door*, Room*>& adjacent)
	{
		adjacent.first->SetActionState(state);
	});
}

bool RoomManager::IsAllCleared()
{
	if (!m_AllCleared)
	{
		MapConstIterator cit{};
		cit = std::find_if(m_pDungeonComplex.cbegin(), m_pDungeonComplex.cend(), [] (const std::pair< Room*, std::vector<std::pair<Door*, Room*>>>& current)
		{
			return (current.first->GetState() != Room::State::cleared);
		});

		// if all rooms are cleared cit will be the cend iterator 
		if (cit == m_pDungeonComplex.cend())
		{
			m_AllCleared = true;
		}
	}
	return m_AllCleared;
}

MiniMap::Type RoomManager::UpdateMapRoom(const std::pair<int, int>& gridPos) const
{
	// find room
	MapConstIterator cit{};
	cit = std::find_if(m_pDungeonComplex.cbegin(), m_pDungeonComplex.cend(), [gridPos](const std::pair< Room*, std::vector<std::pair<Door*, Room*>>>& current)
	{
		return (current.first != nullptr && current.first->GetGridPosition() == gridPos);
	});

	// room doesn't exist
	if (cit == m_pDungeonComplex.cend())
		return MiniMap::Type::none;

	// active room?
	if (cit->first == m_pActiveRoom)
		return MiniMap::Type::active;

	// neighbour?
	MapConstIterator activeRoom{};
	activeRoom = m_pDungeonComplex.find(m_pActiveRoom);

	for (std::pair<Door*, Room*> pair : activeRoom->second)
	{
		if (pair.second->GetGridPosition() == gridPos)
			return MiniMap::Type::neighbour;
	}

	// room cleared?
	if (cit->first->GetState() == Room::State::cleared)
		return MiniMap::Type::cleared;

	// else room is not discovered yet
	return MiniMap::Type::none;
}
void RoomManager::InitialiseMap(MiniMap& miniMap)
{
	for (std::pair<Room*, std::vector<std::pair<Door*, Room*>>> pRoom : m_pDungeonComplex)
	{
		miniMap.AddRoom(pRoom.first->GetGridPosition());
	}
}

void RoomManager::SetActiveRoom(std::pair<int, int> gridPos)
{
	MapConstIterator cit = std::find_if(m_pDungeonComplex.cbegin(), m_pDungeonComplex.cend(), [gridPos] (const std::pair< Room*, std::vector<std::pair<Door*, Room*>>>& pElement) {
		return (pElement.first->GetGridPosition() == gridPos);
	});

	m_pActiveRoom = cit->first;
}

// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const RoomManager& rm)
{
	out << std::setfill(' ') << std::setw(6) << "" << "<RoomManager\n";

	// make sure active room is first
		// typecasten
	if (typeid(*rm.m_pActiveRoom) == typeid(NormalRoom))
	{
		out << *static_cast<NormalRoom*> (rm.m_pActiveRoom);
	}
	else if (typeid(*rm.m_pActiveRoom) == typeid(StartRoom))
	{
		out << *static_cast<StartRoom*> (rm.m_pActiveRoom);
	}

	for (std::pair<Room*, std::vector<std::pair<Door*, Room*>>> pRoom : rm.m_pDungeonComplex)
	{
		if (pRoom.first == rm.m_pActiveRoom)
			continue;
		// typecasten
		if (typeid(*pRoom.first) == typeid(NormalRoom))
		{
			out << *static_cast<NormalRoom*> (pRoom.first);
		}
		else if (typeid(*pRoom.first) == typeid(StartRoom))
		{
			out << *static_cast<StartRoom*> (pRoom.first);
		}
	}

	out << std::setw(6) << "" << "/RoomManager>\n";
	return out;
}
