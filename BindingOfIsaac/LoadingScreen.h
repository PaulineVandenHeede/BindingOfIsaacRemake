#pragma once

class Texture;

class LoadingScreen
{
public:
	LoadingScreen();

	void Draw() const;

private:
	Texture* m_pTexture;
};

