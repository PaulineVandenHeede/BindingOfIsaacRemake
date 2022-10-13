#include "pch.h"
#include "Monster.h"
#include "Singleton.h"
#include "Tear.h"
#include "utils.h"
#include <iomanip>

Monster::Monster(const Point2f& pos, float scale, int maxHealth, int health, int damage)
	: m_HealthBar{ maxHealth, health, scale }
	, m_Scale{ scale }
	, m_ClipWidth{ 32.f }
	, m_ClipHeight{ 32.f }
	, m_IsHit{ false }
	, m_Damage{ damage }
{
	m_Shape = Rectf{ pos.x - (m_ClipWidth * m_Scale) / 2, pos.y - (m_ClipHeight * m_Scale) / 2, m_ClipWidth * m_Scale, m_ClipHeight * m_Scale };
}

Monster::Monster(std::stringstream& info, float scale, int damage)
	: m_HealthBar{ 0, 0, scale }
	, m_Scale{ scale }
	, m_ClipWidth{ 32.f }
	, m_ClipHeight{ 32.f }
	, m_IsHit{ false }
	, m_Damage{ damage }
{
	// pos
	std::string part{ utils::GetAttributeValue("Position", info.str()) };
	Point2f pos{};
	if (part != "")
		pos = utils::StringToPos(part);

	// health
	part = utils::GetAttributeValue("Health", info.str());
	std::pair<int, int> healthBar{};
	if (part != "")
		healthBar = utils::StringToHealthBar(part);

	m_HealthBar.IncreaseMaxHealth(healthBar.second);
	m_HealthBar.DoHealing(healthBar.first);

	m_IsHit = (m_HealthBar.GetHealth() == 0);

	m_Shape = Rectf{ pos.x - (m_ClipWidth * m_Scale) / 2, pos.y - (m_ClipHeight * m_Scale) / 2, m_ClipWidth * m_Scale, m_ClipHeight * m_Scale };
}

int Monster::GetDamage() const
{
	return m_Damage;
}

HealthBar& Monster::GetHealthBar()
{
	return m_HealthBar;
}

Rectf Monster::GetShape() const
{
	return m_Shape;
}


// --> HELPER FUNCTIONS
bool Monster::HitByTear(Tear* pTear)
{
	// don't do anything when already hit
	if (m_IsHit)
		return false;

	if (utils::IsOverlapping(m_Shape, pTear->GetShape()))
	{
		pTear->HitSomething(); // update tear
		return true;
	}

	return false;
}

bool Monster::TouchedByCharacter(const Rectf& actorShape) const
{
	if (m_IsHit)
		return false;

	return (utils::IsOverlapping(actorShape, m_Shape));
}

void Monster::AddEffect(const std::string& fileName, const Point2f & pos, float clipWidth, float clipHeigth, float offsetX, float offsetY, bool temporary, int nrCols, int nrRows, int numberOfFrames, int framesPerSec)
{
	Singleton::GetInstance()->GetEffectManager()->AddEffect(fileName, pos, clipWidth, clipHeigth, offsetX, offsetY, m_Scale, temporary, nrCols, nrRows, numberOfFrames, framesPerSec);
}

// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const Monster& monster)
{
	out << std::setfill(' ') << std::setw(15) << "" << "Position=\"" << monster.m_Shape.left << ',' << monster.m_Shape.bottom << "\"\n";
	out << std::setw(15) << "" << " HealthBar=\"" << monster.m_HealthBar << "\"\n";

	return out;
}

