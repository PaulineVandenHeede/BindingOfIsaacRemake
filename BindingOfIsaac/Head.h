#pragma once
#include "Character.h"
#include "TearManager.h"
#include <unordered_map>

class Texture;
class Sprite;
class Monster;
class Boss;

class Head
{
public:
	Head(const Point2f& position, const Texture* pTexture, int damage = 3);
	~Head(); 
	
	// rule of 5
	Head(const Head& other) = default;
	Head(Head&& other) = default;
	Head& operator=(const Head& other) = default;
	Head& operator=(Head&& other) = default;

	void Draw() const;
	void DrawTears() const;
	void Update(float elapsedSec, const Room* pRoom);

	void ObjectHit(Monster* pMonster);
	void ObjectHit(Boss* pBoss);

	void SetPosition(const Point2f& position);
	void SetActionState(const Character::ActionState actionState);
	void SetDirection(const Direction direction);
	void SetCostumeHead(std::string fileName);

	void ChangeDamage(int decrement);
	void ChangeTearRange(int decrement);

private:
	// --> MEMBER VARIABLES
	Rectf m_Shape;

	Sprite* m_pHead;
	Texture * m_pTextureCostume;
	std::unordered_map<std::string,Sprite*> m_pCostumes;

	Offset2f m_TearOffset;

	Character::ActionState m_ActionState;
	Direction m_Direction;

	TearManager m_TearManager;

	const float m_ClipSize;
	const float m_Scale;

	const int m_NrFramesPerSec;
	float m_AnimTime;
	int m_Damage;

	// --> HELPER FUNCTIONS
	void HandleKeyboardInput(Direction direction);
	void DrawMirrroredHead(const Point2f& pos) const;
	void SetActFrames(int nrOfFrame) const;

};

