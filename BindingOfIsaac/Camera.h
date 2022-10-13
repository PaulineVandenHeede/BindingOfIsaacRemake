#pragma once
class Camera
{
public:
	Camera(float width, float height);

	void Update(float elapsedSec);

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void SetLevelBoundaries(const Point2f& pos);
	void Transform(const Rectf& target) const;
	void SetToZoomIn();

	bool GetZoomIn() const;

private:
	// --> MEMBER VARIABLES
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;

	float m_AnimTime;
	float m_MaxAnimTime;

	bool m_ZoomIn;

	// --> HELPER FUNCTIONS
	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
};