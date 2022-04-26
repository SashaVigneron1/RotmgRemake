#pragma once

#include "Enemy.h"

class ProjectileManager;
class EnemyManager;

class Medusa final : public Enemy
{


public:
	Medusa(ProjectileManager* projectileManager, EnemyManager* enemyManager, const Point2f& position);
	Medusa(const Medusa& other) = delete;
	Medusa& operator=(const Medusa& other) = delete;
	Medusa(Medusa&& other) = delete;
	Medusa& operator=(Medusa&& other) = delete;
	virtual ~Medusa();

	virtual void Update(float elapsedSec, const Point2f& playerPos) override;
	virtual void Draw() const override;
private:

};

