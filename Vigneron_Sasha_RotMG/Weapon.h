#pragma once

#include "Item.h"

class Weapon final : public Item
{
public:
	Weapon(const std::string& itemName, const std::string& texturePath, float damage);
	Weapon(const Weapon& other);
	Weapon& operator=(const Weapon& other) = delete;
	Weapon(Weapon&& other);
	Weapon& operator=(Weapon&& other) = delete;

	float GetDamage() const;
private: 
	float m_Damage;
};

