#pragma once
#include <vector>
#include "Tear.h"
#include "Stage.h"

class TearManagerMonster
{
public:
	enum class Type
	{
		water,
		blood
	};

	TearManagerMonster(Type type, int range, float speed);
	~TearManagerMonster(); //--> RULE OF 5
	bool CreateTear(const Point2f& position, Direction direction);
	void CreateTearsInFourDirection(const Point2f& pos, bool diagonal);
	void Draw() const;
	void Update(float elapsedSec, const Stage& stage);
	void DeleteAllTears();
	bool GetIsEven() const;
	std::vector<Tear*> GetTears() const;

private:
	std::vector<Tear*> m_pTears;
	TextureManager* m_pTextureManager;
	Texture* m_pTexture;

	const float m_MaxTime;
	float m_Time;
	float m_Speed;

	Type m_Type;
	int m_Range;

	bool m_IsEven;
	bool m_IsHit;

	// --> HELPER FUNCTIONS
	void DeleteTearsOutOfRange();
	void DeleteTear(int index);

	void CreateTearsDiagonal(const Point2f& pos);
	void CreateTearsNonDiagonal(const Point2f& pos);

};


