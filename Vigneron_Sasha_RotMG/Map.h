#pragma once
#include <vector>
#include "NoiseGenerator.h"
#include "Character.h"

class NoiseGenerator;
class TileManager;
class ObstacleManager;
class Biome;
class Enemy;

class Map final
{

public: 
	Map();
	Map(const Map& other) = delete;
	Map& operator=(const Map& other) = delete;
	Map(Map&& other) = delete;
	Map& operator=(Map&& other) = delete;
	~Map();

	void DrawMap(const Point2f& playerPos) const;
	void Update(Point2f& playerPosition, float playerSize, Character::CharacterDirection direction);

	std::vector<Biome*> GetBiomes();

	ObstacleManager* GetObstacleManager() const;
	Biome* GetBiome(const Point2f& position);

private: 

	NoiseGenerator* m_pNoiseGenerator;
	TileManager* m_pTileManager;
	ObstacleManager* m_pObstacleManager;



	//Overall
	int m_Width;
	int m_Height;
	float m_Scale;
	Point2f m_Offset;

	std::vector<Biome*> m_pBiomes;

	//HeightMap
	float** m_HeightMap;

	//HeatMap
	float** m_HeatMap;

	//MoistureMap
	float** m_MoistureMap;

	//IslanMap
	float** m_DistanceMap;

	
	Biome* GetBiome(float height, float heat, float moisture, float distance);


	void CreateBiomes();
	void DeleteBiomes();
	void CreateNoiseMaps();
	void DeleteNoiseMaps();
};

