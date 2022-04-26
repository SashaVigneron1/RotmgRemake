#include "pch.h"
#include "Map.h"
#include "NoiseGenerator.h"
#include "TileManager.h"
#include "ObstacleManager.h"
#include "Biome.h"
#include "Texture.h"
#include "Enemy.h"

Map::Map()
	: m_pNoiseGenerator{ new NoiseGenerator{}}
	, m_pTileManager { new TileManager{}}
	, m_pObstacleManager { new ObstacleManager{}}
	, m_HeightMap{}
	, m_Width{ 250 }
	, m_Height{ 250 }
	, m_Scale{ 7.f}
	, m_Offset{ Point2f{0,0} }
{
	m_Offset = Point2f{ -m_Width / 4.f, -m_Height / 4.f };

	CreateBiomes();
	CreateNoiseMaps();

	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Width; y++)
		{
			Biome* pBiome = GetBiome(m_HeightMap[x][y], m_HeatMap[x][y], m_MoistureMap[x][y], m_DistanceMap[x][y]);
			m_pTileManager->AddTile(Point2f{ x * (m_pTileManager->GetTextureSize().width) * m_Scale - (-m_Offset.x * (m_pTileManager->GetTextureSize().width) * m_Scale),
											 y * (m_pTileManager->GetTextureSize().height) * m_Scale - (-m_Offset.y * (m_pTileManager->GetTextureSize().height) * m_Scale) },
									pBiome->GetBiomeType(), m_Scale);
			float randomNr{rand()%101 / 100.f};
			if (randomNr < pBiome->GetObstacleFrequency()) 
			{
				m_pObstacleManager->AddObstacle(pBiome->GetObstacleTexture(), Point2f{ x * (m_pTileManager->GetTextureSize().width) * m_Scale - (-m_Offset.x * (m_pTileManager->GetTextureSize().width) * m_Scale),
											 y * (m_pTileManager->GetTextureSize().height) * m_Scale - (-m_Offset.y * (m_pTileManager->GetTextureSize().height) * m_Scale)}, m_Scale);
			}
		}
	}

}

Map::~Map()
{
	
	DeleteBiomes();
	DeleteNoiseMaps();

	delete m_pNoiseGenerator;
	delete m_pTileManager;
	delete m_pObstacleManager;
}

Biome* Map::GetBiome(float height, float heat, float moisture, float distance)
{
	

	std::vector<Biome*> pTempBiomes;
	for (Biome* biome : m_pBiomes)
	{
		if (biome->MatchesCondition(height, heat, moisture, distance)) 
		{
			pTempBiomes.push_back(biome);
		}
	}

	Biome* biomeToReturn{};
	float smallestNumber{ -1.f };

	for (Biome* biome : pTempBiomes)
	{
		if (smallestNumber == -1.f)
		{
			float diffValue{ biome->GetDiffValue(height, heat, moisture, distance) };
			smallestNumber = diffValue;
			biomeToReturn = biome;
		}
		else 
		{
			float diffValue{ biome->GetDiffValue(height, heat, moisture, distance) };
			if (diffValue <= smallestNumber)
			{
				smallestNumber = diffValue;
				biomeToReturn = biome;
			}
		}
	}
	return biomeToReturn;
}
Biome* Map::GetBiome(const Point2f& position) 
{
	int x{int(position.x / m_pTileManager->GetTextureSize().width / m_Scale) - (int)m_Offset.x};
	int y{int(position.y / m_pTileManager->GetTextureSize().height / m_Scale) - (int)m_Offset.y};
	Biome* pBiome = GetBiome(m_HeightMap[x][y], m_HeatMap[x][y], m_MoistureMap[x][y], m_DistanceMap[x][y]);
	return pBiome;
}

void Map::CreateBiomes()
{
	// Grasslands
	float minHeight{ 0.1f };
	float minHeat{ 0.2f };
	float minMoisture{ 0.1f };
	float maxDistance{ 0.5f };
	Texture* pObstacleTexture{ new Texture{"Resources/Images/Biomes/Grasslands/Obstacles/Grasslands_Obstacle_0.png"} };
	float obstacleFrequency{ 0.025f }; // in percentage of all tiles

	m_pBiomes.push_back(new Biome{ Biome::BiomeType::Grasslands, minHeight, minHeat, minMoisture, maxDistance, pObstacleTexture, obstacleFrequency});

	// Beach
	minHeight = 0.0f;
	minHeat = 0.1f;
	minMoisture = 0.0f;
	maxDistance = 0.55f;
	pObstacleTexture = new Texture{"Resources/Images/Biomes/Beach/Obstacles/Beach_Obstacle_0.png"};
	obstacleFrequency = 0.025f;
	m_pBiomes.push_back(new Biome{ Biome::BiomeType::Beach, minHeight, minHeat, minMoisture, maxDistance, pObstacleTexture, obstacleFrequency });

	// Water
	minHeight = 0.0f;
	minHeat = 0.0f;
	minMoisture = 0.0f;
	maxDistance = 0.56f;
	pObstacleTexture = nullptr;
	obstacleFrequency = 0.f; 
	m_pBiomes.push_back(new Biome{ Biome::BiomeType::Water, minHeight, minHeat, minMoisture, maxDistance, pObstacleTexture, obstacleFrequency });

	// Ocean
	minHeight = 0.0f;
	minHeat = 0.0f;
	minMoisture = 0.0f;
	maxDistance = 1.f;
	pObstacleTexture = nullptr;
	obstacleFrequency = 0.f;
	m_pBiomes.push_back(new Biome{ Biome::BiomeType::Ocean, minHeight, minHeat, minMoisture, maxDistance, pObstacleTexture, obstacleFrequency });

	// Forest
	minHeight = 0.1f;
	minHeat = 0.3f;
	minMoisture = 0.5f;
	maxDistance = 0.4f;
	pObstacleTexture = new Texture{ "Resources/Images/Biomes/Forest/Obstacles/Forest_Obstacle_0.png" };
	obstacleFrequency = 0.025f;
	m_pBiomes.push_back(new Biome{ Biome::BiomeType::Forest, minHeight, minHeat, minMoisture, maxDistance, pObstacleTexture, obstacleFrequency });


	// Godlands
	minHeight = 0.2f;
	minHeat = 0.4f;
	minMoisture = 0.4f;
	maxDistance = 0.3f;
	pObstacleTexture = new Texture{ "Resources/Images/Biomes/Godlands/Obstacles/Godlands_Obstacle_0.png" };
	obstacleFrequency = 0.01f;
	m_pBiomes.push_back(new Biome{ Biome::BiomeType::Godlands, minHeight, minHeat, minMoisture, maxDistance, pObstacleTexture, obstacleFrequency });


}
void Map::DeleteBiomes() 
{
	for (Biome* pBiome : m_pBiomes)
	{
		delete pBiome;
	}
}
void Map::CreateNoiseMaps()
{
	m_HeightMap = m_pNoiseGenerator->GenerateMapArray(m_Width, m_Height);
	m_HeatMap = m_pNoiseGenerator->GenerateMapArray(m_Width, m_Height);
	m_MoistureMap = m_pNoiseGenerator->GenerateMapArray(m_Width, m_Height);
	m_DistanceMap = m_pNoiseGenerator->GenerateIslandMapArray(m_Width, m_Height);
}
void Map::DeleteNoiseMaps() 
{
	for (int x{ 0 }; x < m_Width; ++x)
	{
		delete m_HeightMap[x];
	}
	delete m_HeightMap;

	for (int x{ 0 }; x < m_Width; ++x)
	{
		delete m_HeatMap[x];
	}
	delete m_HeatMap;

	for (int x{ 0 }; x < m_Width; ++x)
	{
		delete m_MoistureMap[x];
	}
	delete m_MoistureMap;

	for (int x{ 0 }; x < m_Width; ++x)
	{
		delete m_DistanceMap[x];
	}
	delete m_DistanceMap;
}

void Map::DrawMap(const Point2f& playerPos) const
{
	m_pTileManager->Draw(playerPos);
	m_pObstacleManager->Draw(playerPos);
}
void Map::Update(Point2f& playerPosition, float playerSize, Character::CharacterDirection direction) 
{
	m_pTileManager->Update(playerPosition, playerSize, direction);
	m_pObstacleManager->Update(playerPosition, playerSize);
}

std::vector<Biome*> Map::GetBiomes()
{
	return m_pBiomes;
}

ObstacleManager* Map::GetObstacleManager() const
{
	return m_pObstacleManager;
}

