#pragma once
class Texture;
struct Point2f;
#include <string>

class Sprite
{
public:
	// --> MEMBER FUNCTIONS/ METHODS
	Sprite(const Texture* texture, int nrCols, int nrRows, int numberOfFrames, float clipWidth, float clipHeight, float offsetX, float offsetY, int framesPerSec);
	~Sprite() = default;
	
	void Update(float elapsedSec);
	void Draw(const Point2f& pos, float scaleWidth = 1.f, float scaleHeight = 1.f) const;
	// default parameters: no scaling

	float GetClipWidth() const;
	float GetClipHeight() const;
	int GetActFrame() const;
	int GetNrOfFrames() const;
	int GetNrOfCycles() const;

	void SetActFrame(int newActFrame);
	
private:
	// --> MEMBER VARIABLES
	const Texture* m_pTexture;

	const float m_ClipHeight;
	const float m_ClipWidth;
	const float m_OffsetX;
	const float m_OffsetY;

	float m_AnimTime;

	const int m_Cols;
	const int m_Rows;
	const int m_NrOfFrames;
	const int m_NrFramesPerSec;

	int m_AnimFrame;
	int m_NrOfCycles;

	// --> HELPER FUNCTIONS
};