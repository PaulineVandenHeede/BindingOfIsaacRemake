#pragma once
class Texture;

class MenuScreen
{
public:
	enum class Option
	{
		newRun = 0,
		rerun = 1,
		continueRun = 2,
		options = 3,
		end = 4
	};

	MenuScreen(const float width, const float height);

	void Draw() const;
	Option ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

private:
	Texture* m_pMenu;

	float m_Height;
	float m_Width;
	float m_Scale;

	Option m_CurrentOption;
	const int m_MaxOptions;

};

