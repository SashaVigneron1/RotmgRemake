#pragma once
#include <vector>
#include "Biome.h"
#include "Character.h"

class Tile;

class TileManager final
{
public:

	TileManager();
	TileManager(const TileManager& other) = delete;
	TileManager& operator=(const TileManager& other) = delete;
	TileManager(TileManager&& other) = delete;
	TileManager& operator=(TileManager&& other) = delete;
	~TileManager();

	void AddTile(const Point2f& pos, Biome::BiomeType biome, float scale);
	void Draw(const Point2f& playerPos) const;
	void Update(Point2f& playerPos, float playerSize, Character::CharacterDirection direction);
	Rectf GetTextureSize();

private:
	std::vector<Tile*> m_pTiles;

	Texture* m_pGrassTexture;

	Texture* m_pSandTexture;

	Texture* m_pWaterTexture;

	Texture* m_pOceanTexture;

	Texture* m_pForestTexture;

	Texture* m_pGodlandsTexture;

	void CreateTextures();
	void DeleteTextures();
	void DeleteTiles();
};

