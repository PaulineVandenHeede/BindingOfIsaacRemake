#include "pch.h"
#include "Sprite.h"
#include "structs.h"
#include "Texture.h"
#include <iostream>

// constructor
Sprite::Sprite(const Texture* texture, int nrCols, int nrRows, int numberOfFrames, float clipWidth, float clipHeight, float offsetX, float offsetY, int framesPerSec)
	: m_pTexture{ texture }
	, m_ClipWidth{ clipWidth }
	, m_ClipHeight{ clipHeight }
	, m_OffsetX{ offsetX }
	, m_OffsetY{ offsetY }
	, m_AnimTime{ 0.f }
	, m_Cols{ nrCols }
	, m_Rows{ nrRows }
	, m_NrOfFrames{ numberOfFrames }
	, m_NrFramesPerSec{ framesPerSec }
	, m_AnimFrame{ 0 }
	, m_NrOfCycles{ 0 }
{
}

// move constructor
//Sprite::Sprite(Sprite&& other) noexcept
//	: m_Cols{ other.m_Cols }
//	, m_Rows{ other.m_Rows }
//	, m_NrOfFrames{ other.m_NrOfFrames }
//	, m_ClipWidth{ other.m_ClipWidth }
//	, m_ClipHeight{ other.m_ClipHeight }
//	, m_NrFramesPerSec{ other.m_NrFramesPerSec }
//	, m_AnimFrame{ other.m_AnimFrame }
//	, m_AnimTime{ other.m_AnimTime }
//	, m_pTexture{ other.m_pTexture }
//{
//	// neutraal maken other
//	other.m_Cols = 0;
//	other.m_Rows = 0;
//	other.m_NrOfFrames = 0;
//	other.m_ClipWidth = 0;
//	other.m_ClipHeight = 0;
//	other.m_NrFramesPerSec = 0;
//	other.m_AnimFrame = 0;
//	other.m_AnimTime = 0;
//	other.m_pTexture = nullptr;
//}


// move assignment
//Sprite& Sprite::operator=(Sprite&& other) noexcept
//{	
//	if (&other == this) // prevent self assignment
//	{
//		return *this; // zichzelf returnen
//	}
//
//	delete m_pTexture;
//
//	// moven van other naar this
//	m_Cols = other.m_Cols;
//	m_Rows = other.m_Rows;
//	m_NrOfFrames = other.m_NrOfFrames;
//	m_ClipWidth = other.m_ClipWidth;
//	m_ClipHeight = other.m_ClipHeight;
//	m_NrFramesPerSec = other.m_NrFramesPerSec;
//	m_AnimFrame = other.m_AnimFrame;
//	m_AnimTime = other.m_AnimTime;
//	m_pTexture = other.m_pTexture;
//
//	// other neutraal maken
//	other.m_Cols = 0;
//	other.m_Rows = 0;
//	other.m_NrOfFrames = 0;
//	other.m_ClipWidth = 0;
//	other.m_ClipHeight = 0;
//	other.m_NrFramesPerSec = 0;
//	other.m_AnimFrame = 0;
//	other.m_AnimTime = 0;
//	other.m_pTexture = nullptr;
//
//	return *this; // zichzelf returnen
//}

void Sprite::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime > (1.f/m_NrFramesPerSec))
	{
		++m_AnimFrame;

		if(m_AnimFrame == m_NrOfFrames)
			++m_NrOfCycles %= INT_MAX; // no overflow

		m_AnimFrame %= m_NrOfFrames;
		m_AnimTime -= (1.f / m_NrFramesPerSec);
	}
}

void Sprite::Draw(const Point2f& pos, float scaleWidth, float scaleHeight) const
{
	Rectf srcRect{};
	srcRect.width = m_ClipWidth;
	srcRect.height = m_ClipHeight;
	srcRect.left = m_OffsetX + srcRect.width * (m_AnimFrame % m_Cols);
	srcRect.bottom = m_OffsetY + srcRect.height * (m_AnimFrame / m_Cols);

	
	Rectf destRect{};
	destRect.width = srcRect.width * scaleWidth;
	destRect.height = srcRect.height * scaleHeight;
	destRect.left = pos.x;
	destRect.bottom = pos.y;

	m_pTexture->Draw(destRect, srcRect);
}

float Sprite::GetClipWidth() const
{
	return m_ClipWidth;
}

float Sprite::GetClipHeight() const
{
	return m_ClipHeight;
}

int Sprite::GetActFrame() const
{
	return m_AnimFrame;
}
int Sprite::GetNrOfFrames() const
{
	return m_NrOfFrames;
}

int Sprite::GetNrOfCycles() const
{
	return m_NrOfCycles;
}
void Sprite::SetActFrame(int newActFrame)
{
	m_AnimFrame = newActFrame;
}

// --> HELPER FUNCTIONS

