#pragma once

class Texture;

class Hatch
{
public:
	Hatch(const Point2f& pos, float scale);
	Hatch(std::stringstream& info, float scale);
	~Hatch();

	//RULE OF 5
	Hatch(const Hatch& other) = delete;
	Hatch(Hatch&& other) = default;
	Hatch& operator=(const Hatch& other) = delete;
	Hatch& operator=(Hatch&& other) = default;

	void Draw() const;
	bool HandleCollision(const Rectf& actorShape);

	friend std::ostream& operator<<(std::ostream& out, const Hatch& hatch);

private:
	Texture* m_pTexture;

	Rectf m_Shape;

	float m_ClipWidth;
	float m_ClipHeight;
	float m_Scale;
	float m_OffsetX;
	float m_OffsetY;
};