#include "pch.h"
#include "UIManager.h"
#include "utils.h"
#include "Inventory.h"

UIManager::UIManager(const Window& window, Inventory* inventory)
	: m_MenuTexture{ "Resources/Images/UI/Menu/UIMenu.png" }
	, m_Window{ window }
	, m_Health{ 100.f }
	, m_Mana{ 100.f }
	, m_pInventory{ inventory }
{
}

UIManager::~UIManager()
{
}

void UIManager::Update()
{
}

void UIManager::DrawUI() const
{
	//// Health Bar
	//Empty HealthBar
	Rectf healthUIRect{};
	float borderOffset{ 5.f };
	healthUIRect.left = m_Window.width - GetMenuWidth() + borderOffset;
	healthUIRect.bottom = 394.f;
	healthUIRect.width = GetMenuWidth() - borderOffset*2;
	healthUIRect.height = 21.f;
	utils::SetColor(Color4f{ (36/255.f), (185/255.f), (74/255.f), 1.f });
	utils::FillRect(healthUIRect);

	//Actual HealthBar
	if (m_Health > 0.f) 
	{
		healthUIRect.width *= m_Health / 100.f;
		utils::SetColor(Color4f{ 0, 1, 0, 1 });
		utils::FillRect(healthUIRect);
	}


	//// ManaBar
	//Empty ManaBar
	Rectf ManaUIRect{};
	ManaUIRect.left = m_Window.width - GetMenuWidth() + borderOffset;
	ManaUIRect.bottom = 367.f;
	ManaUIRect.width = GetMenuWidth() - borderOffset * 2;
	ManaUIRect.height = 20.f;
	utils::SetColor(Color4f{ 0.0f, 0.5f, 0.88f, 1.f });
	utils::FillRect(ManaUIRect);

	//Actual ManaBar
	if (m_Mana > 0.f)
	{
		ManaUIRect.width *= m_Mana / 100.f;
		utils::SetColor(Color4f{ 0.f, 0.83f, 0.84f, 1.f });
		utils::FillRect(ManaUIRect);
	}

	//Menu
	m_MenuTexture.Draw(Point2f{ m_Window.width - m_MenuTexture.GetWidth(), 0.0f });
	m_pInventory->Draw();
}

void UIManager::SetHealth(float health)
{
	m_Health = health;
}
void UIManager::SetMana(float mana)
{
	m_Mana = mana;
}
float UIManager::GetMenuWidth() const 
{
	return m_MenuTexture.GetWidth();
}
