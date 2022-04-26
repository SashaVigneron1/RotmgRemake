#pragma once

#include "Item.h"

class Special final : public Item
{
public: 

	Special(const std::string& itemName, const std::string& texturePath
		, float attackBonus, float defenseBonus);
	Special(const Special& other);
	Special& operator=(const Special& other) = delete;
	Special(Special&& other);
	Special& operator=(Special&& other) = delete;

	float GetAttackBonus() const;
	float GetDefenseBonus() const;

private: 
	float m_AttackBonus;
	float m_DefenseBonus;
};

