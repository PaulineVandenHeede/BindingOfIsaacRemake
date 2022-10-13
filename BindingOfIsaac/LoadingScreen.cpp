#include "pch.h"
#include "LoadingScreen.h"
#include "Singleton.h"
#include "Texture.h"

LoadingScreen::LoadingScreen()
	: m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture("Resources/Images/menu/loading_screen.png") }
{
}

void LoadingScreen::Draw() const
{
	glPushMatrix();

	glScalef(2.5f, 2.5f, 0);
	m_pTexture->Draw();

	glPopMatrix();
}
