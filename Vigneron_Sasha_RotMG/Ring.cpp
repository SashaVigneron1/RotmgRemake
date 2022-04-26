#include "pch.h"
#include "Ring.h"

Ring::Ring(const std::string& itemName, const std::string& texturePath, float attackBonus, float defenseBonus)
	: Item{itemName, Item::ItemType::Ring,  texturePath}
	, m_AttackBonus{attackBonus}
	, m_DefenseBonus{defenseBonus}
{
}

Ring::Ring(const Ring& other)
	: Item{ other }
	, m_AttackBonus{ other.m_AttackBonus }
	, m_DefenseBonus{ other.m_DefenseBonus }
{
}

Ring::Ring(Ring&& other) noexcept
	: Item{ other }
	, m_AttackBonus{ other.m_AttackBonus }
	, m_DefenseBonus{ other.m_DefenseBonus }
{
}

float Ring::GetAttackBonus() const
{
	return m_AttackBonus;
}

float Ring::GetDefenseBonus() const
{
	return m_DefenseBonus;
}
