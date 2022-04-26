#include "pch.h"
#include "Character.h"
#include "Texture.h"
#include "Inventory.h"
#include "Game.h"


using namespace utils;

Character::Character(float bulletDamage, float bulletRange, float bulletSpeed, Color4f bulletColor, ProjectileManager* projectileManager, ItemBagManager* pItemBagManager, Game* game, std::string shootingSpritesPath, std::string walkingSpritesPath, std::string idleSpritePath, SoundEffect* pFireSound, SoundEffect* pSoundOnDeath, SoundEffect* pSoundOnHit)
	: m_HorSpeed{ 700.f }
	, m_VerSpeed{ 700.f }
	, m_Position{ Point2f{0.0f,0.0f} }
	, m_CharacterState{ Character::CharacterState::idle }
	, m_CharacterDirection{ Character::CharacterDirection::down }
	, m_pShootingSprites{ new Texture{shootingSpritesPath} }
	, m_pWalkingSprites{ new Texture{walkingSpritesPath} }
	, m_pIdleSprite{ new Texture{idleSpritePath} }
	, m_CurrentSpriteIndex{}
	, m_NrFramesPerAnim{ 2 }
	, m_AccuSec{}
	, m_ActFrame{}
	, m_FrameSec{0.2f}
	, m_Scale{8.0f}
	, m_IsShooting{}
	, m_TimeBetweenShots{0.3f}
	, m_AccTimeBetweenShots{}
	, m_pProjectileManager{projectileManager}
	, m_BaseDamage{bulletDamage}
	, m_BulletRange{bulletRange}
	, m_BulletSpeed{bulletSpeed}
	, m_BulletColor{bulletColor}
	, m_Health{ 100.f }
	, m_Mana{ 100.f }
	, m_HealthRegenerationPerSecond{ 5.f }
	, m_ManaRegenerationPerSecond{ 2.5f }
	, m_CharacterSize{ 8.f }
	, m_Size{}
	, m_pInventory{ new Inventory{pItemBagManager} }
	, m_pGame{ game }
	, m_pFireSound{ pFireSound }
	, m_pSoundOnDeath{ pSoundOnDeath }
	, m_pSoundOnHit{ pSoundOnHit }
{
	m_Size = m_CharacterSize * m_Scale;
	m_pFireSound->SetVolume(50);
	m_pSoundOnDeath->SetVolume(50);
	m_pSoundOnHit->SetVolume(50);
}

Character::~Character()
{
	delete m_pShootingSprites;
	delete m_pWalkingSprites;
	delete m_pIdleSprite;
	delete m_pInventory;
	delete m_pFireSound;
	delete m_pSoundOnDeath;
	delete m_pSoundOnHit;
}

void Character::Update(float elapsedSec, const Point2f& mousePos)
{

	Regenerate(elapsedSec);

	UpdateSprites(elapsedSec);
	UpdateShooting(elapsedSec, mousePos);

	switch (m_CharacterState)
	{
	case Character::CharacterState::idle:
		UpdateIdle();
		break;
	case Character::CharacterState::moving:
		UpdateMoving(elapsedSec);
		break;
	default:
		break;
	}
	CheckIfDamaged();
}


void Character::CheckIfDamaged()
{
	Rectf playerRect{ m_Position.x - m_Size / 2.f, m_Position.y - m_Size / 2.f, m_Size, m_Size };

	float damageToTake{ m_pProjectileManager->GetDamageIfHit(playerRect, true) };
	if (damageToTake != -1.f)
	{
		// Get Hit
		damageToTake -= m_pInventory->GetDefenseBonus();
		if (damageToTake < 0)
		{
			damageToTake = 1.f;
		}
		m_pSoundOnHit->Play(0);
		m_Health -= damageToTake;
		if (m_Health <= 0.f)
		{
			m_pSoundOnDeath->Play(0);
			m_pGame->HandleDeath();
		}
	}
}

void Character::DrawPlayer() const
{
	int cols{ 4 }; // Always 4 columns for the 4 directions
	int rows{ m_NrFramesPerAnim }; // Always 2 images per action

	if (m_CharacterState == CharacterState::idle && !m_IsShooting) 
	{

		Rectf srcRect{};
		srcRect.width = m_pIdleSprite->GetWidth();
		srcRect.height = m_pIdleSprite->GetHeight();
		srcRect.left = 0.0f;
		srcRect.bottom = srcRect.height;

		Rectf destRect{};
		destRect.width = srcRect.width * m_Scale;
		destRect.height = srcRect.height * m_Scale;
		destRect.left = m_Position.x - destRect.width / 2.0f;
		destRect.bottom = m_Position.y - destRect.height / 2.0f;

		m_pIdleSprite->Draw(destRect, srcRect);
	}
	else 
	{
		Rectf srcRect{};
		srcRect.width = m_pShootingSprites->GetWidth() / cols;		// This is fine, shootingsprites is always in the same form as the walkingsprites
		srcRect.height = m_pShootingSprites->GetHeight() / rows;

		int column{};
		switch (m_CharacterDirection)
		{
		case Character::CharacterDirection::upLeft:
			column = (int)CharacterDirection::left;
			break;
		case Character::CharacterDirection::upRight:
			column = (int)CharacterDirection::right;
			break;
		case Character::CharacterDirection::downLeft:
			column = (int)CharacterDirection::left;
			break;
		case Character::CharacterDirection::downRight:
			column = (int)CharacterDirection::right;
			break;
		default:
			column = (int)m_CharacterDirection; //Dependant on Direction (Front - Left - Right - Back)
			break;
		}
		srcRect.left = column * srcRect.width;
		srcRect.bottom = (m_ActFrame + 1) * srcRect.height;	//Dependant on activeFrame

		Rectf destRect{};
		destRect.width = srcRect.width * m_Scale;
		destRect.height = srcRect.height * m_Scale;
		destRect.left = m_Position.x - destRect.width / 2.0f;
		destRect.bottom = m_Position.y - destRect.height / 2.0f;

		if (m_IsShooting) 
		{
			m_pShootingSprites->Draw(destRect, srcRect);
		}
		else 
		{
			m_pWalkingSprites->Draw(destRect, srcRect);
		}
	}

}

void Character::UpdateIdle() 
{
	const Uint8* state{ SDL_GetKeyboardState(nullptr) };

	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D] 
		|| state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S] 
		|| state[SDL_SCANCODE_SPACE])
	{	
		if (state[SDL_SCANCODE_A]) m_CharacterDirection = Character::CharacterDirection::left;
		if (state[SDL_SCANCODE_D]) m_CharacterDirection = Character::CharacterDirection::right;
		if (state[SDL_SCANCODE_W]) m_CharacterDirection = Character::CharacterDirection::up;
		if (state[SDL_SCANCODE_S]) m_CharacterDirection = Character::CharacterDirection::down;

		m_CharacterState = Character::CharacterState::moving;
	}
}
void Character::UpdateMoving(float elapsedSec) 
{
	const Uint8* state{ SDL_GetKeyboardState(nullptr) };

	bool isInput{ false };

	//Horizontal & Vertical
	if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_W])
	{
	m_CharacterDirection = Character::CharacterDirection::upLeft;
	float horSpeed{ m_HorSpeed * sin(DegreesToRad(45)) };
	float verSpeed{ m_VerSpeed * sin(DegreesToRad(45)) };
	m_Velocity.x = -horSpeed;
	m_Velocity.y = verSpeed;
	isInput = true;
	}
	else if (state[SDL_SCANCODE_D] && state[SDL_SCANCODE_W])
	{
	m_CharacterDirection = Character::CharacterDirection::upRight;
	float horSpeed{ m_HorSpeed * sin(DegreesToRad(45)) };
	float verSpeed{ m_VerSpeed * sin(DegreesToRad(45)) };
	m_Velocity.x = horSpeed;
	m_Velocity.y = verSpeed;
	isInput = true;
	}
	else if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_S])
	{
	m_CharacterDirection = Character::CharacterDirection::downLeft;
	float horSpeed{ m_HorSpeed * sin(DegreesToRad(45)) };
	float verSpeed{ m_VerSpeed * sin(DegreesToRad(45)) };
	m_Velocity.x = -horSpeed;
	m_Velocity.y = -verSpeed;
	isInput = true;
	}
	else if (state[SDL_SCANCODE_D] && state[SDL_SCANCODE_S])
	{
	m_CharacterDirection = Character::CharacterDirection::downRight;
	float horSpeed{ m_HorSpeed * sin(DegreesToRad(45)) };
	float verSpeed{ m_VerSpeed * sin(DegreesToRad(45)) };
	m_Velocity.x = horSpeed;
	m_Velocity.y = -verSpeed;
	isInput = true;
	}
	//Horizontal
	else if (state[SDL_SCANCODE_A])
	{
		m_CharacterDirection = Character::CharacterDirection::left;
		m_Velocity.x = -m_HorSpeed;
		m_Velocity.y = 0.0f;
		isInput = true;
	}
	else if (state[SDL_SCANCODE_D])
	{
		m_CharacterDirection = Character::CharacterDirection::right;
		m_Velocity.x = m_HorSpeed;
		m_Velocity.y = 0.0f;
		isInput = true;
	}
	//Vertical
	else if (state[SDL_SCANCODE_W])
	{
		m_CharacterDirection = Character::CharacterDirection::up;
		m_Velocity.x = 0.0f;
		m_Velocity.y = m_VerSpeed;
		isInput = true;
	}
	else if (state[SDL_SCANCODE_S])
	{
		m_CharacterDirection = Character::CharacterDirection::down;
		m_Velocity.x = 0.0f;
		m_Velocity.y = -m_VerSpeed;
		isInput = true;
	}
	else 
	{
		m_Velocity.x = 0.0f;
		m_Velocity.y = 0.0f;
	}
	

	if (!isInput)
	{
		m_CharacterState = Character::CharacterState::idle;
	}

	m_Position += m_Velocity * elapsedSec;
}
void Character::SetShooting(bool isShooting) 
{
	if (isShooting) 
	{
		m_IsShooting = true;
	}
	else 
	{
		m_IsShooting = false;
	}

}
void Character::UpdateShooting(float elapsedSec, Point2f mousePos)
{
	if (m_AccTimeBetweenShots < m_TimeBetweenShots) 
	{
		m_AccTimeBetweenShots += elapsedSec;
	}
	else 
	{
		if (m_IsShooting)  
		{
			//Shoot projectile
			Vector2f direction{};
			direction.x = m_Position.x - mousePos.x;
			direction.x *= -1; // Because we have to shoot towards the mouse, instead of away from him
			direction.y = m_Position.y - mousePos.y;
			direction.y *= -1;

			m_pProjectileManager->AddProjectile(m_BaseDamage + m_pInventory->GetAttackBonus(), m_BulletRange, m_Position, direction.Normalized()* m_BulletSpeed, m_BulletColor, true);
			m_pFireSound->Play(0);
			m_AccTimeBetweenShots -= m_TimeBetweenShots;
		}
	}
}

void Character::UpdateSprites(float elapsedSec) 
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= m_FrameSec)
	{
		++m_ActFrame %= m_NrFramesPerAnim;
		m_AccuSec -= m_FrameSec;
	}
}

void Character::Regenerate(float elapsedSec)
{
	if (m_Health < 100.f
		&& m_Health > 0.f) 
	{
		m_Health += m_HealthRegenerationPerSecond * elapsedSec;
	}
	if (m_Mana < 100.f) 
	{
		m_Mana += m_ManaRegenerationPerSecond * elapsedSec;
	}
}

Point2f Character::GetPosition() const
{
	return Point2f{ m_Position.x - m_Size / 2.f, m_Position.y - m_Size / 2.f};
}
float Character::GetSize() const 
{
	return m_Size;
}

Character::CharacterDirection Character::GetDirection() const
{
	return m_CharacterDirection;
}

float Character::GetHealth() const
{
	return m_Health;
}

float Character::GetMana() const
{
	return m_Mana;
}

Inventory* Character::GetInventory() const
{
	return m_pInventory;
}

Point2f& Character::GetPositionReference()
{
	return m_Position;
}

