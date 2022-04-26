#pragma once
#include "pch.h"
#include "utils.h"

class ProjectileManager;

class Projectile final
{
public: 
	Projectile(ProjectileManager* projectileManager, float damage, float range, const Point2f& startingPosition, const Vector2f& velocity, const Color4f& color, bool isPlayerProjectile);
	Projectile(const Projectile& other) = delete;
	Projectile& operator=(const Projectile& other) = delete;
	Projectile(Projectile&& other) = delete;
	Projectile& operator=(Projectile&& other) = delete;
	~Projectile();

	void Update(float elapsedSec);
	void Draw() const;
	bool CheckOverlap(const Rectf& rect);
	bool IsPlayerProjectile() const;

	float GetDamage() const;
	Rectf GetRect() const;

private: 
	const float m_Width;
	const float m_Height;

	const float m_Damage;
	const float m_Range;
	const Point2f m_StartPosition;
	const Vector2f m_Velocity;
	const Color4f m_Color;
	const float m_Angle;
	const bool m_IsPlayerProjectile;
	Point2f m_Position;

	ProjectileManager* m_pProjectileManager;
};

