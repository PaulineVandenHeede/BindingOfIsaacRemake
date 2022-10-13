#pragma once
#include "Texture.h"

class DoubleSprite;
class Sprite;

class HealthBar
{
public:
	enum class Type
	{
		normal = 0,
		boss = 1
	};

	HealthBar(int maxHealth, int health, float scale, Type type = Type::normal);
	~HealthBar();

	// rule of 5
	HealthBar(const HealthBar& other) = delete;
	HealthBar(HealthBar&& other) = delete;
	HealthBar& operator=(const HealthBar& other) = delete;
	HealthBar& operator=(HealthBar&& other) = delete;

	void DoDamage(int damage);
	void DoHealing(int healing);
	void IncreaseMaxHealth(int increment);
	void DecreaseMaxHealth(int decrement);

	void Draw(const Point2f& pos) const;
	void Update(float elapsedSec);

	void DrawBoss(const Point2f& pos) const;
	
	int GetHealth() const;
	int GetMaxHealth() const;

	friend std::ostream& operator<<(std::ostream& out, const HealthBar& hb);

private:
	Texture* m_pTexture;
	Texture* m_pGlowTexture;
	Sprite* m_pGlow;
	DoubleSprite* m_pHeart;

	float m_Scale;
	float m_OriginalScale;
	float m_MaxScale;
	float m_AnimTime;
	float m_AnimTimeSound;
	float m_MaxAnimTime;

	Type m_Type;

	int m_MaxHealth;
	int m_Health;

	bool m_Shrink;
	bool m_SoundInitialised;

	// --> HELPER FUNCTIONS
};