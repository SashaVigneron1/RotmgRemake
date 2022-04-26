#include "pch.h"
#include "ProjectileManager.h"
#include "ObstacleManager.h"
#include "Projectile.h"


ProjectileManager::ProjectileManager(ObstacleManager* obstacleManager)
	: m_pObstacleManager{ obstacleManager }
{
}

ProjectileManager::~ProjectileManager()
{
	size_t size{ m_pProjectiles.size() };
	for (size_t i = 0; i < size; i++)
	{
		delete m_pProjectiles[size - i - 1];
		m_pProjectiles.pop_back();
	}
}

void ProjectileManager::UpdateProjectiles(float elapsedSec)
{
	for (size_t i{}; i < m_pProjectiles.size(); ++i)
	{
		if (m_pProjectiles[i]) 
		{
			if (m_pObstacleManager->CheckHit(m_pProjectiles[i]->GetRect()))
			{
				RemoveProjectile(m_pProjectiles[i]);
			}
			else 
			{
				m_pProjectiles[i]->Update(elapsedSec);
			}
		}
	}
}

void ProjectileManager::DrawProjectiles() const
{
	for (Projectile* pProjectile : m_pProjectiles)
	{
		if (pProjectile)
		{
			pProjectile->Draw();
		}
	}
}

void ProjectileManager::AddProjectile(float damage, float range, const Point2f& startingPosition, const Vector2f& velocity, const Color4f& color, bool isPlayerProjectile)
{
	m_pProjectiles.push_back(new Projectile{ this, damage, range,startingPosition, velocity, color, isPlayerProjectile });
}

void ProjectileManager::RemoveProjectile(Projectile* projectile) 
{
	for (size_t i{}; i < m_pProjectiles.size(); ++i)
	{
		if (m_pProjectiles[i] == projectile)
		{
			delete m_pProjectiles[i];
			m_pProjectiles[i] = nullptr;
			m_pProjectiles[i] = m_pProjectiles.back();
			m_pProjectiles.pop_back();
			return;
		}
	}
}

float ProjectileManager::GetDamageIfHit(const Rectf& rect, bool isPlayer)
{
	bool succes{ false };
	float damage{ -1.f };
	for (Projectile* pProjectile : m_pProjectiles)
	{
		if (pProjectile && !succes)
		{
			if (isPlayer)
			{
				if (!pProjectile->IsPlayerProjectile()) 
				{
					if (pProjectile->CheckOverlap(rect))
					{
						damage = pProjectile->GetDamage();
						succes = true;
						RemoveProjectile(pProjectile);
					}
				}
			}
			else 
			{
				if (pProjectile->IsPlayerProjectile())
				{
					if (pProjectile->CheckOverlap(rect))
					{
						damage = pProjectile->GetDamage();
						succes = true;
						RemoveProjectile(pProjectile);
					}
				}
			}
			
		}
	}
	return damage;
}


