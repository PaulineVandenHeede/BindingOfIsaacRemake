#pragma once

class Texture;

class ChargeBar
{
public:

	enum class State
	{
		empty = 0,
		quarter = 1,
		half = 2,
		threeQuarters = 3,
		full = 4
	};

	ChargeBar(float scale);
	ChargeBar(std::stringstream& info, float scale);

	void DrawHUD(const Point2f& pos) const;

	void Draw() const;
	void Update(float elapsedSec, const Rectf& actorShape);

	bool IsOverlapping(const Rectf& actorShape) const;

	void SetEmtpy();
	void SetFull();

	friend std::ostream& operator<<(std::ostream& out, const ChargeBar& chargeBar);

private:
	Texture* m_pTextureHUD;
	Texture* m_pLemon;
	Texture* m_pTexture;

	Circlef m_Shape;

	float m_ClipWidth;
	float m_ClipHeight;
	float m_Scale;

	float m_AnimTime;
	float m_MaxAnimTime;

	State m_State;

	bool m_Active;

	std::string StateToString() const;
	State StringToState(const std::string& info) const;
};

