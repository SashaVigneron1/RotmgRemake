#include "pch.h"
#include "Necromancer.h"
#include "Texture.h"
#include "Game.h"

Necromancer::Necromancer(ProjectileManager* projectileManager, ItemBagManager* pItemBagManager, Game* game)
	: Character{ 10.0f, 700.0f, 1000.0f, Color4f{1.0f,0.0f,0.8f,1.0f}, 
				projectileManager, pItemBagManager, game, 
				"Resources/Images/Classes/Necromancer/ShootingSprites.png", 
				"Resources/Images/Classes/Necromancer/WalkingSprites.png",
				"Resources/Images/Classes/Necromancer/Idle.png"
				, new SoundEffect{"Resources/Sounds/Classes/Necromancer/necromancer_shoot.wav"}
				, new SoundEffect{"Resources/Sounds/Classes/Necromancer/necromancer_death.wav"}
				, new SoundEffect{"Resources/Sounds/Classes/Necromancer/necromancer_hit.wav"}
				}
	, m_AccCooldownTime{}
	, m_CooldownTime{ 2.0f }
	, m_ManaNeeded{ 20.f }
	, m_HealthRestore{ 50.f }
{
	m_HorSpeed = 700.f;
	m_VerSpeed = 700.f;
	m_HealthRegenerationPerSecond = 1.5f;
	m_ManaRegenerationPerSecond = 2.5f;
	m_TimeBetweenShots = 0.3f;
}

Necromancer::~Necromancer()
{
	
}

void Necromancer::PerformSpecialAttack(const Point2f& mousePos)
{
	if (m_AccCooldownTime >= m_CooldownTime && m_Mana >= m_ManaNeeded)
	{
		m_Health += m_HealthRestore;
		if (m_Health > 100) m_Health = 100;
		m_Mana -= m_ManaNeeded;
		m_AccCooldownTime -= m_CooldownTime;
	}
}

void Necromancer::Update(float elapsedSec, const Point2f& mousePos)
{
	Character::Update(elapsedSec, mousePos);
	UpdateSpecialAttack(elapsedSec);
}

void Necromancer::UpdateSpecialAttack(float elapsedSec) 
{
	if (m_AccCooldownTime <= m_CooldownTime) 
	{
		m_AccCooldownTime += elapsedSec;
	}
}
