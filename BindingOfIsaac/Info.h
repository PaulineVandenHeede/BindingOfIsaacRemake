#pragma once
// #include "Texture.h"
class Texture;

class Info
{
public:
	Info(const Point2f& pos);
	~Info();

	void Draw() const;

private:
	std::string m_Instructions;
	std::string m_NameGame;
	std::string m_Name;

	Texture* m_pTextureInstructions;
	Texture* m_pTextureNameGame;
	Texture* m_pTextureName;

	Point2f m_Position;
};