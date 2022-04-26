#pragma once

#include "Item.h"
#include "Texture.h"
#include <vector>

class ItemBag final
{
public: 
	ItemBag(const std::vector<Item*>& items, const Point2f& position);
	ItemBag(const ItemBag& other) = delete;
	ItemBag& operator=(const ItemBag& other) = delete;
	ItemBag(ItemBag&& other) = delete;
	ItemBag& operator=(ItemBag&& other) = delete;
	~ItemBag();

	void UpdateItemPositions(const Point2f& position, bool isClick);
	void ShowUI(bool value);

	void Draw() const;
	void DrawUI() const;
	Rectf GetRect() const;
	int GetSize() const;

	float GetTimer() const;
	void AddTime(float elapsedSec);

	bool IsPlayerOnTop() const;
	Item* GetHeldItem();

	void ReleaseItem();


private: 
	const int m_Columns;
	const int m_Rows;

	bool m_IsHoldingItem;
	Item* m_HeldItem;

	bool m_ShowingUI;
	std::vector<Item*> m_pItems;
	Point2f m_Position;
	Texture m_Texture;
	Texture m_UITexture;

	float m_AccTime;


	bool AddItem(Item* pItem);
	bool ContainsItem(Item* pItem);
	void RemoveItem(Item* pItem);
};

