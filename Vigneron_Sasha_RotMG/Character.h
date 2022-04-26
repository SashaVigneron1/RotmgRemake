#pragma once
#include "pch.h"
#include "utils.h"
#include "ProjectileManager.h"
#include "SoundEffect.h"

class Inventory;
class Texture;
class ProjectileManager;
class ItemBagManager;
class Game;

class Character
{
	public:
		enum class CharacterState
		{
			idle = 0,
			moving = 1, 
			//shooting = 2
		};
		enum class CharacterDirection
		{
			down = 0,
			left = 1,
			right = 2, 
			up = 3,
			upLeft = 4,
			upRight = 5,
			downLeft = 6,
			downRight = 7 
		};

		Character(float bulletDamage, float bulletRange, float bulletSpeed, Color4f bulletColor, ProjectileManager* projectileManager, ItemBagManager* pItemBagManager, Game* game, std::string shootingSpritesPath, std::string walkingSpritesPath, std::string idleSpritePath, SoundEffect* pFireSound, SoundEffect* pSoundOnDeath, SoundEffect* pSoundOnHit);
		Character(const Character& other) = delete;
		Character& operator=(const Character& other) = delete;
		Character(Character&& other) = delete;
		Character& operator=(Character&& other) = delete;
		virtual ~Character();

		virtual void Update(float elapsedSec, const Point2f& mousePos);
		void DrawPlayer() const;
		void SetShooting(bool isShooting);
		virtual void PerformSpecialAttack(const Point2f& mousePos) = 0;

		Point2f GetPosition() const;
		Point2f& GetPositionReference();
		float GetSize() const;
		CharacterDirection GetDirection() const;
		float GetHealth() const;
		float GetMana() const;
		Inventory* GetInventory() const;

	protected:
		ProjectileManager* m_pProjectileManager;
		Point2f m_Position;


		float m_Health;
		float m_Mana;

		float m_HorSpeed;
		float m_VerSpeed;

		float m_HealthRegenerationPerSecond;
		float m_ManaRegenerationPerSecond;
		//Projectiles
		float m_TimeBetweenShots;
		const float m_BaseDamage;
		const float m_BulletRange;
		const float m_BulletSpeed;
		const Color4f m_BulletColor;

		Inventory* m_pInventory;


	private:
		Game* m_pGame;

		SoundEffect* m_pFireSound;
		SoundEffect* m_pSoundOnHit;
		SoundEffect* m_pSoundOnDeath;

		float m_AccTimeBetweenShots;


		Vector2f m_Velocity;
		CharacterState m_CharacterState;
		CharacterDirection m_CharacterDirection;

		

		//Textures
		const Texture* m_pWalkingSprites;
		const Texture* m_pShootingSprites;
		const Texture* m_pIdleSprite;
		const float m_CharacterSize;
		float m_Size;

		//Functions
		void CheckIfDamaged();
		void UpdateIdle();
		void UpdateMoving(float elapsedSec);
		void UpdateShooting(float elapsedSec, Point2f mousePos);
		void UpdateSprites(float elapsedSec);
		void Regenerate(float elapsedSec);

		//Sprite handling
		bool m_IsShooting;
		int m_CurrentSpriteIndex;
		const float m_FrameSec;
		float m_AccuSec;
		int m_ActFrame;
		const int m_NrFramesPerAnim;
		float m_Scale;
};

