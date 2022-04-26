#pragma once
#include "UIManager.h"
#include "SoundStream.h"

class Character;
class ProjectileManager;
class EnemyManager;
class CameraController;
class NoiseGenerator;
class Map;
class ItemBagManager;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );
	
	void HandleDeath();

private:
	// DATA MEMBERS
	const Window m_Window;


	enum class GameState {
		CharacterSelect = 0,
		Playing = 1,
		Paused = 2,
		Dead = 3
	};

	// FUNCTIONS


	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void PrintInfo() const;
	void SelectCharacter();

	void DrawGame() const;
	// VARS


	GameState m_GameState;

	Character* m_pCharacter;
	ProjectileManager* m_pProjectileManager;
	EnemyManager* m_pEnemyManager;
	CameraController* m_pCameraController;
	Map* m_pMap;
	Point2f m_MousePosition;
	UIManager* m_pUIManager;
	ItemBagManager* m_pItemBagManager;

	Texture m_LogoTexture;
	Texture m_CharacterSelectTexture;
	Texture m_DeathScreenTexture;

	SoundStream* m_pBackgroundMusic;

	bool m_IsAutoFireEnabled;
};