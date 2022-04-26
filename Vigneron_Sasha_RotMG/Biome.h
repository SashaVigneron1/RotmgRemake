#pragma once
#include "Tile.h"
#include <vector>

class Enemy;

class Biome final
{

public:

	enum class BiomeType
	{
		Grasslands = 0,
		Beach = 1,
		Water = 2,
		Ocean = 3,
		Forest = 4,
		Godlands = 5
	};

	Biome(BiomeType biome, float minHeight, float minHeat, float minMoisture, float maxDistance, Texture* obstacleTexture, float obstacleFrequency);
	Biome(const Biome& other) = delete;
	Biome& operator=(const Biome& other) = delete;
	Biome(Biome&& other) = delete;
	Biome& operator=(Biome&& other) = delete;
	~Biome();

	float GetDiffValue(float height, float heat, float minMoisture, float maxDistance);
	BiomeType GetBiomeType() const;
	bool MatchesCondition(float height, float heat, float minMoisture, float maxDistance);
	Texture* GetObstacleTexture() const;
	float GetObstacleFrequency() const;
private:
	const float m_MinHeight;
	const float m_MinHeat;
	const float m_MinMoisture;
	const float m_MaxDistance;
	const BiomeType m_BiomeType;

	Texture* m_pObstacleTexture;
	const float m_ObstacleFrequency;

};

