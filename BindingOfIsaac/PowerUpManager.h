#pragma once
#include <vector>

class Character;
class PowerUp;
class Camera;

enum class TypePowerUp
{
	heart = 0,
	belt = 1,
	snack = 2,
	poison = 3,
	lemon = 4,
	end = 5
};

struct PowerUpInfo
{
	PowerUpInfo() : fileName{}, type{ TypePowerUp::end }
	{
	}

	std::string fileName;
	TypePowerUp type;
};

class PowerUpManager
{
public:
	PowerUpManager();
	PowerUpManager(std::stringstream& info);
	~PowerUpManager( );

	// rule of 5
	PowerUpManager(const PowerUpManager& other) = delete;
	PowerUpManager(PowerUpManager&& other) = delete;
	PowerUpManager& operator=(const PowerUpManager& other) = delete;
	PowerUpManager& operator=(PowerUpManager&& other) = delete;

	void AddItem(const Point2f &center, float scale);
	void Update(Character* pActor, Camera* pCamera);
	void Draw() const;

	size_t Size() const;
	void HitItem(Character* pActor, Camera* pCanera);

	friend std::ostream& operator<< (std::ostream& out, const PowerUpManager& pum);

private:
	std::vector<PowerUp*> m_pItems;

	float m_Scale;

	// --> HELPER FUNCTIONS
	void DeleteAllPowerUps();
	void DeletePowerUps();
};
