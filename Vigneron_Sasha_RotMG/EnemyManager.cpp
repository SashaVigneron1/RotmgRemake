#include "pch.h"
#include "EnemyManager.h"
#include "ProjectileManager.h"
#include "ItemBagManager.h"
#include "Enemy.h"
#include "Snake.h"
#include "Medusa.h"
#include "Golem.h"
#include "Map.h"
#include "Biome.h"

EnemyManager::EnemyManager(ProjectileManager* projectileManager, Map* map, ItemBagManager* itemBagManager)
	: m_MinTimeBetweenSpawns{1.f}
	, m_MaxTimeBetweenSpawns{5.f}
	, m_CurrTimeBetweenSpawn{}
	, m_AccTimeBetweenSpawn{}
	, m_MinXDifference{600.f}
	, m_MaxXDifference{800.f}
	, m_MinYDifference{500.f}
	, m_MaxYDifference{800.f}
	, m_pProjectileManager{ projectileManager }
	, m_pMap{ map }
	, m_pItemBagManager{ itemBagManager }
{
}

EnemyManager::~EnemyManager()
{
	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		delete m_pEnemies[i];
		m_pEnemies[i] = nullptr;
	}
	for (size_t i = 0; i < m_pEnemyDeathSounds.size(); i++)
	{
		delete m_pEnemyDeathSounds[i];
		m_pEnemyDeathSounds[i] = nullptr;
	}
}

void EnemyManager::Update(float elapsedSec, const  Point2f& playerPos)
{
	UpdateEnemies(elapsedSec, playerPos);
	UpdateDeathSounds(elapsedSec);
	SpawnEnemiesNearPlayer(elapsedSec, playerPos);
}

void EnemyManager::DrawEnemies(const Point2f& playerPos) const
{
	bool isOptimizeMode{ true };
	if (isOptimizeMode)
	{
		float maxDistance{ 820.f };
		for (Enemy* pEnemy : m_pEnemies)
		{
			Point2f position{ pEnemy->GetPosition() };
			float distance{ utils::GetDistance(playerPos, position) };
			if (distance <= maxDistance)
			{
				pEnemy->Draw();
			}
		}
	}
}

void EnemyManager::UpdateEnemies(float elapsedSec, const Point2f& playerPos)
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		if (pEnemy) 
		{
			pEnemy->Update(elapsedSec, playerPos);
		}
	}
}

void EnemyManager::SpawnEnemiesNearPlayer(float elapsedSec, const Point2f& playerPos)
{	
	m_AccTimeBetweenSpawn += elapsedSec;
	if (m_AccTimeBetweenSpawn >= m_CurrTimeBetweenSpawn) 
	{
		//Reset Timers
		m_CurrTimeBetweenSpawn = rand() % (int(m_MaxTimeBetweenSpawns - m_MinTimeBetweenSpawns) * 100 - 1) + m_MinTimeBetweenSpawns * 100;
		m_CurrTimeBetweenSpawn /= 100;
		m_AccTimeBetweenSpawn = 0.f;

		//Get Random Position
		float xDiff{ rand() % int(m_MaxXDifference - m_MinXDifference - 1) + m_MinXDifference };
		int xDir{ rand() % 2};
		switch (xDir) 
		{
			case 1:
				xDiff *= -1;
		}
		float yDiff{ rand() % int(m_MaxYDifference - m_MinYDifference - 1) + m_MinYDifference };
		int yDir{ rand() % 2 };
		switch (yDir)
		{
			case 1:
				yDiff *= -1;
		}
		Point2f randomPosition{ playerPos.x + xDiff, playerPos.y + yDiff };

		//Spawn Enemy From Biome At Position
		Biome* pBiome = m_pMap->GetBiome(randomPosition);
		if (pBiome) 
		{
			switch (pBiome->GetBiomeType())
			{
			case Biome::BiomeType::Beach:
				m_pEnemies.push_back(new Snake{ m_pProjectileManager, this, randomPosition });
				break;
			case Biome::BiomeType::Forest:
				m_pEnemies.push_back(new Golem{ m_pProjectileManager, this, randomPosition });
				break;
			case Biome::BiomeType::Godlands:
				m_pEnemies.push_back(new Medusa{ m_pProjectileManager, this, randomPosition });
				break;
			case Biome::BiomeType::Grasslands:
				m_pEnemies.push_back(new Snake{ m_pProjectileManager, this, randomPosition });
				break;
			default:
				break;
			}
		}
		else 
		{
			std::cerr << "Couldn't load biome correctly.\n";
		}
	}
}

void EnemyManager::KillEnemy(Enemy* enemy) 
{
	//Small chance to spawn itembag;
	float chanceToSpawnItemBag{ 0.25f };
	float randomNumber{ rand() % 100 / 100.f };
	if (randomNumber <= chanceToSpawnItemBag) 
	{
		m_pItemBagManager->AddRandomItemBag(enemy->GetPosition());
	}

	for (size_t i{0}; i < m_pEnemies.size(); ++i)
	{
		if (enemy == m_pEnemies[i]) 
		{
			delete m_pEnemies[i];
			m_pEnemies[i] = m_pEnemies.back();
			m_pEnemies.pop_back();
			return;
		}
	}
}

void EnemyManager::PlayEnemyDeathSound(const std::string& filePath)
{
	SoundEffect* pSoundEffect{ new SoundEffect{filePath} };
	EnemyDeathSound* enemyDeathSound{ new EnemyDeathSound{pSoundEffect} };
	m_pEnemyDeathSounds.push_back(enemyDeathSound);
	enemyDeathSound->soundEffect->Play(0);
}

void EnemyManager::UpdateDeathSounds(float elapsedSec) 
{
	float maxTimeAlive{ 5.f };
	for (int i{}; i < m_pEnemyDeathSounds.size(); ++i)
	{
		m_pEnemyDeathSounds[i]->timeAlive += elapsedSec;
		if (m_pEnemyDeathSounds[i]->timeAlive >= maxTimeAlive)
		{
			delete m_pEnemyDeathSounds[i];
			m_pEnemyDeathSounds[i] = nullptr;
			m_pEnemyDeathSounds[i] = m_pEnemyDeathSounds.back();
			m_pEnemyDeathSounds.pop_back();
		}
	}
}


EnemyDeathSound::EnemyDeathSound(SoundEffect* sound)
	: soundEffect{ sound }
	, timeAlive{ 0.0f }
{
}

EnemyDeathSound::~EnemyDeathSound()
{
	delete soundEffect;
}
