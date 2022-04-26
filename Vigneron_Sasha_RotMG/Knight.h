#pragma once
#include "Character.h"

class Game;

class Knight final : public Character
{
public:
	Knight(ProjectileManager* projectileManager, ItemBagManager* pItemBagManager, Game* game);
	Knight(const Knight& other) = delete;
	Knight& operator=(const Knight& other) = delete;
	Knight(Knight&& other) = delete;
	Knight& operator=(Knight&& other) = delete;
	virtual ~Knight() override;

	virtual void PerformSpecialAttack(const Point2f& mousePos) override;
	virtual void Update(float elapsedSec, const Point2f& mousePos) override;

private:
	//Special Attack
	const int m_NrOfBullets;
	const float m_SpecialBulletDamage;
	const float m_SpecialBulletRange;
	const float m_SpecialBulletSpeed;
	const Color4f m_SpecialBulletColor;
	const float m_Range;
	const float m_CooldownTime;
	float m_AccCooldownTime;
	const float m_ManaNeeded;

	void UpdateSpecialAttack(float elapsedSec);
};

