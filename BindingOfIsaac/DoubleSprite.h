#pragma once
class Texture;
struct Point2f;
#include <string>

class DoubleSprite
{
public:
	// --> MEMBER FUNCTIONS/ METHODS
	DoubleSprite(const Texture* texture, int nrCols, int nrRows, int numberOfFrames, int numbeOfFramesTotal, float clipWidth, float clipHeight, float offsetX, float offsetY);
	// Sprite(const Sprite& other) = delete;
	// Sprite(Sprite&& other) noexcept;

	// Sprite& operator=(const Sprite& other) = delete;
	// Sprite& operator=(Sprite&& other) noexcept;

	void Update(float elapsedSec);
	void Draw(const Point2f& pos, float scaleWidth = 1.f, float scaleHeight = 1.f) const;
	// default parameters: no scaling

	float GetClipWidth() const;
	float GetClipHeight() const;

	void SetActFrame(int newActFrame);
	int GetActFrame() const;

private:
	// --> MEMBER VARIABLES
	const Texture* m_pTexture;

	const float m_ClipHeight;
	const float m_ClipWidth;
	const float m_OffsetX;
	const float m_OffsetYConst;
	float m_AnimTime;
	float m_OffsetY;

	const int m_Cols;
	const int m_Rows;
	const int m_NrOfFrames;
	const int m_NrOfFramesTotal;
	const int m_NrFramesPerSec;
	int m_AnimFrame;
	
	// --> HELPER FUNCTIONS
};

