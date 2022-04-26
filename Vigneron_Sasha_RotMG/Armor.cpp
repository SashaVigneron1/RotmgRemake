#include "pch.h"
#include "Armor.h"

Armor::Armor(const std::string& itemName, const std::string& texturePath, float defense)
	: Item{itemName, Item::ItemType::Armor, texturePath}
	, m_Defense{ defense }
{

}

Armor::Armor(const Armor& other)
	: Item{ other }
	, m_Defense{ other.m_Defense}
{
}

Armor::Armor(Armor&& other) noexcept
	: Item{ other }
	, m_Defense{ other.m_Defense }
{
}

float Armor::GetDefense() const
{
	return m_Defense;
}
