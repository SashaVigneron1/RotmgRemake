#pragma once
#include <vector>
#include "utils.h"

class Projectile;
class ObstacleManager;

class ProjectileManager final
{
public:
	ProjectileManager(ObstacleManager* obstacleManager);
	ProjectileManager(const ProjectileManager& other) = delete;
	ProjectileManager& operator=(const ProjectileManager& other) = delete;
	ProjectileManager(ProjectileManager&& other) = delete;
	ProjectileManager& operator=(ProjectileManager&& other) = delete;
	~ProjectileManager();

	void UpdateProjectiles(float elapsedSec);
	void DrawProjectiles() const;
	void AddProjectile(float damage, float range, const Point2f& startingPosition, const Vector2f& velocity, const Color4f& color, bool isPlayerProjectile);
	void RemoveProjectile(Projectile* projectile);
	float GetDamageIfHit(const Rectf& rect, bool isPlayer);

private:
	std::vector<Projectile*> m_pProjectiles;
	ObstacleManager* m_pObstacleManager;
};

