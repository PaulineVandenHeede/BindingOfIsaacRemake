#include "pch.h"
#include "HealthBar.h"
#include "Singleton.h"
#include "DoubleSprite.h"
#include "ScreenFlow.h"
#include "Sprite.h"

HealthBar::HealthBar(int maxHealth, int health, float scale, HealthBar::Type type)
	: m_pTexture{ nullptr }
	, m_pGlowTexture{ nullptr }
	, m_pGlow{ nullptr }
	, m_pHeart{ nullptr }
	, m_MaxHealth{ maxHealth }
	, m_Health{ health }
	, m_Scale{ scale }
	, m_OriginalScale{ scale }
	, m_MaxScale{ 3.f }
	, m_Type{ type }
	, m_AnimTime{ 0.f }
	, m_AnimTimeSound{ 0.f }
	, m_MaxAnimTime{ 0.5f }
	, m_Shrink{ false }
	, m_SoundInitialised{ false }
{
	switch (m_Type)
	{
	
	case Type::boss:
		m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/ui/ui_bosshealthbar.png");
		break;
	case Type::normal:
	default:
		m_pTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/ui/ui_hearts.png");
		m_pGlowTexture = Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/ui/hearts_glow.png");
		m_pHeart = new DoubleSprite{ m_pTexture, 3, 1, 3, 6, 16.f, 16.f, 0.f, 16.f };
		m_pGlow = new Sprite{ m_pGlowTexture, 2, 1,2,452.f, 296.f, 0.f, 296.f, 10 };
		break;
	}

	
}

HealthBar::~HealthBar()
{
	delete m_pGlow;
	m_pGlow = nullptr;

	delete m_pHeart;
	m_pHeart = nullptr;

	m_pTexture = nullptr;
	m_pGlowTexture = nullptr;
}

void HealthBar::DoDamage(int damage)
{
	m_Health -= damage;
}

void HealthBar::DoHealing(int healing)
{
	m_Health += healing;
	if (m_Health > m_MaxHealth)
		m_Health = m_MaxHealth;
}

void HealthBar::IncreaseMaxHealth(int increment)
{
	m_MaxHealth += increment;
}

void HealthBar::DecreaseMaxHealth(int decrement)
{
	m_MaxHealth -= decrement;
}

void HealthBar::Draw(const Point2f& pos) const
{
	Point2f newPos{ pos };
	if (m_pHeart == nullptr || m_pGlow == nullptr || m_pGlowTexture == nullptr)
		return;

	int lowHealth{ 2 };
	int increment{};
	if (m_Health <= lowHealth)
	{
		increment = 3;
		m_pGlow->Draw(Point2f{ 0.f,0.f }, m_OriginalScale, m_OriginalScale);
	}
	for (int i{ 2 }; i <= m_MaxHealth; i += 2)
	{
		
		Point2f position{newPos};

		if (m_Health >= i)
		{
			// draw whole heart
			m_pHeart->SetActFrame(increment);
			position.x -= m_pHeart->GetClipWidth() / 2 * m_Scale;
			m_pHeart->Draw( position , m_Scale, m_Scale);
		}
		if (m_Health == (i-1))
		{
			// draw half heart
			m_pHeart->SetActFrame(increment + 1);
			position.x -= m_pHeart->GetClipWidth() / 2 * m_Scale;
			m_pHeart->Draw(position, m_Scale, m_Scale);
		}
		if (m_Health < (i-1))
		{
			// draw empty heart
			if (m_Health <= lowHealth) // also let it move when almost dead
			{
				m_pHeart->SetActFrame(increment + 2);
				position.x -= m_pHeart->GetClipWidth() / 2 * m_Scale;
				m_pHeart->Draw(position, m_Scale, m_Scale);
			}
			else
			{
				m_pHeart->SetActFrame(increment + 2);
				position.x -= m_pHeart->GetClipWidth() / 2 * m_OriginalScale;
				m_pHeart->Draw(position, m_OriginalScale, m_OriginalScale);
			}
		}

		newPos.x += (m_pHeart->GetClipWidth() * m_MaxScale);
	}
}
void HealthBar::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	m_AnimTimeSound += elapsedSec;

	if (m_Shrink)
	{
		if (m_AnimTime > m_MaxAnimTime/2)
		{
			m_Scale = m_OriginalScale;
			m_Shrink = !m_Shrink;
			m_AnimTime -= m_MaxAnimTime;
			m_pGlow->SetActFrame(0);
		}
	}
	else
	{
		if (m_AnimTime > m_MaxAnimTime)
		{
			m_Scale = m_MaxScale;
			m_Shrink = !m_Shrink;
			m_AnimTime -= m_MaxAnimTime;
			m_pGlow->SetActFrame(1);

			
		}
	}

	if (!m_SoundInitialised)
	{
		// sound
		std::string fileName{ "Resources/Sounds/heart/heartbeat.wav" };
		Singleton::GetInstance()->GetEffectManager()->AddSoundEffect(fileName, true);


		m_SoundInitialised = true;
	}
}

void HealthBar::DrawBoss(const Point2f& pos) const
{
	Rectf srcRect{0.f, m_pTexture->GetHeight(), m_pTexture->GetWidth(), m_pTexture->GetHeight()/2};
	Rectf destRect{pos.x, pos.y, srcRect.width * m_Scale, srcRect.height * m_Scale};

	// draw bar
	m_pTexture->Draw(destRect, srcRect);

	// draw health
	if (m_Health > 0)
	{
		float width{ 128.f - 106.f + (106.f * (float(m_Health) / float(m_MaxHealth))) };
		srcRect = Rectf{ 0.f, m_pTexture->GetHeight() / 2, width, m_pTexture->GetHeight() / 2 };
		destRect = Rectf{ pos.x, pos.y, srcRect.width * m_Scale, srcRect.height * m_Scale };

		m_pTexture->Draw(destRect, srcRect);

		srcRect = Rectf{ 131.f, m_pTexture->GetHeight() / 2, 2.f, m_pTexture->GetHeight() / 2 };
		destRect.left += destRect.width - 1;
		destRect.width = srcRect.width * m_Scale;
		destRect.height = srcRect.height * m_Scale;

		m_pTexture->Draw(destRect, srcRect);
	}
	
}

int HealthBar::GetHealth() const
{
	return m_Health;
}

int HealthBar::GetMaxHealth() const
{
	return m_MaxHealth;
}

// --> OPERRATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const HealthBar& hb)
{
	out << hb.m_Health << "," << hb.m_MaxHealth;
	return out;
}