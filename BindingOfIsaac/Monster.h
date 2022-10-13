#pragma once
#include "HealthBar.h"
#include <vector>

class TextureManager;
class Room;
class Tear;
class Character;

class Monster
{
public:
	Monster(const Point2f& pos, float scale, int maxHealth = 10, int health = 10, int damage = 1);
	Monster(std::stringstream& info, float scale, int damage = 1);
	virtual ~Monster() = default;

	virtual void Draw() const = 0; // pure virtual
	virtual void Update(float elapsedSec, Character* pActor, const Room* pRoom) = 0; // pure virtual

	virtual void HitByTears(int damage) = 0; // pure virtual
	virtual bool ReadyToDelete() const = 0; // pure virtual


	int GetDamage() const;
	HealthBar& GetHealthBar();
	Rectf GetShape() const;

	friend std::ostream& operator<<(std::ostream& out, const Monster& monster);

protected:
	// --> MEMBER VARIABLES
	HealthBar m_HealthBar;

	Rectf m_Shape;

	float m_Scale;
	float m_ClipWidth;
	float m_ClipHeight;

	bool m_IsHit;
	int m_Damage;

	// --> HELPER FUCNTIONS
	bool HitByTear(Tear* pTear);
	bool TouchedByCharacter(const Rectf& actorShape) const;

	void AddEffect(const std::string& fileName, const Point2f& pos, float clipWidth, float clipHeight, float offsetX, float offsetY, bool temporary = false, int nrCols = 1,
		int nrRows = 1, int numberOfFrames = 1, int framesPerSec = 10);
};