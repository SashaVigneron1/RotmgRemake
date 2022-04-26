#pragma once
#include "Character.h"

class Game;

class Necromancer final : public Character
{
public:
	Necromancer(ProjectileManager* projectileManager, ItemBagManager* pItemBagManager, Game* game);
	Necromancer(const Necromancer& other) = delete;
	Necromancer& operator=(const Necromancer& other) = delete;
	Necromancer(Necromancer&& other) = delete;
	Necromancer& operator=(Necromancer&& other) = delete;
	virtual ~Necromancer() override;

	virtual void PerformSpecialAttack(const Point2f& mousePos) override;
	virtual void Update(float elapsedSec, const Point2f& mousePos) override;
private:
	const float m_CooldownTime;
	float m_AccCooldownTime;
	const float m_ManaNeeded;
	const float m_HealthRestore;

	void UpdateSpecialAttack(float elapsedSec);

};

