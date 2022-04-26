#include "pch.h"
#include "Biome.h"
#include "Texture.h"
#include "Enemy.h"

Biome::Biome(BiomeType biome, float minHeight, float minHeat, float minMoisture, float maxDistance, Texture* obstacleTexture, float obstacleFrequency)
	: m_BiomeType{ biome }
	, m_MinHeight {minHeight}
	, m_MinHeat{ minHeat }
	, m_MinMoisture{ minMoisture }
	, m_MaxDistance{ maxDistance }
	, m_pObstacleTexture{ obstacleTexture }
	, m_ObstacleFrequency{ obstacleFrequency }
{
}

Biome::~Biome()
{
	delete m_pObstacleTexture;
}

float Biome::GetDiffValue(float height, float heat, float moisture, float distance)
{
	return (height - m_MinHeight) + (heat - m_MinHeat) + (moisture - m_MinMoisture) + (m_MaxDistance - distance);
}

Biome::BiomeType Biome::GetBiomeType() const 
{
	return m_BiomeType;
}

bool Biome::MatchesCondition(float height, float heat, float moisture, float distance)
{
	return (height >= m_MinHeight) && (heat >= m_MinHeat) && (moisture >= m_MinMoisture) && (distance <= m_MaxDistance);
}

Texture* Biome::GetObstacleTexture() const 
{
	return m_pObstacleTexture;
}

float Biome::GetObstacleFrequency() const
{
	return m_ObstacleFrequency;
}