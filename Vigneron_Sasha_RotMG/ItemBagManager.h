#pragma once

#include <vector>
#include "ItemAssets.h"

class ItemBag;

class ItemBagManager final
{
public:
	ItemBagManager();
	ItemBagManager(const ItemBagManager& other) = delete;
	ItemBagManager& operator=(const ItemBagManager& other) = delete;
	ItemBagManager(ItemBagManager&& other) = delete;
	ItemBagManager& operator=(ItemBagManager&& other) = delete;
	~ItemBagManager();

	void AddItemBag(ItemBag* pItemBag);
	void AddRandomItemBag(const Point2f& position);
	void DrawItemBags() const;
	void DrawUI() const;
	bool IsOnTopOfBag(const Rectf& rect);
	void CheckIfBagsAreEmpty();

	void DestroyBag(ItemBag* pItemBag);
	void UpdateItemPositions(const Point2f& position, bool isClick);

	Item* GetHeldItem();

	void AddTimers(float elapsedSec);
private:

	std::vector<ItemBag*> m_pItemBags;
	ItemAssets m_ItemAssets;

};

