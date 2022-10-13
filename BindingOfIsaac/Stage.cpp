#include "pch.h"
#include "ScreenFlow.h"
#include "Game.h"
#include "Stage.h"
#include "Door.h"
#include "Room.h"
#include "StartRoom.h"
#include "NormalRoom.h"
#include "BossRoom.h"
#include "Character.h"
#include "Hatch.h"
#include "RoomManager.h"
#include "Singleton.h"
#include "Camera.h"
#include <iomanip>
#include <iostream>
#include <cmath>
#include <algorithm>

// --> MEMBER FUNCTIONS
Stage::Stage(int idStage)
	: m_pRoomManager{ nullptr }
	, m_pBossRoom{ nullptr }
	, m_pHatch{ nullptr }
	, m_Scale{ 2.5f }
	, m_ScaleBoss{ 3.f }
	, m_IdStage{ idStage }
	, m_StageCleared{ false }
{
	int increment{ 2 };
	int minCols{ 9 };
	int minRows{ 7 };

	int cols{ minCols + (m_IdStage - 1) * increment };
	int rows{ minRows + (m_IdStage - 1) * increment };

	m_RoomWidth = 226.f * m_Scale * 2.f;
	m_RoomHeight = 148.f * m_Scale * 2.f;

	m_pRoomManager = new RoomManager{ m_Scale, m_RoomWidth, m_RoomHeight };

	InitRooms(cols, rows);
	ConnectRooms();

}

Stage::Stage(std::stringstream& info, bool rerun)
	: m_Scale{ 2.5f }
	, m_ScaleBoss{ 3.f }
	, m_pBossRoom{ nullptr }
	, m_pHatch{ nullptr }
	, m_pRoomManager{ nullptr }
{
	m_RoomWidth = 226.f * m_Scale * 2.f;
	m_RoomHeight = 148.f * m_Scale * 2.f;

	std::string part{};
	std::stringstream cleared{};

	// id
	part = utils::GetAttributeValue("id", info.str());
	m_IdStage = std::stoi(part);

	// stage cleared
	cleared << utils::GetAttributeValue("stageCleared", info.str());
	cleared >> std::boolalpha >> m_StageCleared;

	
	part = utils::GetPartValue("BossRoom", info.str());

	if (part == "" || rerun)
	{
		// roomManager
		part = utils::GetPartValue("RoomManager", info.str());

		std::string hatch{ utils::GetPartValue("Hatch", info.str()) };
		// clear stringstream
		info.str(std::string{});

		info << part;

		m_pRoomManager = new RoomManager{ info, m_StageCleared , m_Scale, m_RoomWidth, m_RoomHeight, rerun };

		// hatch
		if (hatch != "")
		{
			info.str(std::string{});
			info << hatch;
			m_pHatch = new Hatch{ info, m_Scale };
		}

		ConnectRooms();
	}
	else
	{
		// clear stringstream
		info.str(std::string{});

		info << part;

		m_pBossRoom = new BossRoom{ info, m_ScaleBoss };
	}
}

Stage::~Stage()
{
	delete m_pRoomManager;
	m_pRoomManager = nullptr;

	delete m_pHatch;
	m_pHatch = nullptr;

	delete m_pBossRoom;
	m_pBossRoom = nullptr;
}

void Stage::Draw(Character* pActor) const
{
	if (m_pBossRoom != nullptr)
	{
		m_pBossRoom->Draw();
	}
	else
	{
		m_pRoomManager->Draw();
		
		if (m_pHatch != nullptr)
		{
			m_pHatch->Draw();
		}
	}

	pActor->Draw();
}
void Stage::Update(float elapsedSec, Character* pActor, Camera* pCamera, MiniMap& miniMap)
{
	if (m_pBossRoom != nullptr) // only do this when there is a bossroom
	{
		// update character
		pActor->Update(elapsedSec, m_pBossRoom);

		m_pBossRoom->Update(elapsedSec, m_pBossRoom, pActor);


		if (m_pBossRoom->DidYouWin())
		{
			ScreenFlow::m_CurrentScreen = ScreenFlow::ScreenTypes::won;
		}

		return; // don't do the rest if there's a bossroom
	}

	if (m_pHatch != nullptr) // only do this when there is a hatch
	{
		if (m_pHatch->HandleCollision(pActor->GetShape()))
		{
			CreateBossRoom(pActor->GetShape(), pCamera);
		}
	}

	Room* activeRoom{ m_pRoomManager->GetActiveRoom() };

	// change active room if collided with door
	m_pRoomManager->HandleCollisionDoors(pActor, pCamera);

	// update actor --> handle collision room
	pActor->Update(elapsedSec, activeRoom);

	activeRoom->Update(elapsedSec, m_pRoomManager, pActor, pCamera);

	if (m_pRoomManager->IsAllCleared())
	{
		SetHatch();
		m_StageCleared = true;
	}

	miniMap.UpdateMap(m_pRoomManager);
}

Room* Stage::GetActiveRoom() const
{
	return m_pRoomManager->GetActiveRoom();
}

BossRoom* Stage::GetBossRoom() const
{
	return m_pBossRoom;
}

bool Stage::GetStageCleared() const
{
	return m_StageCleared;
}

void Stage::SetHatch()
{
	if (m_pHatch != nullptr)
		return;
	std::pair<int, int> posGrid{ m_pRoomManager->GetActiveRoom()->GetGridPosition() };
	Point2f pos{ posGrid.first * m_RoomWidth, -posGrid.second * m_RoomHeight };
	m_pHatch = new Hatch{ Point2f{pos.x + m_RoomWidth/2, pos.y + m_RoomHeight/2}, m_Scale };
}
bool Stage::HandleCollisionHatch(const Rectf& actorShape)
{
	if (m_pHatch != nullptr)
	{
		return m_pHatch->HandleCollision(actorShape);
	}
	else
	{
		return false;
	}
}

void Stage::InitialiseMiniMap(MiniMap& miniMap)
{
	m_pRoomManager->InitialiseMap(miniMap);
}

// --> HELPER FUNCTIONS
void Stage::InitRooms(int cols, int rows)
{
	// declaration variables
	int maxWithNrOfNeighbours{ 2 };
	int nrOfWithNrOfNeighbours{ 0 };

	int maxRooms{ int(fmin(20, rand() % 2 + 5 + floor(m_IdStage * 10.f / 3))) }; // floor: downwards rounding
	
	int centerRoom = (rows / 2) * cols + cols / 2;

	int currentRoom = centerRoom;
	int currentRoomX{ centerRoom % cols };
	int currentRoomY{ centerRoom / cols };

	// start room
	Point2f pos{ currentRoomX * m_RoomWidth, -currentRoomY * m_RoomHeight };
	m_pRoomManager->AddRoom(new StartRoom{ pos, std::make_pair(currentRoomX, currentRoomY), Room::State::cleared, m_Scale});

	// normal rooms
	int i{};
	while (i < maxRooms)
	{
		int x = rand() % 4 + 1; // random direction

		if (x == 1) // down
		{
 			if ( (currentRoom + cols) < (cols * rows) ) // check if in grid
			{
				int newRoom{ currentRoom + cols };
				int newRoomX{ newRoom % cols };
				int newRoomY{ newRoom / cols };

				pos.y -= m_RoomHeight;

				if (!m_pRoomManager->FindRoom(std::make_pair(newRoomX, newRoomY))) // check if there's already a room
				{
					int neighbours = GetNeighbours(newRoom, cols, rows);
					
					if (neighbours == 2 && nrOfWithNrOfNeighbours < maxWithNrOfNeighbours)
					{
						m_pRoomManager->AddRoom(new NormalRoom{ pos, std::make_pair(newRoomX, newRoomY), Room::State::none, m_Scale });
						currentRoom = newRoom;
						++i;
						++nrOfWithNrOfNeighbours;
					}
					else if (neighbours < 2)
					{
						m_pRoomManager->AddRoom(new NormalRoom{ pos, std::make_pair(newRoomX, newRoomY), Room::State::none, m_Scale });
						currentRoom = newRoom;
						++i;
					}
					else
					{
						currentRoom = centerRoom;
						pos.x = (centerRoom % cols) * m_RoomWidth;
						pos.y =  -(centerRoom / cols) * m_RoomHeight;
					}
				}
				else
				{
					currentRoom = newRoom;
				}
			}
		}
		else if (x == 2) // up
		{
			if (currentRoom - cols > 0) // check if in grid
			{
				int newRoom{ currentRoom - cols };
				int newRoomY{ newRoom / cols };
				int newRoomX{ newRoom % cols };

				pos.y += m_RoomHeight;

				if (!m_pRoomManager->FindRoom(std::make_pair(newRoomX, newRoomY))) // check if there's already a room
				{
					int neighbours = GetNeighbours(newRoom, cols,rows);
					
					if (neighbours == 2 && (nrOfWithNrOfNeighbours < maxWithNrOfNeighbours) )
					{
						m_pRoomManager->AddRoom(new NormalRoom{ pos, std::make_pair(newRoomX, newRoomY), Room::State::none, m_Scale });
						currentRoom = newRoom;
						++i;
						++nrOfWithNrOfNeighbours;
					}
					else if (neighbours < 2)
					{
						m_pRoomManager->AddRoom(new NormalRoom{ pos, std::make_pair(newRoomX, newRoomY), Room::State::none, m_Scale });
						currentRoom =newRoom;
						++i;
					}
					else
					{
						currentRoom = centerRoom;
						pos.x = (centerRoom % cols) * m_RoomWidth;
						pos.y = -(centerRoom / cols) * m_RoomHeight;
					}
				}
				else
				{
					currentRoom = newRoom;
				}
			}

		}
		else if (x == 3) // left
		{
			if ((currentRoom - 1) % cols != cols - 1) // check if in grid
			{
				int newRoom{ currentRoom - 1};
				int newRoomY{ newRoom / cols };
				int newRoomX{ newRoom % cols };

				pos.x -= m_RoomWidth;

				if (!m_pRoomManager->FindRoom(std::make_pair(newRoomX, newRoomY))) // check if there's already a room
				{
					int neighbours = GetNeighbours(newRoom, cols, rows);
					
					if (neighbours == 2 && nrOfWithNrOfNeighbours < maxWithNrOfNeighbours)
					{
						m_pRoomManager->AddRoom(new NormalRoom{ pos, std::make_pair(newRoomX, newRoomY), Room::State::none, m_Scale });
						currentRoom = newRoom;
						++i;
						++nrOfWithNrOfNeighbours;
					}
					else if (neighbours < 2)
					{
						m_pRoomManager->AddRoom(new NormalRoom{ pos, std::make_pair(newRoomX, newRoomY), Room::State::none, m_Scale });
						currentRoom = newRoom;
						++i;
					}
					else
					{
						currentRoom = centerRoom;
						pos.x = (centerRoom % cols) * m_RoomWidth;
						pos.y = -(centerRoom / cols) * m_RoomHeight;
					}
				}
				else
				{
					--currentRoom;
				}
			}
		}
		else if (x == 4) // right
		{
			if ((currentRoom + 1) % cols != 0) // check if in grid
			{
				int newRoom{ currentRoom + 1 };
				int newRoomX{ newRoom % cols };
				int newRoomY{ newRoom / cols };

				pos.x += m_RoomWidth;

				if (!m_pRoomManager->FindRoom(std::make_pair(newRoomX, newRoomY))) // check if there's already a room
				{
					int neighbours = GetNeighbours(newRoom, cols, rows);
					
					if (neighbours == 2 && nrOfWithNrOfNeighbours < maxWithNrOfNeighbours)
					{
						m_pRoomManager->AddRoom(new NormalRoom{ pos, std::make_pair(newRoomX, newRoomY), Room::State::none, m_Scale });
						currentRoom = newRoom;
						++i;
						++nrOfWithNrOfNeighbours;
					}
					else if (neighbours < 2)
					{
						m_pRoomManager->AddRoom(new NormalRoom{ pos, std::make_pair(newRoomX, newRoomY), Room::State::none, m_Scale });
						currentRoom = newRoom;
						++i;
					}
					else
					{
						currentRoom = centerRoom;
						pos.x = (centerRoom % cols) * m_RoomWidth;
						pos.y = -(centerRoom / cols) * m_RoomHeight;
					}
				}
				else
				{
					++currentRoom;
				}
			}
		}
	}
}
int Stage::GetNeighbours(int currentRoom, int cols, int rows)
{
	int i{};
	// down
	if (currentRoom + cols < cols * rows) // check if in grid
	{
		int newRoomX{ (currentRoom + cols) % cols };
		int newRoomY{ (currentRoom + cols) / cols };

		if (m_pRoomManager->FindRoom(std::make_pair(newRoomX, newRoomY))) ++i; // check if there's a room
	}
	// up
	if (currentRoom - cols > 0) // check if in grid
	{
		int newRoomX{ (currentRoom - cols) % cols };
		int newRoomY{ (currentRoom - cols) / cols };

		if (m_pRoomManager->FindRoom(std::make_pair(newRoomX, newRoomY))) ++i; // check if there's a room
	}
	// right
	if ((currentRoom + 1) % cols != 0) // check if in grid
	{
		int newRoomX{ (currentRoom + 1) % cols };
		int newRoomY{ (currentRoom + 1) / cols };

		if (m_pRoomManager->FindRoom(std::make_pair(newRoomX, newRoomY))) ++i; // check if there's a room
	}
	// left
	if ((currentRoom - 1) % cols != cols - 1) // check if in grid
	{
		int newRoomX{ (currentRoom - 1) % cols };
		int newRoomY{ (currentRoom - 1) / cols };

		if (m_pRoomManager->FindRoom(std::make_pair(newRoomX, newRoomY))) ++i; // check if there's a room
	}

	return i;
}

void Stage::ConnectRooms()
{
	m_pRoomManager->FindAdjacentRooms();
}

void Stage::CreateBossRoom(Rectf& actorShape, Camera* camera)
{
	std::pair<int, int> gridPos{ 0 , 0 };
	Point2f pos{ 0.f, 0.f };

	m_pBossRoom = new BossRoom{ pos, gridPos, Room::State::closed, 3.f };

	// change camera
	camera->SetLevelBoundaries(Rectf{ pos, m_RoomWidth / m_Scale * m_ScaleBoss, m_RoomHeight / m_Scale * m_ScaleBoss });

	// change actor position
	pos.x += m_pBossRoom->GetShape().width / 2;
	pos.y += m_pBossRoom->GetShape().height / 2;
	actorShape.left = pos.x;
	actorShape.bottom = pos.y;

	// clear effects from stage
	Singleton::GetInstance()->GetEffectManager()->DeleteAllEffects();
}

// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const Stage& stage)
{
	out << std::setfill(' ') << std::setw(3) << "" << "<Stage\n";
	out << std::setw(3) << "" << "id=\"" << stage.m_IdStage << "\" \n";
	out << std::setw(3) << "" << "stageCleared=\"" << std::boolalpha << stage.m_StageCleared << "\" \n";

	if (stage.m_pRoomManager != nullptr)
	{
		out << *stage.m_pRoomManager;

		if (stage.m_pHatch != nullptr)
		{
			out << *stage.m_pHatch;
		}
	}

	if(stage.m_pBossRoom != nullptr)
	{
		out << *stage.m_pBossRoom;
	}

	out << std::setw(3) << "" << "/Stage>\n";
	return out;
}
