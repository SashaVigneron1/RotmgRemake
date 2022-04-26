#include "pch.h"
#include "Medusa.h"

Medusa::Medusa(ProjectileManager* projectileManager, EnemyManager* enemyManager, const Point2f& position)
	:Enemy("Resources/Images/Enemies/Medusa/Medusa_Walking.png"
		, "Resources/Images/Enemies/Medusa/Medusa_Shooting.png"
		, "Resources/Images/Enemies/Medusa/Medusa_Idle.png"
		, projectileManager, enemyManager, position
		, "Resources/Sounds/Enemies/Medusa/medusa_death.wav" 
		, "Resources/Sounds/Enemies/Medusa/medusa_hit.wav")
{
	m_Scale = 8.f;
	m_Range = 500.f;
	m_MaxDistanceToPlayer = 400.f;
	m_BulletDamage = 50.f;
	m_BulletRange = 700.f;
	m_BulletSpeed = 1000.0f;
	m_BulletColor = Color4f{ 0, 0.8f, 0.6f, 1 };
	m_TimeBetweenShots = 0.2f;

	m_Defense = 20.f;

	m_AttackingMovementSpeed = 500.f;
	m_WanderMovementSpeed = 150.f;
}

Medusa::~Medusa()
{

}

void Medusa::Update(float elapsedSec, const Point2f& playerPos)
{
	Enemy::Update(elapsedSec, playerPos);
}

void Medusa::Draw() const
{
	Enemy::Draw();
}

