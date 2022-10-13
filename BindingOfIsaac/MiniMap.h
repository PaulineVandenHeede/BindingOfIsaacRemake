#pragma once

class RoomManager;
class Texture;

class MiniMap
{
public:
	enum class Type
	{
		active = 0,
		neighbour = 1,
		cleared = 2,
		none = 3
	};

	MiniMap(float scale);
	~MiniMap();

	void Draw(const Point2f& pos) const;
	void UpdateMap(const RoomManager* rm);

	void AddRoom(std::pair<int, int> gridPos);

private:
	std::vector<std::pair<std::pair<int, int>, Type>> m_MiniMap;

	Texture* m_pTexture;

	float m_ClipWidthRoom;
	float m_ClipHeightRoom;
	float m_Scale;

	std::pair<int, int> m_Center;
};

