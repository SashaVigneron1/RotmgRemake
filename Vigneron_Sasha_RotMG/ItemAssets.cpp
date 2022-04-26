#include "pch.h"
#include "ItemAssets.h"


ItemAssets::ItemAssets()
	: m_NullItem{ "Null", Item::ItemType::Item, "Resources/Images/Items/null.png" }
	, m_NullWeapon{ "Null",  "Resources/Images/Items/null.png" , 0.f}
	, m_NullSpecial{ "Null",  "Resources/Images/Items/null.png" , 0.f, 0.f}
	, m_NullArmor{ "Null",  "Resources/Images/Items/null.png" , 0.f}
	, m_NullRing{ "Null",  "Resources/Images/Items/null.png" , 0.f, 0.f}

{
	m_pWeapons.push_back(new Weapon{ "Ichimonji", "Resources/Images/Items/Swords/Ichimonji.png", 1.f });
	m_pWeapons.push_back(new Weapon{ "Line Kutter Katana", "Resources/Images/Items/Swords/Line Kutter Katana.png", 10.f });
	m_pWeapons.push_back(new Weapon{ "Masamune", "Resources/Images/Items/Swords/Masamune.png", 20.f });
	m_pWeapons.push_back(new Weapon{ "Sadamune", "Resources/Images/Items/Swords/Sadamune.png", 40.f });

	m_pSpecials.push_back(new Special{ "Basic Star", "Resources/Images/Items/Specials/Basic Star.png", 10.f, 0.f });
	m_pSpecials.push_back(new Special{ "Four-Point Star", "Resources/Images/Items/Specials/Four-Point Star.png", 5.f, 0.f });
	m_pSpecials.push_back(new Special{ "Silver Star", "Resources/Images/Items/Specials/Silver Star.png", 10.f, 10.f });
	m_pSpecials.push_back(new Special{ "Wind Circle", "Resources/Images/Items/Specials/Wind Circle.png", 20.f, 10.f });
	m_pSpecials.push_back(new Special{ "Midnight Star", "Resources/Images/Items/Specials/Midnight Star.png", 40.f, 0.f });

	m_pArmors.push_back(new Armor{ "Wolfskin Armor", "Resources/Images/Items/Armors/Wolfskin Armor.png", 1.f });
	m_pArmors.push_back(new Armor{ "Griffon Hide Armor", "Resources/Images/Items/Armors/Griffon Hide Armor.png", 10.f });
	m_pArmors.push_back(new Armor{ "Hydra Skin Armor", "Resources/Images/Items/Armors/Hydra Skin Armor.png", 20.f });
	m_pArmors.push_back(new Armor{ "Wyrmhide Armor", "Resources/Images/Items/Armors/Wyrmhide Armor.png", 40.f });

	m_pRings.push_back(new Ring{ "Ring of Attack 1", "Resources/Images/Items/Rings/Ring of Attack 1.png", 5.f, 0.f});
	m_pRings.push_back(new Ring{ "Ring of Attack 2", "Resources/Images/Items/Rings/Ring of Attack 2.png", 10.f, 0.f});
	m_pRings.push_back(new Ring{ "Ring of Attack 3", "Resources/Images/Items/Rings/Ring of Attack 3.png", 20.f, 0.f});
	m_pRings.push_back(new Ring{ "Ring of Attack 4", "Resources/Images/Items/Rings/Ring of Attack 4.png", 40.f, 0.f});
	m_pRings.push_back(new Ring{ "Ring of Defense 1", "Resources/Images/Items/Rings/Ring of Defense 1.png", 0.f, 5.f});
	m_pRings.push_back(new Ring{ "Ring of Defense 2", "Resources/Images/Items/Rings/Ring of Defense 2.png", 0.f, 10.f});
	m_pRings.push_back(new Ring{ "Ring of Defense 3", "Resources/Images/Items/Rings/Ring of Defense 3.png", 0.f, 20.f});
	m_pRings.push_back(new Ring{ "Ring of Defense 4", "Resources/Images/Items/Rings/Ring of Defense 4.png", 0.f, 40.f});

	m_pOtherItems.push_back(new Item{ "Other", Item::ItemType::Item, "Resources/Images/Items/Other/Other.png"});
}

ItemAssets::~ItemAssets()
{
	for (size_t i{ 0 }; i < m_pWeapons.size(); ++i)
	{
		delete m_pWeapons[i];
	}
	for (size_t i{ 0 }; i < m_pSpecials.size(); ++i)
	{
		delete m_pSpecials[i];
	}
	for (size_t i{ 0 }; i < m_pArmors.size(); ++i)
	{
		delete m_pArmors[i];
	}
	for (size_t i{ 0 }; i < m_pRings.size(); ++i)
	{
		delete m_pRings[i];
	}
	for (size_t i{ 0 }; i < m_pOtherItems.size(); ++i)
	{
		delete m_pOtherItems[i];
	}
}


Weapon ItemAssets::GetWeapon(const std::string& name)
{
	for (size_t i{ 0 }; i < m_pWeapons.size(); ++i)
	{
		if (m_pWeapons[i]->GetItemName() == name)
		{
			return *m_pWeapons[i];
		}
	}
	return m_NullWeapon;
}


Special ItemAssets::GetSpecial(const std::string& name)
{
	for (size_t i{ 0 }; i < m_pSpecials.size(); ++i)
	{
		if (m_pSpecials[i]->GetItemName() == name)
		{
			return *m_pSpecials[i];
		}
	}
	return m_NullSpecial;
}

Armor ItemAssets::GetArmor(const std::string& name)
{
	for (size_t i{ 0 }; i < m_pArmors.size(); ++i)
	{
		if (m_pArmors[i]->GetItemName() == name)
		{
			return *m_pArmors[i];
		}
	}
	return m_NullArmor;
}

Ring ItemAssets::GetRing(const std::string& name)
{
	for (size_t i{ 0 }; i < m_pRings.size(); ++i)
	{
		if (m_pRings[i]->GetItemName() == name)
		{
			return *m_pRings[i];
		}
	}
	return m_NullRing;
}

Item ItemAssets::GetItem(const std::string& name)
{
	for (size_t i{ 0 }; i < m_pOtherItems.size(); ++i)
	{
		if (m_pOtherItems[i]->GetItemName() == name)
		{
			return *m_pOtherItems[i];
		}
	}
	return m_NullItem;
}


Weapon ItemAssets::GetRandomWeapon() const
{
	int index{ rand() % (int)m_pWeapons.size() };
	return *m_pWeapons[index];
}

Special ItemAssets::GetRandomSpecial() const
{
	int index{ rand() % (int)m_pSpecials.size() };
	return *m_pSpecials[index];
}

Armor ItemAssets::GetRandomArmor() const
{
	int index{ rand() % (int)m_pArmors.size() };
	return *m_pArmors[index];
}

Ring ItemAssets::GetRandomRing() const
{
	int index{ rand() % (int)m_pRings.size() };
	return *m_pRings[index];
}
Item ItemAssets::GetRandomItem() const
{
	int index{ rand() % (int)m_pOtherItems.size() };
	return *m_pOtherItems[index];
}




