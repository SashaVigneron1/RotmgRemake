#pragma once

#include "Enemy.h"

class ProjectileManager;
class EnemyManager;

class Snake final : public Enemy
{
	

public: 
	Snake(ProjectileManager* projectileManager, EnemyManager* enemyManager, const Point2f& position);
	Snake(const Snake& other) = delete;
	Snake& operator=(const Snake& other) = delete;
	Snake(Snake&& other) = delete;
	Snake& operator=(Snake&& other) = delete;
	virtual ~Snake() override;

	virtual void Update(float elapsedSec, const Point2f& playerPos) override;
	virtual void Draw() const override;
private:

};

