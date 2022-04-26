#pragma once

#include "Texture.h"

class Inventory;

class UIManager final
{
public:
	UIManager(const Window& window, Inventory* inventory);
	UIManager(const UIManager& other) = delete;
	UIManager& operator=(const UIManager& other) = delete;
	UIManager(UIManager&& other) = delete;
	UIManager& operator=(UIManager&& other) = delete;
	~UIManager();

	void Update();
	void DrawUI() const;
	void SetHealth(float health);
	void SetMana(float mana);
	float GetMenuWidth() const;
private:
	const Window& m_Window;
	Texture m_MenuTexture;

	float m_Health;
	float m_Mana;

	Inventory* m_pInventory;
};

