#include "pch.h"
#include "StartRoom.h"
#include "RoomManager.h"
#include "Singleton.h"
#include "Texture.h"
#include <iomanip>


StartRoom::StartRoom(const Point2f& pos, std::pair<int, int> gridPos, State state, float scale)
	: Room{ pos, gridPos, state, scale }
	, m_pControls{ nullptr }
{
	m_SrcRect = Rectf{ 8.f, m_ClipHeight, m_ClipWidth - 8.f, m_ClipHeight - 8.f };

	m_pControls = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/backdrop/controls.png");
}

StartRoom::StartRoom(std::stringstream & info, float scale)
	: Room{ info, scale }
	, m_pControls{ nullptr }
{
	m_SrcRect = Rectf{ 8.f, m_ClipHeight, m_ClipWidth - 8.f, m_ClipHeight - 8.f };

	m_pControls = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/backdrop/controls.png");
}

StartRoom::~StartRoom()
{
	m_pControls = nullptr;
}

void StartRoom::Draw(const RoomManager* pRoomManager) const
{
	DrawRoom();

	float width{ m_Shape.width };
	float height{ 42.f * m_Scale * 2.f };
	float roomHeight{ m_Shape.height };
	Rectf destRect2{ m_Shape.left , m_Shape.bottom + roomHeight / 2 - 42.f * m_Scale, width ,  height };
	m_pControls->Draw(destRect2);

	pRoomManager->DrawDoors();

	Singleton::GetInstance()->GetEffectManager()->Draw();
}
void StartRoom::Update(float elapsedSec, RoomManager* pRoomManager, Character* pActor, Camera* pCamera)
{
}

// --> HELPER FUNCTIONS

// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const StartRoom& room)
{
	out << std::setfill(' ') << std::setw(9) << "" << "<StartRoom\n";

	out << std::setw(9) << "" << " GridPos=\"" << room.m_GridPos.first << ',' << room.m_GridPos.second << "\" \n";
	out << std::setw(9) << "" << " State=\"" << room.StateToString(room.m_State) << "\" \n";

	out << std::setw(9) << "" << "/StartRoom> \n";

	return out;
}
