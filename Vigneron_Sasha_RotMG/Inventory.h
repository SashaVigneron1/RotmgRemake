#pragma once
#include <vector>
#include "ItemAssets.h"
#include "Item.h"

#include "Weapon.h"
#include "Special.h"
#include "Armor.h"
#include "Ring.h"


class Item;
class ItemBagManager;

class Inventory final
{

public:
	Inventory(ItemBagManager* pItemBagManager);
	Inventory(const Inventory& other) = delete;
	Inventory& operator=(const Inventory& other) = delete;
	Inventory(Inventory&& other) = delete;
	Inventory& operator=(Inventory&& other) = delete;
	~Inventory();

	void Draw() const;
	float GetDefenseBonus() const;
	float GetAttackBonus() const;

	void UpdateItemPositions(const Point2f& position, bool isClick);

	void ReleaseItem(const Point2f& mousePosition, const Point2f& playerPosition, Item* pItem = nullptr);
	void DropItem(Item* pItem, const Point2f& position);
	void RemoveItem(Item* pItem);
private:
	const int m_Columns;
	const int m_Rows;

	bool m_IsHoldingItem;
	Item* m_HeldItem;

	Rectf m_GearZone;
	Rectf m_InventoryZone;

	Weapon* m_pActiveWeapon;
	Special* m_pActiveSpecial;
	Armor* m_pActiveArmor;
	Ring* m_pActiveRing;

	std::vector<Item*> m_pItems;

	ItemAssets m_ItemAssets;
	ItemBagManager* m_pItemBagManager;

	bool AddItem(Item* pItem);
	bool ContainsItem(Item* pItem);
};

