#include "pch.h"
#include "Hatch.h"
#include "Singleton.h"
#include "Texture.h"
#include "utils.h"
#include <iomanip>
#include <sstream>

Hatch::Hatch(const Point2f& pos, float scale)
	: m_ClipWidth{ 64.f }
	, m_ClipHeight{ 64.f }
	, m_Scale{ scale }
	, m_OffsetX{ 16.f }
	, m_OffsetY{ 16.f }
{
	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/grid/door_11_trapdoor.png");
	
	m_Shape.left = pos.x + m_OffsetX * m_Scale - (m_ClipWidth * m_Scale) / 2;
	m_Shape.bottom = pos.y + m_OffsetY * m_Scale - (m_ClipHeight * m_Scale) / 2;
	m_Shape.width = m_ClipWidth * m_Scale - (m_OffsetX * 2 * m_Scale);
	m_Shape.height = m_ClipHeight * m_Scale - (m_OffsetY * 2 * m_Scale);
}

Hatch::Hatch(std::stringstream& info, float scale)
	: m_ClipWidth{ 64.f }
	, m_ClipHeight{ 64.f }
	, m_Scale{ scale }
	, m_OffsetX{ 16.f }
	, m_OffsetY{ 16.f }
{
	m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/grid/door_11_trapdoor.png");

	// get position
	std::string position{ utils::GetAttributeValue("Position", info.str()) };
	Point2f pos{};
	if (position != "")
		pos = utils::StringToPos(position);

	m_Shape.left = pos.x + m_OffsetX * m_Scale - (m_ClipWidth * m_Scale) / 2;
	m_Shape.bottom = pos.y + m_OffsetY * m_Scale - (m_ClipHeight * m_Scale) / 2;
	m_Shape.width = m_ClipWidth * m_Scale - (m_OffsetX * 2 * m_Scale);
	m_Shape.height = m_ClipHeight * m_Scale - (m_OffsetY * 2 * m_Scale);
}

Hatch::~Hatch()
{
	m_pTexture = nullptr;
}

void Hatch::Draw() const
{
	Rectf srcRect{0.f, m_ClipHeight, m_ClipWidth, m_ClipHeight};
	Rectf destRect{m_Shape.left - m_OffsetX * m_Scale, m_Shape.bottom - m_OffsetY * m_Scale, m_ClipWidth * m_Scale, m_ClipHeight * m_Scale};

	m_pTexture->Draw(destRect, srcRect);
}

bool Hatch::HandleCollision(const Rectf& actorShape)
{
	return utils::IsOverlapping(m_Shape, actorShape);
}

// --> OPERATOR OVERLOADING

std::ostream& operator<<(std::ostream& out, const Hatch& hatch)
{
	out << std::setfill(' ') << std::setw(6) << "" << "<Hatch\n";
	out << std::setw(6) << "" << " Position=\"" << hatch.m_Shape.left << ',' << hatch.m_Shape.bottom << "\" \n";
	out << std::setw(6) << "" << "/Hatch>\n";

	return out;
}
