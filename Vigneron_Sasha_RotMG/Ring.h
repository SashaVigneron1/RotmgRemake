#pragma once

#include "Item.h"

class Ring final : public Item
{
public: 
	Ring(const std::string& itemName, const std::string& texturePath, float attackBonus, float defenseBonus);
	Ring(const Ring& other);
	Ring& operator=(const Ring& other) = delete;
	Ring(Ring&& other) noexcept;
	Ring& operator=(Ring&& other) = delete;

	float GetAttackBonus() const;
	float GetDefenseBonus() const;

private: 
	float m_AttackBonus;
	float m_DefenseBonus;
};

