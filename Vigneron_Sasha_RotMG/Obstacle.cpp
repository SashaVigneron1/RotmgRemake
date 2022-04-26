#include "pch.h"
#include "Obstacle.h"
#include "Texture.h"
#include "utils.h"

Obstacle::Obstacle(Texture* texture, const Point2f& position, float scale)
	: m_pTexture{texture}
	, m_Rect{ Rectf{position.x, position.y, 10.f, 10.f} }
	, m_Scale{ scale }
{

}

Rectf Obstacle::GetRect() const
{
	Rectf rect{ m_Rect.left, m_Rect.bottom, m_pTexture->GetWidth() * m_Scale, m_pTexture->GetHeight() * m_Scale };
	return rect;
}

void Obstacle::Draw() const
{
	Rectf destRect{ m_Rect.left, m_Rect.bottom, m_pTexture->GetWidth() * m_Scale, m_pTexture->GetHeight() * m_Scale };
	
	m_pTexture->Draw(destRect);
}

bool Obstacle::CheckHit(const Rectf& rect) const
{
	return utils::IsOverlapping(Rectf{ m_Rect.left, m_Rect.bottom, m_Rect.width * m_Scale, m_Rect.height * m_Scale }, rect);
}


