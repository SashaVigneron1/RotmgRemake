#pragma once
#include <vector>

class Enemy;
class Map;
class ProjectileManager;
class ItemBagManager;
class SoundEffect;


struct EnemyDeathSound 
{
	EnemyDeathSound(SoundEffect* soundEffect);
	~EnemyDeathSound();
	SoundEffect* soundEffect;
	float timeAlive;
};

class EnemyManager final
{
public:
	EnemyManager(ProjectileManager* projectileManager, Map* map, ItemBagManager* itemBagManager);
	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager& operator=(const EnemyManager& other) = delete;
	EnemyManager(EnemyManager&& other) = delete;
	EnemyManager& operator=(EnemyManager&& other) = delete;
	~EnemyManager();

	void Update(float elapsedSec, const Point2f& playerPos);
	void DrawEnemies(const Point2f& playerPos) const;
	void KillEnemy(Enemy* enemy);

	void PlayEnemyDeathSound(const std::string& filePath);

private:
	std::vector<Enemy*> m_pEnemies;
	std::vector<EnemyDeathSound*> m_pEnemyDeathSounds;

	ProjectileManager* m_pProjectileManager;
	ItemBagManager* m_pItemBagManager;
	Map* m_pMap;

	// Spawn Timer
	float m_MinTimeBetweenSpawns;
	float m_MaxTimeBetweenSpawns;
	float m_CurrTimeBetweenSpawn;
	float m_AccTimeBetweenSpawn;

	// Spawn Position
	float m_MinXDifference;
	float m_MaxXDifference;
	float m_MinYDifference;
	float m_MaxYDifference;

	void UpdateEnemies(float elapsedSec, const Point2f& playerPos);
	void UpdateDeathSounds(float elapsedSec);
	void SpawnEnemiesNearPlayer(float elapsedSec, const Point2f& playerPos);
};

