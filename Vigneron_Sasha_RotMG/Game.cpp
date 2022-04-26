#include "pch.h"
#include "Game.h"
#include "Necromancer.h"
#include "Ninja.h"
#include "Knight.h"
#include "ProjectileManager.h"
#include "ItemBagManager.h"
#include "Inventory.h"	
#include "EnemyManager.h"
#include "CameraController.h"
#include "utils.h"
#include "NoiseGenerator.h"
#include "Map.h"

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_MousePosition{Point2f{0,0}}
	, m_IsAutoFireEnabled{ false }
	, m_pMap{ nullptr }
	, m_pCharacter{ nullptr }
	, m_pItemBagManager{ nullptr }
	, m_pProjectileManager{ nullptr }
	, m_pUIManager{ nullptr }
	, m_GameState{ GameState::CharacterSelect }
	, m_LogoTexture{ "Resources/Images/UI/Logo.png" }
	, m_CharacterSelectTexture{ "Resources/Images/UI/CharacterSelect.png" }
	, m_DeathScreenTexture{ "Resources/Images/UI/DeathScreen.png" }
	, m_pBackgroundMusic{ new SoundStream{"Resources/Sounds/BackgroundSong.wav"} }
{
	PrintInfo();
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
	delete m_pBackgroundMusic;
}

void Game::Initialize( )
{
	m_pMap = new Map{};
	m_pItemBagManager = new ItemBagManager{};
	m_pProjectileManager = new ProjectileManager{ m_pMap->GetObstacleManager() };
	m_pCameraController = new CameraController{ m_Window.width, m_Window.height };
	m_pEnemyManager = new EnemyManager{ m_pProjectileManager, m_pMap, m_pItemBagManager };
	m_pBackgroundMusic->SetVolume(25);
	m_pBackgroundMusic->Play(true);
}

void Game::Cleanup( )
{
	if (m_pMap) delete m_pMap;
	m_pMap = nullptr;
	if (m_pCharacter) delete m_pCharacter;
	m_pCharacter = nullptr;
	if (m_pProjectileManager) delete m_pProjectileManager;
	m_pProjectileManager = nullptr;
	if (m_pEnemyManager) delete m_pEnemyManager;
	m_pEnemyManager = nullptr;
	if (m_pCameraController) delete m_pCameraController;
	m_pCameraController = nullptr;
	if (m_pUIManager) delete m_pUIManager;
	m_pUIManager = nullptr;
	if (m_pItemBagManager) delete m_pItemBagManager;
	m_pItemBagManager = nullptr;
}

void Game::Update( float elapsedSec )
{
	const Uint8* state{ SDL_GetKeyboardState(nullptr) };

	switch (m_GameState)
	{
	case Game::GameState::CharacterSelect:
		SelectCharacter();

		break;
	case Game::GameState::Playing:
		m_pCharacter->Update(elapsedSec, utils::ScreenToWorldPoint(m_MousePosition, m_pCharacter->GetPosition(), m_Window, m_pUIManager->GetMenuWidth()));
		if (!(m_GameState == GameState::Dead)) 
		{
			m_pCameraController->Follow(m_pCharacter->GetPosition(), elapsedSec);
			m_pProjectileManager->UpdateProjectiles(elapsedSec);
			m_pMap->Update(m_pCharacter->GetPositionReference(), m_pCharacter->GetSize(), m_pCharacter->GetDirection());
			m_pEnemyManager->Update(elapsedSec, m_pCharacter->GetPosition());
			m_pUIManager->SetHealth(m_pCharacter->GetHealth());
			m_pUIManager->SetMana(m_pCharacter->GetMana());
			m_pItemBagManager->IsOnTopOfBag(Rectf{ m_pCharacter->GetPosition().x - m_pCharacter->GetSize() / 2.f
													, m_pCharacter->GetPosition().y - m_pCharacter->GetSize() / 2.f
													, m_pCharacter->GetSize(), m_pCharacter->GetSize() });
			m_pCharacter->GetInventory()->UpdateItemPositions(m_MousePosition, false);
			m_pItemBagManager->UpdateItemPositions(m_MousePosition, false);
			m_pItemBagManager->AddTimers(elapsedSec);
		}
		break;
	case Game::GameState::Paused:
		break;
	case Game::GameState::Dead:

		if (state[SDL_SCANCODE_RETURN]) 
		{
			Initialize();
			m_GameState = GameState::CharacterSelect;
		}
		break;
	default:
		break;
	}
	
}

void Game::Draw( ) const
{
	ClearBackground();


	switch (m_GameState)
	{
	case Game::GameState::CharacterSelect:
		m_CharacterSelectTexture.Draw(Rectf{ 0,0,m_Window.width, m_Window.height });
		break;
	case Game::GameState::Playing:
		DrawGame();
		break;
	case Game::GameState::Paused:
		DrawGame();
		utils::SetColor(Color4f{ 0,0,0,0.5f });
		utils::FillRect(0, 0, m_Window.width, m_Window.height);
		m_LogoTexture.Draw(Point2f{ m_Window.width / 2.f - m_LogoTexture.GetWidth() / 2.f, m_Window.height / 2.f - m_LogoTexture.GetHeight() / 2.f });
		break;
	case Game::GameState::Dead:
		m_DeathScreenTexture.Draw(Rectf{ 0,0,m_Window.width, m_Window.height });
		break;
	default:
		break;
	}

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
		
		case SDLK_i:
			PrintInfo();
			break;
		case SDLK_p:
			if (m_GameState == GameState::Playing) 
			{
				m_IsAutoFireEnabled = !m_IsAutoFireEnabled;
				if (m_IsAutoFireEnabled)
				{
					m_pCharacter->SetShooting(true);
				}
				else
				{
					m_pCharacter->SetShooting(false);
				}
			}
			
			break;
		case SDLK_ESCAPE:
			if (m_GameState == GameState::Playing || m_GameState == GameState::Paused) 
			{
				if (m_GameState == GameState::Paused)
				{
					m_GameState = GameState::Playing;
					m_pBackgroundMusic->Resume();
				}
				else
				{
					m_GameState = GameState::Paused;
					m_pBackgroundMusic->Pause();
				}
			}
			break;
		case SDLK_KP_PLUS:
			if (m_GameState == GameState::Playing)
			{
				m_pCameraController->ZoomIn();
			}
			break;
		case SDLK_KP_MINUS:
			if (m_GameState == GameState::Playing)
			{
				m_pCameraController->ZoomOut();
			}
			break;
		case SDLK_KP_MULTIPLY:
			if (m_GameState == GameState::Playing)
			{
				m_pCameraController->ResetZoom();
			}
			break;
	}		
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_MousePosition = Point2f{ float(e.x), float(m_Window.height - e.y) };
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		if (m_GameState == GameState::Playing)
		{
			if (!m_IsAutoFireEnabled)
			{
				m_pCharacter->SetShooting(true);
			}
			m_pCharacter->GetInventory()->UpdateItemPositions(m_MousePosition, true);
			m_pItemBagManager->UpdateItemPositions(m_MousePosition, true);
		}
		break;
	case SDL_BUTTON_RIGHT:
		if (m_GameState == GameState::Playing) 
		{
			m_pCharacter->PerformSpecialAttack(utils::ScreenToWorldPoint(m_MousePosition, m_pCharacter->GetPosition(), m_Window, m_pUIManager->GetMenuWidth()));
		}
		break;
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		if (m_GameState == GameState::Playing) 
		{
			if (!m_IsAutoFireEnabled)
			{
				m_pCharacter->SetShooting(false);
			}

			m_pCharacter->GetInventory()->ReleaseItem(m_MousePosition, m_pCharacter->GetPosition(), m_pItemBagManager->GetHeldItem());
		}
		break;
	case SDL_BUTTON_RIGHT:
		break;
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::PrintInfo() const 
{
	system("cls"); // Clear the console | Source: https://www.youtube.com/watch?v=-cThYucewG0&ab_channel=FahadJamshad

	std::cout << "Welcome to my version of Realm of the Mad God!\n";

	std::cout << "\n";
	std::cout << "\tDescription:\n";
	std::cout << "\t\tYour goal is to get stronger!\n";
	std::cout << "\t\tYou do this by killing enemies!\n";
	std::cout << "\t\tThey might drop some loot bags!\n";
	std::cout << "\t\tCheck the bags to see if they contain something usefull!\n";
	std::cout << "\t\tOf course, you must avoid enemy projectiles, or you will die (permadeath)!\n";
	
	std::cout << "\n";
	std::cout << "\tControls:\n";
	std::cout << "\t\t'W-A-S-D': Move your character.\n";
	std::cout << "\t\t'P': Toggles autofire.\n";
	std::cout << "\t\t'NUMPAD +':  Zoom-In.\n";
	std::cout << "\t\t'NUMPAD -':  Zoom-Out.\n";
	std::cout << "\t\t'NUMPAD x':  Reset Zoom.\n";
}

void Game::SelectCharacter()
{
	const Uint8* state{ SDL_GetKeyboardState(nullptr) };

	if (state[SDL_SCANCODE_1]) 
	{
		m_pCharacter = new Necromancer{ m_pProjectileManager, m_pItemBagManager, this};
		m_GameState = GameState::Playing;
		m_pUIManager = new UIManager{ m_Window, m_pCharacter->GetInventory() };
	}
	else if (state[SDL_SCANCODE_2]) 
	{
		m_pCharacter = new Ninja{ m_pProjectileManager, m_pItemBagManager, this };
		m_GameState = GameState::Playing;
		m_pUIManager = new UIManager{ m_Window, m_pCharacter->GetInventory() };
	}
	else if (state[SDL_SCANCODE_3]) 
	{
		m_pCharacter = new Knight{ m_pProjectileManager, m_pItemBagManager, this };
		m_GameState = GameState::Playing;
		m_pUIManager = new UIManager{ m_Window, m_pCharacter->GetInventory() };
	}
	
}

void Game::HandleDeath()
{
	m_GameState = GameState::Dead;
	Cleanup();
}

void Game::DrawGame() const
{
	m_pCameraController->OpenDrawingMatrix(m_pUIManager->GetMenuWidth() / 2);
		m_pMap->DrawMap(m_pCharacter->GetPosition());
		m_pProjectileManager->DrawProjectiles();
		m_pItemBagManager->DrawItemBags();
		m_pEnemyManager->DrawEnemies(m_pCharacter->GetPosition());
		m_pCharacter->DrawPlayer();
	m_pCameraController->CloseDrawingMatrix();

	m_pUIManager->DrawUI();
	m_pItemBagManager->DrawUI();
}
