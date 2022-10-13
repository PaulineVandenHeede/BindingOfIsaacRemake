#include "pch.h"
#include "MiniMap.h"
#include "RoomManager.h"
#include "Singleton.h"
#include "Texture.h"
#include <algorithm>


MiniMap::MiniMap(float scale)
	: m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/ui/ui_minimap.png") }
	, m_ClipWidthRoom{ 9.f }
	, m_ClipHeightRoom{ 8.f }
	, m_Scale{ scale }
	, m_Center{0,0}
{
}


MiniMap::~MiniMap()
{

}

void MiniMap::Draw(const Point2f& pos) const
{
	// draw square
	float size{55.f};
	Rectf srcRect{0.f, size, size, size};
	Rectf destRect{pos.x, pos.y, srcRect.width * m_Scale, srcRect.height * m_Scale};

	m_pTexture->Draw(destRect, srcRect);

	// draw rooms
	float midX{ 22.f };
	float midY{ 27.f };

	Point2f position{ pos.x + midX * m_Scale, pos.y + midY * m_Scale };

	for (std::pair<std::pair<int, int>, Type> room : m_MiniMap)
	{
		srcRect = Rectf{0.f, 63.f, m_ClipWidthRoom, m_ClipHeightRoom};

		switch (room.second)
		{
		case Type::active:
			break;
		case Type::neighbour:
			srcRect.bottom += m_ClipHeightRoom;
			break;
		case Type::cleared:
			srcRect.bottom += 2 * m_ClipHeightRoom;
			break;
		case Type::none: // skip these rooms
			continue;
		}

		destRect = Rectf{ position.x + ((room.first.first - m_Center.first) * m_ClipWidthRoom * m_Scale),
				position.y - ((room.first.second - m_Center.second)  * m_ClipHeightRoom * m_Scale),
				m_ClipWidthRoom * m_Scale,
				m_ClipHeightRoom * m_Scale};

		// check if in square --> not: then don't draw room
		Rectf square{ pos.x , pos.y + (8.f * m_Scale), (size - 2.f) * m_Scale, (size - 8.f) * m_Scale };

		if (utils::IsRectInRect(destRect, square))
		{
			m_pTexture->Draw(destRect, srcRect);
		}

	}
}

void MiniMap::UpdateMap(const RoomManager* rm)
{
	for (std::pair<std::pair<int, int>, Type>& room : m_MiniMap)
	{
		room.second = rm->UpdateMapRoom(room.first);

		if (room.second == Type::active)
		{
			m_Center = room.first;
		}
	}
}

void MiniMap::AddRoom(std::pair<int, int> gridPos)
{
	std::vector<std::pair<std::pair<int, int>, Type>>::const_iterator cit{};
	cit = std::find_if(m_MiniMap.cbegin(), m_MiniMap.cend(), [gridPos] (const std::pair<std::pair<int, int>, Type>& element) {
		return (element.first == gridPos);
	});

	if (cit == m_MiniMap.cend())
		m_MiniMap.push_back(std::make_pair(gridPos, Type::none));
}
