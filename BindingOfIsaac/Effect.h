#pragma once

class Texture;
class Sprite;

class Effect
{
public:

	// sprite
	explicit Effect(const std::string& fileName, const Point2f& pos,
		float clipWidth, float clipHeight, float offsetX, float offsetY, float scale, bool temporary = false, int nrCols = 1,
		int nrRows = 1, int numberOfFrames = 1, int framesPerSec = 10);

	~Effect();

	// rule of 5
	Effect(const Effect& other) = delete;
	Effect(Effect&& other) = delete;
	Effect& operator=(const Effect& other) = delete;
	Effect& operator=(Effect&& other) = delete;

	void Draw() const;
	void Update(float elapsedSec);

	bool ReadyToDelete();

private:
	Texture* m_pTexture;
	Sprite* m_pSprite;

	Point2f m_Position;
	Offset2f m_Offset;

	float m_ClipWidth;
	float m_ClipHeight;
	float m_Scale;

	bool m_Temporary;

	// --> HELPER FUNCTIONS
};

