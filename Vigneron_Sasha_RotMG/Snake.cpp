#include "pch.h"
#include "Snake.h"

Snake::Snake(ProjectileManager* projectileManager, EnemyManager* enemyManager, const Point2f& position)
	:Enemy("Resources/Images/Enemies/Snake/Snake_Walking.png"
			, "Resources/Images/Enemies/Snake/Snake_Shooting.png"
			, "Resources/Images/Enemies/Snake/Snake_Idle.png"
			, projectileManager, enemyManager, position
			, "Resources/Sounds/Enemies/Snake/snake_death.wav"
			, "Resources/Sounds/Enemies/Snake/snake_hit.wav")
{
	m_Scale = 4.f;
	m_Range = 500.f;
	m_MaxDistanceToPlayer = 500.f;
	m_BulletDamage = 7.f;
	m_BulletRange = 500.f;
	m_BulletSpeed = 1000.0f;
	m_BulletColor = Color4f{ 0, 0.8f, 0.3f, 1 };
	m_TimeBetweenShots = 0.2f;

	m_AttackingMovementSpeed = 600.f;
	m_WanderMovementSpeed = 200.f;
}

Snake::~Snake()
{
	
}

void Snake::Update(float elapsedSec, const Point2f& playerPos)
{
	Enemy::Update(elapsedSec, playerPos);
}

void Snake::Draw() const
{
	Enemy::Draw();
}

