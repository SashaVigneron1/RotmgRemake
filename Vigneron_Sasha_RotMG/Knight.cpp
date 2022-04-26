#include "pch.h"
#include "Inventory.h"
#include "Game.h"
#include "Knight.h"

Knight::Knight(ProjectileManager* projectileManager, ItemBagManager* pItemBagManager, Game* game)
	: Character{ 15.0f, 300.0f, 1000.0f, Color4f{0.8f,0.8f,0.8f,1.0f},
				projectileManager, pItemBagManager, game,
				"Resources/Images/Classes/Knight/ShootingSprites.png",
				"Resources/Images/Classes/Knight/WalkingSprites.png",
				"Resources/Images/Classes/Knight/Idle.png"
				, new SoundEffect{"Resources/Sounds/Classes/Knight/knight_shoot.wav"}
				, new SoundEffect{"Resources/Sounds/Classes/Knight/knight_death.wav"}
				, new SoundEffect{"Resources/Sounds/Classes/Knight/knight_hit.wav"}
				}
	, m_SpecialBulletDamage{ 30.f }
	, m_SpecialBulletRange{ 200.0f }
	, m_SpecialBulletSpeed{ 1000.0f }
	, m_NrOfBullets{ 8 }
	, m_AccCooldownTime{}
	, m_CooldownTime{ 1.0f }
	, m_Range{ 200.0f }
	, m_SpecialBulletColor{ 0.8f,0.8f,0.8f,1.0f }
	, m_ManaNeeded{ 30.f }
{
	m_HorSpeed = 600.f;
	m_VerSpeed = 600.f;
	m_HealthRegenerationPerSecond = 3.f;
	m_ManaRegenerationPerSecond = 2.f;
	m_TimeBetweenShots = 0.18f;
}

Knight::~Knight()
{

}

void Knight::PerformSpecialAttack(const Point2f& mousePos)
{
	//Create a bunch of projectiles in circle
	float distance{ sqrtf((mousePos.x - m_Position.x) * (mousePos.x - m_Position.x)
					+ (mousePos.y - m_Position.y) * (mousePos.y - m_Position.y)) };
	if (distance <= m_Range)
	{
		if (m_AccCooldownTime >= m_CooldownTime && m_Mana >= m_ManaNeeded)
		{
			for (int i{}; i < m_NrOfBullets; ++i)
			{
				float angle{};
				angle = (float(2 * M_PI) / m_NrOfBullets) * i;
				float cos{ std::cos(angle) };
				float sin{ std::sin(angle) };

				m_pProjectileManager->AddProjectile(m_SpecialBulletDamage + m_pInventory->GetAttackBonus(), m_SpecialBulletRange, mousePos,
					Vector2f{ sin, cos }.Normalized() * m_SpecialBulletSpeed, m_SpecialBulletColor, true);
			}
			m_Mana -= m_ManaNeeded;
			m_AccCooldownTime -= m_CooldownTime;
		}
	}
	else
	{
		if (m_AccCooldownTime >= m_CooldownTime && m_Mana >= m_ManaNeeded)
		{
			for (int i{}; i < m_NrOfBullets; ++i)
			{
				float angle{};
				angle = (float(2 * M_PI) / m_NrOfBullets) * i;
				float cos{ std::cos(angle) };
				float sin{ std::sin(angle) };

				Vector2f vector{ mousePos - m_Position };
				vector = vector.Normalized() * m_Range;
				Point2f pos{ vector.x + m_Position.x, vector.y + m_Position.y };

				m_pProjectileManager->AddProjectile(m_SpecialBulletDamage, m_SpecialBulletRange, pos,
					Vector2f{ sin, cos }.Normalized() * m_SpecialBulletSpeed, m_SpecialBulletColor, true
				);
			}
			m_Mana -= m_ManaNeeded;
			m_AccCooldownTime -= m_CooldownTime;
		}
	}
}

void Knight::Update(float elapsedSec, const Point2f& mousePos)
{
	Character::Update(elapsedSec, mousePos);
	UpdateSpecialAttack(elapsedSec);
}

void Knight::UpdateSpecialAttack(float elapsedSec)
{
	if (m_AccCooldownTime <= m_CooldownTime)
	{
		m_AccCooldownTime += elapsedSec;
	}
}