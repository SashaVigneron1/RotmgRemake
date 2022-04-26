#include "pch.h"
#include "TileManager.h"
#include "Tile.h"
#include "Texture.h"
#include "utils.h"

TileManager::TileManager()
{
	CreateTextures();
}

TileManager::~TileManager()
{
	DeleteTextures();
	DeleteTiles();
}

void TileManager::Draw(const Point2f& playerPos) const
{
	bool isOptimizeMode{ true };
	if (isOptimizeMode) 
	{
		float maxDistance{ 820.f };
		for (Tile* tile : m_pTiles)
		{
			Point2f position{ tile->GetPosition() };
			float distance{ utils::GetDistance(playerPos, position) };
			if (distance <= maxDistance)
			{
				tile->Draw();
			}
		}
	}
	else 
	{
		for (Tile* tile : m_pTiles)
		{
			tile->Draw();
		}
	}
}
void TileManager::Update(Point2f& playerPos, float playerSize, Character::CharacterDirection direction)
{

	float maxDistance{ 150.f };
	float halfOfActualPlayerWidth{ playerSize / 2.f };
	float halfOfActualPlayerHeight{ playerSize / 2.f };

	for (Tile* tile : m_pTiles)
	{
		Rectf tileRect{ tile->GetRect() };
		float distance{ utils::GetDistance(playerPos, Point2f{tileRect.left, tileRect.bottom}) };
		
		if (distance <= maxDistance)
		{
			bool isWalkable{ tile->GetWalkable() };

			if (!isWalkable)
			{
				float offset{ 3.f };
				bool isOverlapping{ tile->IsOverlapping(Rectf{ playerPos.x - halfOfActualPlayerWidth, playerPos.y - halfOfActualPlayerHeight, 2*halfOfActualPlayerWidth,  2 * halfOfActualPlayerHeight }) };

				if (isOverlapping) 
				{
					float emptyBorder{ 10.f };

					switch (direction)
					{
					case Character::CharacterDirection::down:
						if (playerPos.y - halfOfActualPlayerHeight < tileRect.bottom + tileRect.height)
						{
							playerPos.y = tileRect.bottom + tileRect.height + halfOfActualPlayerHeight + offset;
						}
						break;
					case Character::CharacterDirection::left:
						if (playerPos.x - halfOfActualPlayerWidth < tileRect.left + tileRect.width
							&& playerPos.y - halfOfActualPlayerHeight > tileRect.bottom)
						{
							playerPos.x = tileRect.left + tileRect.width + halfOfActualPlayerWidth + offset;
						}
						break;
					case Character::CharacterDirection::right:
						if (playerPos.x + halfOfActualPlayerWidth > tileRect.left
							&& playerPos.y - halfOfActualPlayerHeight > tileRect.bottom)
						{
							playerPos.x = tileRect.left - halfOfActualPlayerWidth - offset;
						}

						break;
					case Character::CharacterDirection::up:

						if (playerPos.y + halfOfActualPlayerHeight > tileRect.bottom)
						{
							playerPos.y = tileRect.bottom - halfOfActualPlayerHeight - offset;
						}
						break;
					case Character::CharacterDirection::upLeft:
						
						// RightSide
						if (playerPos.x - halfOfActualPlayerWidth < tileRect.left + tileRect.width
							&& playerPos.x - halfOfActualPlayerWidth > tileRect.left + tileRect.width - emptyBorder)
						{
							playerPos.x = tileRect.left + tileRect.width + halfOfActualPlayerWidth + offset;
						}
						// or BottomSide
						else if (playerPos.y + halfOfActualPlayerHeight > tileRect.bottom
								&& playerPos.y + halfOfActualPlayerHeight < tileRect.bottom + emptyBorder)
						{
							playerPos.y = tileRect.bottom - halfOfActualPlayerHeight - offset;
						}

						break;
					case Character::CharacterDirection::upRight:
						
						// LeftSide
						if (playerPos.x + halfOfActualPlayerWidth > tileRect.left
							&& playerPos.x + halfOfActualPlayerWidth < tileRect.left + emptyBorder)
						{
							playerPos.x = tileRect.left - halfOfActualPlayerWidth - offset;
						}
						// or BottomSide
						else if (playerPos.y + halfOfActualPlayerHeight > tileRect.bottom
							&& playerPos.y + halfOfActualPlayerHeight < tileRect.bottom + emptyBorder)
						{
							playerPos.y = tileRect.bottom - halfOfActualPlayerHeight - offset;
						}

						break;
					case Character::CharacterDirection::downLeft:
						// RightSide
						if (playerPos.x - halfOfActualPlayerWidth < tileRect.left + tileRect.width
							&& playerPos.x - halfOfActualPlayerWidth > tileRect.left + tileRect.width - emptyBorder)
						{
							playerPos.x = tileRect.left + tileRect.width + halfOfActualPlayerWidth + offset;
						}
						// or TopSide
						else if (playerPos.y - halfOfActualPlayerHeight < tileRect.bottom + tileRect.height
							&& playerPos.y - halfOfActualPlayerHeight > tileRect.bottom + tileRect.height - emptyBorder)
						{
							playerPos.y = tileRect.bottom + tileRect.height + halfOfActualPlayerHeight + offset;
						}
						break;
					case Character::CharacterDirection::downRight:
						// LeftSide
						if (playerPos.x + halfOfActualPlayerWidth > tileRect.left
							&& playerPos.x + halfOfActualPlayerWidth < tileRect.left + emptyBorder)
						{
							playerPos.x = tileRect.left - halfOfActualPlayerWidth - offset;
						}
						// or TopSide
						else if (playerPos.y - halfOfActualPlayerHeight < tileRect.bottom + tileRect.height
							&& playerPos.y - halfOfActualPlayerHeight > tileRect.bottom + tileRect.height - emptyBorder)
						{
							playerPos.y = tileRect.bottom + tileRect.height + halfOfActualPlayerHeight + offset;
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}
		
}

void TileManager::CreateTextures()
{
	m_pGrassTexture = new Texture{ "Resources/Images/Biomes/Grasslands/Tiles/Grass_0.png" };
	m_pSandTexture = new Texture{ "Resources/Images/Biomes/Beach/Tiles/Sand_0.png" };
	m_pWaterTexture = new Texture{ "Resources/Images/Biomes/Water/Tiles/Water_0.png" };
	m_pOceanTexture = new Texture{ "Resources/Images/Biomes/Ocean/Tiles/Ocean_0.png" };
	m_pForestTexture = new Texture{ "Resources/Images/Biomes/Forest/Tiles/Forest_0.png" };
	m_pGodlandsTexture = new Texture{ "Resources/Images/Biomes/Godlands/Tiles/Godlands_0.png" };
}

void TileManager::DeleteTextures()
{
	delete m_pGrassTexture;
	delete m_pSandTexture;
	delete m_pWaterTexture;
	delete m_pOceanTexture;
	delete m_pForestTexture;
	delete m_pGodlandsTexture;
}
void TileManager::DeleteTiles() 
{
	for (size_t i = 0; i < m_pTiles.size(); i++)
	{
		delete m_pTiles[i];
		m_pTiles[i] = nullptr;
	}
}

void TileManager::AddTile(const Point2f& pos, Biome::BiomeType biome, float scale) 
{
	switch (biome)
	{
	case Biome::BiomeType::Grasslands:
		m_pTiles.push_back(new Tile{ m_pGrassTexture, pos, true, scale });
		break;
	case Biome::BiomeType::Beach:
		m_pTiles.push_back(new Tile{ m_pSandTexture, pos, true, scale });
		break;
	case Biome::BiomeType::Water:
		m_pTiles.push_back(new Tile{ m_pWaterTexture, pos, true, scale });
		break;
	case Biome::BiomeType::Ocean:
		m_pTiles.push_back(new Tile{ m_pOceanTexture, pos, false, scale });
		break;
	case Biome::BiomeType::Forest:
		m_pTiles.push_back(new Tile{ m_pForestTexture, pos, true, scale });
		break;
	case Biome::BiomeType::Godlands:
		m_pTiles.push_back(new Tile{ m_pGodlandsTexture, pos, true, scale });
		break;
	default:
		break;
	}
}

Rectf TileManager::GetTextureSize() 
{
	return Rectf{ 0, 0 , m_pGrassTexture->GetWidth(), m_pGrassTexture->GetHeight() };
}

