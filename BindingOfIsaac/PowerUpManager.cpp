#include "pch.h"
#include "PowerUpManager.h"
#include "Heart.h"
#include "Belt.h"
#include "Snack.h"
#include "PoisonTouch.h"
#include "Lemon.h"
#include "utils.h"
#include "Character.h"
#include "Camera.h"
#include <algorithm>
#include <iterator>
#include <iomanip>

PowerUpManager::PowerUpManager()
	: m_pItems{  }
	, m_Scale{ 2.5f }
{
}

PowerUpManager::PowerUpManager(std::stringstream & info)
	: m_pItems{  }
	, m_Scale{ 2.5f }
{
	std::string part{};
	while (!info.eof())
	{
		std::getline(info, part);
		std::string::iterator pos{};

		// delete spaces
		pos = std::remove_if(part.begin(), part.end(), [](const char c) {
			return (c == ' ');
		});
		part.erase(pos, part.end());

		std::string tempInfo{};
		std::stringstream powerUp{};
		// we are sure that active room is first --> AddRoom instead of LoadRoom

		if (part == "<Heart")
		{
			// mepty ss
			powerUp.str(std::string{});
			powerUp << utils::GetPartValue("Heart", info.str());

			m_pItems.push_back(new Heart{ info, 2.5f });

			// shrink string
			std::string find{ "/Heart>" };
			size_t pos = info.str().find(find);
			tempInfo = info.str().substr(pos + find.size());
			info.str(std::string{});
			info << tempInfo;
		}
		else if (part == "<Belt")
		{
			// mepty ss
			powerUp.str(std::string{});
			powerUp << utils::GetPartValue("Belt", info.str());

			m_pItems.push_back(new Belt{ info, 2.5f });

			// shrink string
			std::string find{ "/Belt>" };
			size_t pos = info.str().find(find);
			tempInfo = info.str().substr(pos + find.size());
			info.str(std::string{});
			info << tempInfo;
		}
		else if (part == "<Snack")
		{
			// mepty ss
			powerUp.str(std::string{});
			powerUp << utils::GetPartValue("Snack", info.str());

			m_pItems.push_back(new Snack{ info, 2.5f });

			// shrink string
			std::string find{ "/Snack>" };
			size_t pos = info.str().find(find);
			tempInfo = info.str().substr(pos + find.size());
			info.str(std::string{});
			info << tempInfo;
		}
		else if (part == "<Poison")
		{
			// mepty ss
			powerUp.str(std::string{});
			powerUp << utils::GetPartValue("Poison", info.str());

			m_pItems.push_back(new PoisonTouch{ info, 2.5f });

			// shrink string
			std::string find{ "/Poison>" };
			size_t pos = info.str().find(find);
			tempInfo = info.str().substr(pos + find.size());
			info.str(std::string{});
			info << tempInfo;
		}
		else if (part == "<Lemon")
		{
			// mepty ss
			powerUp.str(std::string{});
			powerUp << utils::GetPartValue("Lemon", info.str());

			m_pItems.push_back(new Lemon{ info, 2.5f });

			// shrink string
			std::string find{ "/Lemon>" };
			size_t pos = info.str().find(find);
			tempInfo = info.str().substr(pos + find.size());
			info.str(std::string{});
			info << tempInfo;
		}
	}
}


PowerUpManager::~PowerUpManager()
{
	DeleteAllPowerUps();
}

void PowerUpManager::AddItem(const Point2f & center, float scale)
{
	TypePowerUp i{TypePowerUp( rand() % int(TypePowerUp::end)) };
	switch (i)
	{
	case TypePowerUp::heart:
		m_pItems.push_back(new Heart{ center, scale });
		break;
	case TypePowerUp::belt:
		m_pItems.push_back(new Belt{ center, scale });
		break;
	case TypePowerUp::snack:
		m_pItems.push_back(new Snack{ center, scale });
		break;
	case TypePowerUp::poison:
		m_pItems.push_back(new PoisonTouch{ center, scale });
		break;
	case TypePowerUp::lemon:
		m_pItems.push_back(new Lemon{ center, scale });
		break;
	case TypePowerUp::end:
	default:
		break;
	}
}

void PowerUpManager::Update(Character* pActor, Camera* pCamera)
{
	// check collision with power up
	HitItem(pActor, pCamera);

	// delete when hit
	DeletePowerUps();
}

void PowerUpManager::Draw() const
{
	for (PowerUp* element : m_pItems)
	{
		element->Draw();
	}
	
}

size_t PowerUpManager::Size() const
{
	return m_pItems.size();
}

void PowerUpManager::HitItem(Character* pActor, Camera* pCamera)
{
	for (PowerUp* element : m_pItems)
	{
		PowerUpInfo info{ element->IsOverlapping(pActor->GetShape()) };
		if (info.type != TypePowerUp::end)
		{
			pActor->PowerUpHit(info);
			pCamera->SetToZoomIn();
		}
	}
}

void PowerUpManager::DeleteAllPowerUps()
{
	for (PowerUp* element : m_pItems)
	{
		delete element;
	}
	m_pItems.clear();
}

void PowerUpManager::DeletePowerUps()
{
	std::vector<PowerUp*>::iterator it{};

	it = std::remove_if(m_pItems.begin(), m_pItems.end(), [] (PowerUp* pPowerUp) {
		if (pPowerUp->ReadyToDelete())
		{
			delete pPowerUp;
			pPowerUp = nullptr;

			return true;
		}
		return false;
	});

	m_pItems.erase(it, m_pItems.end());
}

// --> OPERATOR OVERLOADING
std::ostream& operator<<(std::ostream& out, const PowerUpManager& pum)
{
	out << std::setfill(' ') << std::setw(3) << "" << "<PowerUpManager\n";

	for (PowerUp* pPowerUp : pum.m_pItems)
	{
		out << *pPowerUp;
	}

	out << std::setw(3) << "" << "/PowerUpManager>\n";

	return out;
}
