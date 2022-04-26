#include "pch.h"
#include "Weapon.h"

Weapon::Weapon(const std::string& itemName, const std::string& texturePath, float damage)
	: Item{ itemName,  Item::ItemType::Weapon, texturePath }
	, m_Damage{ damage }
{

}

Weapon::Weapon(const Weapon& other)
	: Item{ other }
	, m_Damage{ other.m_Damage}
{
}

Weapon::Weapon(Weapon&& other)
	: Item{ other }
	, m_Damage{ other.m_Damage }
{
}

float Weapon::GetDamage() const
{
	return m_Damage;
}
