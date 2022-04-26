#include "pch.h"
#include "Golem.h"

Golem::Golem(ProjectileManager* projectileManager, EnemyManager* enemyManager, const Point2f& position)
	:Enemy("Resources/Images/Enemies/Golem/Golem_Walking.png"
		, "Resources/Images/Enemies/Golem/Golem_Shooting.png"
		, "Resources/Images/Enemies/Golem/Golem_Idle.png"
		, projectileManager, enemyManager, position
		, "Resources/Sounds/Enemies/Golem/golem_death.wav"
		, "Resources/Sounds/Enemies/Golem/golem_hit.wav")
{
	m_Scale = 6.f;
	m_Range = 500.f;
	m_MaxDistanceToPlayer = 400.f;
	m_BulletDamage = 120.f;
	m_BulletRange = 700.f;
	m_BulletSpeed = 900.f;
	m_BulletColor = Color4f{ 1, 0.8f, 0.3f, 1 };
	m_TimeBetweenShots = 0.5f;
	m_Defense = 50.f;

	m_AttackingMovementSpeed = 400.f;
	m_WanderMovementSpeed = 200.f;
}

Golem::~Golem()
{

}

void Golem::Update(float elapsedSec, const Point2f& playerPos)
{
	Enemy::Update(elapsedSec, playerPos);
}

void Golem::Draw() const
{
	Enemy::Draw();
}

