#include "pch.h"
#include "NormalRoom.h"
#include "Singleton.h"
#include "MonsterManager.h"
#include "RoomManager.h"
#include "ScreenFlow.h"
#include "Texture.h"
#include <iomanip>


NormalRoom::NormalRoom(const Point2f& pos, std::pair<int,int> gridPos, State state, float scale) // scale == 2.5f
	: Room{ pos, gridPos, state, scale }
	, m_pMonsterManager{ nullptr }
	, m_PowerUpCreated{ false }
{
	//srcRect texture
	m_SrcRect = Rectf{ 8.f, m_ClipHeight, m_ClipWidth - 8.f, m_ClipHeight - 8.f }; 

	m_pMonsterManager = new MonsterManager{ m_Scale, m_Shape, true };
}

NormalRoom::NormalRoom(std::stringstream & info, float scale, bool rerun)
	: Room(info, scale)
	, m_pMonsterManager{ nullptr }
{
	m_SrcRect = Rectf{ 8.f, m_ClipHeight, m_ClipWidth - 8.f, m_ClipHeight - 8.f };

	// monstermanager
	std::stringstream part{};
	part << utils::GetPartValue("MonsterManager", info.str());

	if (rerun)
		m_State = State::none;

	// only load monsters when room is not cleared yet
	if (m_State != State::cleared)
		m_pMonsterManager = new MonsterManager{ part, m_Scale, m_Shape };
	else
		m_pMonsterManager = new MonsterManager{ m_Scale, m_Shape, false };

}


NormalRoom::~NormalRoom()
{
	delete m_pMonsterManager;
	m_pMonsterManager = nullptr;
}

void NormalRoom::Draw(const RoomManager* pRoomManager) const
{
	DrawRoom();

	pRoomManager->DrawDoors();

	Singleton::GetInstance()->GetEffectManager()->Draw();

	m_pMonsterManager->Draw();
	Singleton::GetInstance()->GetPowerUpManager()->Draw();
}
void NormalRoom::Update(float elapsedSec, RoomManager* pRoomManager, Character* pActor, Camera* pCamera)
{
	const Uint8 *pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_C])
	{
		m_pMonsterManager->DeleteAll();
	}

	if (m_State != Room::State::cleared && m_pMonsterManager->GetNrOfMonsters() == 0)
	{
		// only needs to happen once
		if (!m_PowerUpCreated)
		{
			// create power up
			Point2f pos{ m_Shape.left + (m_Shape.width / 2), m_Shape.bottom + (m_Shape.height / 2) };
			Singleton::GetInstance()->GetPowerUpManager()->AddItem(pos, m_Scale);
			m_PowerUpCreated = true;

			pRoomManager->ChangeDoorState(Door::State::open);

			// sound
			std::string fileName{ "Resources/Sounds/doors/door heavy open.wav" };
			Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, false);

		}

		// change doors
		m_State = State::cleared;
	}

	m_pMonsterManager->Update(elapsedSec, pActor, pRoomManager->GetActiveRoom());

	// check collision powerUp
	Singleton::GetInstance()->GetPowerUpManager()->Update(pActor, pCamera);
}

// -- > HELPER FUNCTIONS


// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const NormalRoom& room)
{
	out << std::setfill(' ') << std::setw(9) << "" << "<NormalRoom\n";

	out << std::setw(9) << "" << " GridPos=\"" << room.m_GridPos.first << ',' << room.m_GridPos.second << "\" \n";
	out << std::setw(9) << "" << " State=\"" << room.StateToString(room.m_State) << "\" \n";

	out << *room.m_pMonsterManager;
	
	out << std::setw(9) << "" << "/NormalRoom>\n";

	return out;
}
