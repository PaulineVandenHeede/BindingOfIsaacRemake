#include "pch.h"
#include "ChargeBar.h"
#include "Singleton.h"
#include "Texture.h"
#include <iomanip>
#include <sstream>

ChargeBar::ChargeBar(float scale)
	: m_pTextureHUD{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/ui/ui_chargebar.png") }
	, m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/effects/effect_lemonparty.png") }
	, m_pLemon{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_056_lemonmishap.png") }
	, m_Scale{ scale }
	, m_AnimTime{ 0.f }
	, m_MaxAnimTime{ 3.f }
	, m_State{ State::empty }
	, m_Active{ false }

{
	m_ClipHeight = m_pTextureHUD->GetHeight() / 2;
	m_ClipWidth = m_pTextureHUD->GetWidth() / 4;

	m_Shape.radius = 157.f;
}

ChargeBar::ChargeBar(std::stringstream & info, float scale)
	: m_pTextureHUD{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/ui/ui_chargebar.png") }
	, m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/effects/effect_lemonparty.png") }
	, m_pLemon{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/items/collectibles_056_lemonmishap.png") }
	, m_Scale{ scale }
	, m_MaxAnimTime{ 3.f }
{
	m_ClipHeight = m_pTextureHUD->GetHeight() / 2;
	m_ClipWidth = m_pTextureHUD->GetWidth() / 4;

	m_Shape.radius = 157.f;

	// read info from file
		// state
	std::string part{};
	part = utils::GetAttributeValue("State", info.str());
	m_State = StringToState(part);

		// active
	part = utils::GetAttributeValue("Active", info.str());
	std::stringstream active{ part };
	active >> std::boolalpha >> m_Active;

		// animTime
	part = utils::GetAttributeValue("AnimTime", info.str());
	m_AnimTime = std::stof(part);
}

void ChargeBar::DrawHUD(const Point2f & pos) const
{
	Rectf srcRect{ 0.f, m_pLemon->GetHeight(), m_pLemon->GetWidth(), m_pLemon->GetHeight() };
	Rectf destRect{ pos.x , pos.y, srcRect.width * m_Scale, srcRect.height * m_Scale };

	m_pLemon->Draw(destRect, srcRect);

	srcRect = Rectf{ 0.f, m_ClipHeight, m_ClipWidth, m_ClipHeight };
	destRect = Rectf{ pos.x + (m_pLemon->GetWidth() * 0.8f)* m_Scale, pos.y, srcRect.width * m_Scale, srcRect.height * m_Scale };

	m_pTextureHUD->Draw(destRect, srcRect);

	switch (m_State)
	{
	case ChargeBar::State::empty:
		break;
	case ChargeBar::State::quarter:
		srcRect.left += m_ClipWidth;
		srcRect.height = 12.f;
		destRect.height = srcRect.height * m_Scale;

		m_pTextureHUD->Draw(destRect, srcRect);
		break;
	case ChargeBar::State::half:
		srcRect.left += m_ClipWidth;
		srcRect.height = 18.f;
		destRect.height = srcRect.height * m_Scale;

		m_pTextureHUD->Draw(destRect, srcRect);
		break;
	case ChargeBar::State::threeQuarters:
		srcRect.left += m_ClipWidth;
		srcRect.height = 24.f;
		destRect.height = srcRect.height * m_Scale;

		m_pTextureHUD->Draw(destRect, srcRect);
		break;
	case ChargeBar::State::full:
		srcRect.left += m_ClipWidth;
		srcRect.height = m_pTextureHUD->GetHeight() / 2;
		destRect.height = srcRect.height * m_Scale;

		m_pTextureHUD->Draw(destRect, srcRect);
		break;
	default:
		break;
	}

	srcRect = Rectf{ 0.f, m_ClipHeight * 2, m_ClipWidth, m_ClipHeight };
	destRect = Rectf{ pos.x + (m_pLemon->GetWidth() * 0.8f)* m_Scale, pos.y, srcRect.width * m_Scale, srcRect.height * m_Scale };

	m_pTextureHUD->Draw(destRect, srcRect);
}

void ChargeBar::Update(float elapsedSec, const Rectf& actorShape)
{
	m_AnimTime += elapsedSec;

	const Uint8 *pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_Q])
	{
		if (m_State == State::full)
		{
			SetEmtpy();
			m_Active = true;

			m_Shape.center = Point2f{ actorShape.left + actorShape.width / 2, actorShape.bottom };
		}
	}

	if (m_AnimTime > (m_MaxAnimTime / 2))
	{
		m_Active = false;
	}

	if (m_AnimTime > m_MaxAnimTime)
	{
		m_AnimTime -= m_MaxAnimTime;
		if (m_State == State::threeQuarters || m_State == State::full)
		{
			m_State = State::full;
		}
		else
		{
			m_State = State(int(m_State) + 1);
		}
	}
}

bool ChargeBar::IsOverlapping(const Rectf& actorShape) const
{
	return (m_Active && utils::IsOverlapping(actorShape, m_Shape));
}

void ChargeBar::Draw() const
{
	if (m_Active)
	{
		Point2f pos{ m_Shape.center.x - m_pTexture->GetWidth() / 2,  m_Shape.center.y - m_pTexture->GetHeight() / 2};

		m_pTexture->Draw(pos);
	}
}

void ChargeBar::SetEmtpy()
{
	m_State = State::empty;
	m_AnimTime = 0;
}

void ChargeBar::SetFull()
{
	m_State = State::full;
}

std::string ChargeBar::StateToString() const
{
	std::string state{};

	switch (m_State)
	{
	case State::empty:
		state = "empty";
		break;
	case State::quarter:
		state = "quarter";
		break;
	case State::half:
		state = "half";
		break;
	case State::threeQuarters:
		state = "threeQuarters";
		break;
	case State::full:
		state = "full";
		break;
	default:
		break;
	}

	return state;
}


ChargeBar::State ChargeBar::StringToState(const std::string& info) const
{
	State state{ State::empty };

	if (info == "empty")
		state = State::empty;
	else if (info == "quarter")
		state = State::quarter;
	else if (info == "half")
		state = State::half;
	else if (info == "threeQuarters")
		state = State::threeQuarters;
	else if (info == "full")
		state = State::full;

	return state;
}

std::ostream & operator<<(std::ostream & out, const ChargeBar & chargeBar)
{
	out << std::setfill(' ') << std::setw(6) << "" << "<ChargeBar\n";

	out << std::setw(6) << "" << " State=\"" << chargeBar.StateToString() << "\" \n";
	out << std::setw(6) << "" << " Active=\"" << std::boolalpha << chargeBar.m_Active << "\" \n";
	out << std::setw(6) << "" << " AnimTime=\"" << chargeBar.m_AnimTime << "\" \n";

	out << std::setw(6) << "" << "/ChargeBar>\n";
	return out;
}
