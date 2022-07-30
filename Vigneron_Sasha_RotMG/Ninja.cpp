#include "pch.h"
#include "Ninja.h"
#include "Inventory.h"
#include "Game.h"

Ninja::Ninja(ProjectileManager* projectileManager, ItemBagManager* pItemBagManager, Game* game)
	: Character{ 7.0f, 500.0f, 1000.0f, Color4f{0.1f,0.1f,0.4f,1.0f},
				projectileManager, pItemBagManager, game, 
				"Resources/Images/Classes/Ninja/ShootingSprites.png",
				"Resources/Images/Classes/Ninja/WalkingSprites.png",
				"Resources/Images/Classes/Ninja/Idle.png"
				, new SoundEffect{"Resources/Sounds/Classes/Ninja/ninja_shoot.wav"}
				, new SoundEffect{"Resources/Sounds/Classes/Ninja/ninja_death.wav"} 
				, new SoundEffect{"Resources/Sounds/Classes/Ninja/ninja_hit.wav"} 
				}
	, m_AccCooldownTime{}
	, m_CooldownTime{ 3.0f }
	, m_Range{ 600.f }
	, m_ManaNeeded{ 30.f }
{
	m_HorSpeed = 800.f;
	m_VerSpeed = 800.f;
	m_HealthRegenerationPerSecond = 1.f;
	m_ManaRegenerationPerSecond = 2.f;
}

Ninja::~Ninja()
{

}

void Ninja::PerformSpecialAttack(const Point2f& mousePos)
{
	//Create a bunch of projectiles in circle
	float sqrDistance{ (mousePos.x - m_Position.x) * (mousePos.x - m_Position.x)
					+ (mousePos.y - m_Position.y) * (mousePos.y - m_Position.y) };
	if (sqrDistance <= m_Range * m_Range)
	{
		if (m_AccCooldownTime >= m_CooldownTime && m_Mana >= m_ManaNeeded)
		{
			m_Position = mousePos;
			m_Mana -= m_ManaNeeded;
			m_AccCooldownTime -= m_CooldownTime;
		}
	}
	else
	{
		if (m_AccCooldownTime >= m_CooldownTime && m_Mana >= m_ManaNeeded)
		{
			Vector2f vector{ mousePos - m_Position };
			vector = vector.Normalized() * m_Range;
			Point2f pos{ vector.x + m_Position.x, vector.y + m_Position.y };

			m_Position = pos;
			m_Mana -= m_ManaNeeded;
			m_AccCooldownTime -= m_CooldownTime;
		}
	}
}

void Ninja::Update(float elapsedSec, const Point2f& mousePos)
{
	Character::Update(elapsedSec, mousePos);
	UpdateSpecialAttack(elapsedSec);
}

void Ninja::UpdateSpecialAttack(float elapsedSec)
{
	if (m_AccCooldownTime <= m_CooldownTime)
	{
		m_AccCooldownTime += elapsedSec;
	}
}
