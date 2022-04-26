#pragma once
#include "Item.h"
#include <vector>


#include "Weapon.h"
#include "Armor.h"
#include "Special.h"
#include "Ring.h"

class ItemAssets
{
public: 
	ItemAssets();
	ItemAssets(const ItemAssets& other) = delete;
	ItemAssets& operator=(const ItemAssets& other) = delete;
	ItemAssets(ItemAssets&& other) = delete;
	ItemAssets& operator=(ItemAssets&& other) = delete;
	~ItemAssets();

	Item GetItem(const std::string& name);
	Weapon GetWeapon(const std::string& name);
	Special GetSpecial(const std::string& name);
	Armor GetArmor(const std::string& name);
	Ring GetRing(const std::string& name);

	Weapon GetRandomWeapon() const;
	Special GetRandomSpecial() const;
	Armor GetRandomArmor() const;
	Ring GetRandomRing() const;
	Item GetRandomItem() const;
private:
	std::vector<Weapon*> m_pWeapons;
	std::vector<Special*> m_pSpecials;
	std::vector<Armor*> m_pArmors;
	std::vector<Ring*> m_pRings;
	std::vector<Item*> m_pOtherItems;

	Weapon m_NullWeapon;
	Special m_NullSpecial;
	Armor m_NullArmor;
	Ring m_NullRing;
	Item m_NullItem;
};

