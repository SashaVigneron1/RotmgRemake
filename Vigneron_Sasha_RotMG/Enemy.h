#pragma once
#include "SoundStream.h"
#include "SoundEffect.h"

class Texture;
class ProjectileManager;
class EnemyManager;

class Enemy
{
public:
	enum class EnemyState
	{
		idle = 0,
		wandering = 1,
		attacking = 2
	};
	enum class EnemyDirection 
	{
		left = 0,
		right = 1
	};

	Enemy(const std::string& walkingSpritesPath, const std::string& shootingSpritesPath, const std::string& idleSpritesPath, ProjectileManager* projectileManager, EnemyManager* enemyManager, const Point2f& position, const std::string& pSoundOnDeathFilePath, const std::string& pSoundOnHitFilePath);
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& other) = delete;
	virtual ~Enemy();

	virtual void Draw() const;
	virtual void Update(float elapsedSec, const Point2f& playerPos);
	Point2f GetPosition() const;
	
protected:
	// Shooting
	float m_Range;
	float m_MaxDistanceToPlayer;
	float m_BulletDamage;
	float m_BulletRange;
	float m_BulletSpeed;
	Color4f m_BulletColor;
	float m_TimeBetweenShots;

	//Other
	float m_AttackingMovementSpeed;
	float m_WanderMovementSpeed;
	float m_Scale;

	float m_Defense;

private:

	float m_Health;
	EnemyManager* m_pEnemyManager;
	SoundEffect* m_pSoundOnHit;
	std::string m_SoundOnDeathFilePath;

	// Animations
	Texture* m_pWalkingSprite;
	Texture* m_pShootingSprite;
	Texture* m_pIdleSprite;
	float m_AccTime;
	int m_FramesPerSecond;
	int m_CurrentIndex;
	int m_NrOfFrames;

	float m_AccTimeBetweenShots;
	ProjectileManager* m_pProjectileManager;

	// Movement
	EnemyState m_EnemyState;
	EnemyDirection m_EnemyDirection;
	Point2f m_Position;
	Point2f m_TargetPosition;
	
	float m_IdleTime;
	float m_AccIdleTime;


	void DrawHealth() const;

	void CheckIfDamaged();
	void CheckDirection();
	void UpdateIdle(float elapsedSec);
	void UpdateWander(float elapsedSec);
	void UpdateAttacking(float elapsedSec, const Point2f& playerPos);
	void UpdateSprites(float elapsedSec);
	void WalkTowardsTarget(float elapsedSec, float movementSpeed, float minDistance);
};

