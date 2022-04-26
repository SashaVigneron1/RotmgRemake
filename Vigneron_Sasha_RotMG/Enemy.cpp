#include "pch.h"
#include "Enemy.h"
#include "Texture.h"
#include "ProjectileManager.h"
#include "EnemyManager.h"

Enemy::Enemy(const std::string& walkingSpritesPath, const std::string& shootingSpritesPath, const std::string& idleSpritesPath
			, ProjectileManager* projectileManager, EnemyManager* enemyManager, const Point2f& position
			, const std::string& pSoundOnDeathFilePath, const std::string& pSoundOnHitFilePath)
	: m_pEnemyManager{ enemyManager }
	, m_pWalkingSprite{ new Texture{walkingSpritesPath} }
	, m_pShootingSprite{ new Texture{shootingSpritesPath} }
	, m_pIdleSprite{ new Texture{idleSpritesPath} }
	, m_BulletSpeed{1000.f}
	, m_BulletDamage{20.f}
	, m_BulletRange{ 500.f }
	, m_BulletColor{ Color4f{1,1,1,1} }
	, m_MaxDistanceToPlayer{500.f}
	, m_Range{500.f}
	, m_TimeBetweenShots{0.2f}
	, m_AccTimeBetweenShots{}
	, m_pProjectileManager{ projectileManager }
	, m_Position{ position }
	, m_TargetPosition{ Point2f(0.f, 0.f) }
	, m_AttackingMovementSpeed{600.0f}
	, m_WanderMovementSpeed{200.0f}
	, m_AccTime{}
	, m_FramesPerSecond{4}
	, m_NrOfFrames{2}
	, m_CurrentIndex{}
	, m_Scale{ 7.0f }
	, m_EnemyDirection{ EnemyDirection::left }
	, m_EnemyState{ EnemyState::idle }
	, m_IdleTime{}
	, m_AccIdleTime{}
	, m_Health{100.f}
	, m_Defense{0.f}
	, m_SoundOnDeathFilePath{ pSoundOnDeathFilePath }
	, m_pSoundOnHit{ new SoundEffect{pSoundOnHitFilePath} }
{
	m_pSoundOnHit->SetVolume(50);
}

Enemy::~Enemy()
{
	delete m_pWalkingSprite;
	delete m_pShootingSprite;
	delete m_pIdleSprite;
	delete m_pSoundOnHit;
}

void Enemy::Draw() const
{
	int column{ m_CurrentIndex };
	int nrOfRows{ 2 };
	int row{ int(m_EnemyDirection) }; // Direction: Left - bottom row | right - top row
	
	Rectf srcRect{};
	srcRect.width = m_pWalkingSprite->GetWidth() / m_NrOfFrames;
	srcRect.height = m_pWalkingSprite->GetHeight() / nrOfRows;
	srcRect.left = column * srcRect.width;
	srcRect.bottom = row * srcRect.height;
	 
	Rectf destRect{ };
	destRect.bottom = m_Position.y;
	destRect.left = m_Position.x;
	destRect.width = srcRect.width * m_Scale;
	destRect.height = srcRect.height * m_Scale;

	switch (m_EnemyState)
	{
	case Enemy::EnemyState::idle:
		destRect.width = m_pIdleSprite->GetWidth() * m_Scale;
		destRect.height = m_pIdleSprite->GetHeight() * m_Scale;
		m_pIdleSprite->Draw(destRect);
		break;
	case Enemy::EnemyState::wandering:
		m_pWalkingSprite->Draw(destRect, srcRect);
		break;
	case Enemy::EnemyState::attacking:
		m_pShootingSprite->Draw(destRect, srcRect);
		break;
	default:
		break;
	}

	DrawHealth();
}

void Enemy::Update(float elapsedSec, const Point2f& playerPos)
{
	UpdateSprites(elapsedSec);

	CheckIfDamaged();
	CheckDirection();
	// Check if close to player
	if (utils::GetDistance(m_Position, playerPos) <= m_MaxDistanceToPlayer)
	{
		m_TargetPosition = playerPos;
		m_EnemyState = EnemyState::attacking;
	}

	switch (m_EnemyState)
	{
	case Enemy::EnemyState::idle:
		UpdateIdle(elapsedSec);
		break;
	case Enemy::EnemyState::wandering:
		UpdateWander(elapsedSec);
		break;
	case Enemy::EnemyState::attacking:
		UpdateAttacking(elapsedSec, playerPos);
		break;
	}

}

Point2f Enemy::GetPosition() const
{
	return m_Position;
}

void Enemy::CheckIfDamaged()
{
	// Check if hit
	float width{ (m_pWalkingSprite->GetWidth() / m_NrOfFrames) * m_Scale };
	int nrOfRows{ 2 };
	float height{ (m_pWalkingSprite->GetHeight() / nrOfRows) * m_Scale };
	Rectf enemyRect{ m_Position.x, m_Position.y, width, height };

	float damageToTake{ m_pProjectileManager->GetDamageIfHit(enemyRect, false) };
	if (damageToTake != -1.f) // -1 is the value returned when no damage should be taken
	{
		// Get Hit
		damageToTake -= m_Defense;
		if (damageToTake < 0)
		{
			damageToTake = 1.f;
		}
		m_pSoundOnHit->Play(0);
		m_Health -= damageToTake;
		if (m_Health <= 0.f)
		{
			// Die
			m_pEnemyManager->PlayEnemyDeathSound(m_SoundOnDeathFilePath);
			m_pEnemyManager->KillEnemy(this);
		}
	}
}

void Enemy::CheckDirection()
{
	// Check Direction

	if (m_TargetPosition.x < m_Position.x)
	{
		m_EnemyDirection = Enemy::EnemyDirection::left;
	}
	else
	{
		m_EnemyDirection = Enemy::EnemyDirection::right;
	}
}


void Enemy::UpdateIdle(float elapsedSec)
{
	m_AccIdleTime += elapsedSec;
	if (m_AccIdleTime >= m_IdleTime)
	{
		int maxRange{ 1000 };
		float xDistance{ rand() % maxRange + 1.f };
		(rand() % 2 == 0) ? xDistance *= -1.f : xDistance *= 1.f;
		float yDistance{ rand() % maxRange + 1.f };
		(rand() % 2 == 0) ? yDistance *= -1.f : yDistance *= 1.f;
		m_TargetPosition = Point2f{ m_Position.x + xDistance, m_Position.y + yDistance };
		m_EnemyState = EnemyState::wandering;
	}
}

void Enemy::UpdateWander(float elapsedSec)
{
	WalkTowardsTarget(elapsedSec, m_WanderMovementSpeed, 0.f);
	 
	// If at target
	// Go back to idle
	float precision{ 5.f };
	if (abs(m_Position.x - m_TargetPosition.x) <= precision
		&& abs(m_Position.y - m_TargetPosition.y))
	{
		float minTime{ 2.f };
		float maxTime{ 5.f };

		m_IdleTime = (float(rand() % int(((maxTime - minTime) * 100) + 1)) / 100.f + minTime);
		m_AccIdleTime = 0.f;
		m_EnemyState = EnemyState::idle;
	}
}

void Enemy::UpdateAttacking(float elapsedSec, const Point2f& playerPos)
{
	// walk towards player
	WalkTowardsTarget(elapsedSec, m_AttackingMovementSpeed, 50.f);
	 
	// shoot at player
	if (m_AccTimeBetweenShots < m_TimeBetweenShots)
	{
		m_AccTimeBetweenShots += elapsedSec;
	}
	else
	{
		//Shoot projectile
		Vector2f direction{};
		direction.x = m_Position.x - playerPos.x;
		direction.x *= -1; // Because we have to shoot towards the player, instead of away from him
		direction.y = m_Position.y - playerPos.y;
		direction.y *= -1;
		Point2f startPosition{};
		startPosition.x = m_Position.x + (m_pIdleSprite->GetWidth()*m_Scale) / 2.f;
		startPosition.y = m_Position.y + (m_pIdleSprite->GetHeight() * m_Scale) / 2.f;
		m_pProjectileManager->AddProjectile(m_BulletDamage, m_BulletRange, startPosition, direction.Normalized() * m_BulletSpeed, m_BulletColor, false);
		
		m_AccTimeBetweenShots -= m_TimeBetweenShots;
	}

	// If Player Too far, go to idle;
	if (utils::GetDistance(m_Position, playerPos) >= m_MaxDistanceToPlayer)
	{
		m_EnemyState = EnemyState::idle;
		m_AccIdleTime = 0.f;
	}
}

void Enemy::UpdateSprites(float elapsedSec)
{
	m_AccTime += elapsedSec;
	if (m_AccTime >= (1.f / m_FramesPerSecond))
	{
		++m_CurrentIndex %= m_NrOfFrames;
		m_AccTime = 0.f;
	}
}

void Enemy::DrawHealth() const
{
	// Empty HealthBar
	utils::SetColor(Color4f{ 1, 0, 0, 1 });
	Rectf healthBar{};
	float xOffset{ (m_pIdleSprite->GetWidth() * m_Scale) / 2.f};
	float yOffset{20.f};
	healthBar.width = 50.f;
	healthBar.height = 10.f;
	healthBar.left = m_Position.x + xOffset - (healthBar.width / 2.f);
	healthBar.bottom = m_Position.y - yOffset - healthBar.height;
	utils::FillRect(healthBar);
	
	// Actual Health
	healthBar.width *= (m_Health / 100.0f);
	utils::SetColor(Color4f{ 0, 1, 0, 1 });
	utils::FillRect(healthBar);

	// Frame
	healthBar.width /= (m_Health / 100.0f); // This is to revert to the width of the full bar
	utils::SetColor(Color4f{ 0, 0, 0, 1 });
	utils::DrawRect(healthBar, 2.f);
}

void Enemy::WalkTowardsTarget(float elapsedSec, float movementSpeed, float minDistance) 
{

	if (abs(m_Position.x - m_TargetPosition.x) >= minDistance
		&& abs(m_Position.y - m_TargetPosition.y) >= minDistance) 
	{
		// Calculate difference to target
		// Normalize
		// Times movementspeed
		Vector2f direction{};
		direction.x = m_Position.x - m_TargetPosition.x;
		direction.x *= -1; // Because we have to shoot towards the player, instead of away from him
		direction.y = m_Position.y - m_TargetPosition.y;
		direction.y *= -1;
		direction = direction.Normalized();
		direction *= movementSpeed;

		m_Position.x += direction.x * elapsedSec;
		m_Position.y += direction.y * elapsedSec;
	}

	
}