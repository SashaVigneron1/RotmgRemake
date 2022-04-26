#pragma once
#include "Character.h"

class Game;

class Ninja final : public Character
{

public: 
	Ninja(ProjectileManager* projectileManager, ItemBagManager* pItemBagManager, Game* game);
	Ninja(const Ninja& other) = delete;
	Ninja& operator=(const Ninja& other) = delete;
	Ninja(Ninja&& other) = delete;
	Ninja& operator=(Ninja&& other) = delete;
	virtual ~Ninja() override;

	virtual void PerformSpecialAttack(const Point2f& mousePos) override;
	virtual void Update(float elapsedSec, const Point2f& mousePos) override;

private: 
	//Special Attack
	float m_Range;
	float m_CooldownTime;
	float m_AccCooldownTime;
	float m_ManaNeeded;

	void UpdateSpecialAttack(float elapsedSec);
};

