#pragma once
#include "pch.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "utils.h"


Projectile::Projectile(ProjectileManager* projectileManager, float damage, float range, const Point2f& startingPosition, const Vector2f& velocity, const Color4f& color, bool isPlayerProjectile)
	: m_pProjectileManager{projectileManager}
	, m_Width{ 20.f }
	, m_Height{ 10.0f }
	, m_Damage{damage}
	, m_Range{range}
	, m_StartPosition{startingPosition}
	, m_Position{startingPosition}
	, m_Velocity{velocity}
	, m_Color{ color }
	, m_Angle{ atan2(velocity.y, velocity.x) }
	, m_IsPlayerProjectile{ isPlayerProjectile }
{
	
}

Projectile::~Projectile()
{
}

void Projectile::Update(float elapsedSec)
{
	m_Position += m_Velocity * elapsedSec;
	float distance{ sqrtf((m_Position.x - m_StartPosition.x)
					* (m_Position.x - m_StartPosition.x)
					+ (m_Position.y - m_StartPosition.y)
					* (m_Position.y - m_StartPosition.y)) };
	if (distance >= m_Range) 
	{
		m_pProjectileManager->RemoveProjectile(this);
	}
}

void Projectile::Draw() const
{

	utils::SetColor(m_Color);
	glPushMatrix();
	{
		glTranslatef(m_Position.x, m_Position.y, 0);
		glRotatef(utils::RadToDegrees(m_Angle), 0, 0, 1);
		glTranslatef(-m_Width/2, -m_Height/2, 0);
		utils::FillRect(Point2f{0,0}, m_Width, m_Height);
	}
	glPopMatrix();

}

bool Projectile::CheckOverlap(const Rectf& rect) 
{
	Rectf projectileRect{ m_Position.x, m_Position.y , m_Width, m_Height };
	if (utils::IsOverlapping(rect, projectileRect)) 
	{
		return true;
	}
	return false;
}

bool Projectile::IsPlayerProjectile() const 
{
	return m_IsPlayerProjectile;
}

float Projectile::GetDamage() const
{
	return m_Damage;
}

Rectf Projectile::GetRect() const
{
	Rectf rect{ m_Position.x, m_Position.y, m_Width, m_Height };
	return rect;
}
