#pragma once

#include "Item.h"

class Armor final : public Item
{
public:
	Armor(const std::string& itemName, const std::string& texturePath, float defense);
	Armor(const Armor& other);
	Armor& operator=(const Armor& other) = delete;
	Armor(Armor&& other) noexcept;
	Armor& operator=(Armor&& other) = delete;
	float GetDefense() const;
private:
	float m_Defense;
};

