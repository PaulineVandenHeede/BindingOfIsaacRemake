#include "pch.h"
#include "BossRoom.h"
#include "Boss.h"
#include "RoomManager.h"
#include "Texture.h"
#include "Singleton.h"
#include <iomanip>

BossRoom::BossRoom(const Point2f& pos, std::pair<int, int> gridPos, State state, float scale) // scale == 3.f
	: Room { pos, gridPos, state, scale }
{
	m_SrcRect = Rectf{ m_ClipWidth + 8.f, m_ClipHeight * 2, m_ClipWidth - 8.f, m_ClipHeight - 8.f };

	m_pBoss = new Boss{ Point2f{m_Shape.left + m_Shape.width / 3, m_Shape.bottom + m_Shape.height / 2}, m_Scale };
}

BossRoom::BossRoom(std::stringstream & info, float scale)
	: Room{ info, scale }
{
	m_SrcRect = Rectf{ m_ClipWidth + 8.f, m_ClipHeight * 2, m_ClipWidth - 8.f, m_ClipHeight - 8.f };

	// boss parser
	m_pBoss = new Boss{ info, scale };
}

BossRoom::~BossRoom()
{
	delete m_pBoss;
	m_pBoss = nullptr;
}
void BossRoom::Draw() const
{
	DrawRoom();

	Singleton::GetInstance()->GetEffectManager()->Draw();

	m_pBoss->Draw();
}

// needs to be defined because else this becomes an abstract class
void BossRoom::Draw(const RoomManager * pRoomManager) const
{
}
void BossRoom::Update(float elapsedSec, RoomManager* roomManager, Character* pActor, Camera* pcamera)
{
}

void BossRoom::Update(float elapsedSec, Room* pRoom, Character* pActor)
{
	m_pBoss->Update(elapsedSec, pActor, pRoom);

	if (m_pBoss->DidYouWin())
		m_State = State::cleared;

}


HealthBar & BossRoom::GetHealthBar()
{
	return m_pBoss->GetHealthBar();
}

bool BossRoom::DidYouWin()
{
	return (m_State == State::cleared);
}

// --> HELPER FUNCTIONS


// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const BossRoom& room)
{
	out << std::setfill(' ') << std::setw(6) << "" << "<BossRoom\n";

	out << std::setw(6) << "" << " GridPos=\"" << room.m_GridPos.first << ',' << room.m_GridPos.first << "\" \n";
	out << std::setw(6) << "" << " State=\"" << room.StateToString(room.m_State) << "\" \n";
	
	// boss
	out << *room.m_pBoss;

	out << std::setw(6) << "" << "/BossRoom>\n";

	return out;
}