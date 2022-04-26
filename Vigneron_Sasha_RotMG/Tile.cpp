#include "pch.h"
#include "Tile.h"
#include "Texture.h"
#include "utils.h"

Tile::Tile(const Texture* texture, const Point2f& pos, bool isWalkable, float scale)
	: m_pTexture{ texture }
	, m_Position{ pos }
	, m_IsWalkable{ isWalkable }
	, m_Scale{ scale }
{
}

void Tile::Draw() const
{
	Rectf destRect{ m_Position.x, m_Position.y, m_pTexture->GetWidth() * m_Scale, m_pTexture->GetHeight() * m_Scale };
	m_pTexture->Draw(destRect);
}

Rectf Tile::GetRect() const
{
	return Rectf{ m_Position.x, m_Position.y, m_pTexture->GetWidth() * m_Scale, m_pTexture->GetHeight() * m_Scale };
}

Point2f Tile::GetPosition() const
{
	return m_Position;
}

bool Tile::GetWalkable() const
{
	return m_IsWalkable;
}

bool Tile::IsOverlapping(const Rectf& rect) const
{
	return utils::IsOverlapping(rect, Rectf{ m_Position.x, m_Position.y, m_pTexture->GetWidth() * m_Scale, m_pTexture->GetHeight() * m_Scale });
}
