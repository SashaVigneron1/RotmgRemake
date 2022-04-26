#pragma once

#include "Enemy.h"

class ProjectileManager;
class EnemyManager;

class Golem final : public Enemy
{


public:
	Golem(ProjectileManager* projectileManager, EnemyManager* enemyManager, const Point2f& position);
	Golem(const Golem& other) = delete;
	Golem& operator=(const Golem& other) = delete;
	Golem(Golem&& other) = delete;
	Golem& operator=(Golem&& other) = delete;
	virtual ~Golem();

	virtual void Update(float elapsedSec, const Point2f& playerPos) override;
	virtual void Draw() const override;
private:

};

