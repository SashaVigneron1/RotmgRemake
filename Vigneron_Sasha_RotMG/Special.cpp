#include "pch.h"
#include "Special.h"


Special::Special(const std::string& itemName, const std::string& texturePath, float attackBonus, float defenseBonus)
	: Item{ itemName, Item::ItemType::Special,  texturePath }
	, m_AttackBonus{ attackBonus }
	, m_DefenseBonus{ defenseBonus }
{
}

Special::Special(const Special& other)
	: Item{ other }
	, m_AttackBonus{ other.m_AttackBonus }
	, m_DefenseBonus{ other.m_DefenseBonus}
{
}

Special::Special(Special&& other)
	: Item{ other }
	, m_AttackBonus{ other.m_AttackBonus }
	, m_DefenseBonus{ other.m_DefenseBonus }
{
}


float Special::GetAttackBonus() const
{
	return m_AttackBonus;
}

float Special::GetDefenseBonus() const
{
	return m_DefenseBonus;
}
