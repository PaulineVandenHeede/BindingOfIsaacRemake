#include "pch.h"
#include "PowerUp.h"
#include "Texture.h"
#include <iomanip>
#include <sstream>

PowerUp::PowerUp(const Point2f& pos, float scale)
	: m_ClipHeight{ 32.f }
	, m_ClipWidth{ 32.f }
	, m_Scale{ scale }
	, m_FileName{ "" }
	, m_pTexture{ nullptr }
	, m_IsHit{ false }
{
	m_Shape.width = m_ClipWidth * m_Scale;
	m_Shape.height = m_ClipHeight * m_Scale;
	m_Shape.left = pos.x - ((m_ClipWidth / 2) * m_Scale);
	m_Shape.bottom = pos.y - ((m_ClipHeight / 2) * m_Scale);
}

PowerUp::PowerUp(std::stringstream & info, float scale)
	: m_ClipHeight { 32.f }
	, m_ClipWidth{ 32.f }
	, m_Scale{ scale }
	, m_FileName{ "" }
	, m_pTexture{ nullptr }
	, m_IsHit{ false }
{
	std::string part{ utils::GetAttributeValue("Position", info.str())};
	Point2f pos{};
	pos = utils::StringToPos(part);

	m_Shape.width = m_ClipWidth * m_Scale;
	m_Shape.height = m_ClipHeight * m_Scale;
	// position
	m_Shape.left = pos.x - ((m_ClipWidth / 2) * m_Scale);
	m_Shape.bottom = pos.y - ((m_ClipHeight / 2) * m_Scale);
}

bool PowerUp::ReadyToDelete()
{
	return m_IsHit;
}

// --> HELPER FUNCTIONS
std::string PowerUp::TypeToString(TypePowerUp type) const
{
	std::string out{};
	switch (type)
	{
	case TypePowerUp::heart:
		out = "Heart";
		break;
	case TypePowerUp::belt:
		out = "Belt";
		break;
	case TypePowerUp::snack:
		out = "Snack";
		break;
	case TypePowerUp::poison:
		out = "Poison";
		break;
	case TypePowerUp::lemon:
		out = "Lemon";
		break;
	case TypePowerUp::end:
	default:
		break;
	}

	return out;
}

void PowerUp::Initialise()
{
	m_Shape.width -= (m_Offset.x * 2) * m_Scale;
	m_Shape.height -= (m_Offset.y * 2) * m_Scale;
	m_Shape.left += m_Offset.x * m_Scale;
	m_Shape.bottom += m_Offset.y * m_Scale;
}


// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const PowerUp& powerUp)
{
	out << std::setfill(' ') << std::setw(6) << "" << "<" << powerUp.TypeToString(powerUp.m_Type) << "\n";
	out << std::setw(6) << "" << " Position=\"" << powerUp.m_Shape.left << "," << powerUp.m_Shape.bottom << "\" \n";
	out << std::setw(6) << "" << "/" << powerUp.TypeToString(powerUp.m_Type) << ">\n";

	return out;
}