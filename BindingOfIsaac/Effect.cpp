#include "pch.h"
#include "Effect.h"
#include "Texture.h"
#include "Sprite.h"
#include "Singleton.h"

Effect::Effect(const std::string& fileName, const Point2f& pos, 
	float clipWidth, float clipHeight, float offsetX, float offsetY, float scale, bool temporary, int nrCols,
	int nrRows, int numberOfFrames, int framesPerSec)

	: m_pTexture{ Singleton::GetInstance()->GetTextureManager()->GetTexture(fileName) }
	, m_pSprite{ nullptr }
	, m_Position{ pos }
	, m_Offset{ offsetX, offsetY }
	, m_ClipWidth{ clipWidth }
	, m_ClipHeight{ clipHeight }
	, m_Scale{ scale }
	, m_Temporary{ temporary }
{
	m_pSprite = new Sprite{ m_pTexture, nrCols, nrRows, numberOfFrames, clipWidth, clipHeight, offsetX, offsetY, framesPerSec };
}



Effect::~Effect()
{
	delete m_pSprite;
	m_pSprite = nullptr;

	m_pTexture = nullptr;
}

void Effect::Draw() const
{
	if (m_pSprite == nullptr)
		return;

	m_pSprite->Draw(m_Position, m_Scale, m_Scale);
}

void Effect::Update(float elapsedSec)
{
	if (m_pSprite == nullptr)
		return;

	m_pSprite->Update(elapsedSec);
}

bool Effect::ReadyToDelete()
{
	if (m_pSprite == nullptr)
		return false;

	return (m_Temporary && m_pSprite->GetNrOfCycles() == 1);
}
// --> HELPER FUNCTIONS
