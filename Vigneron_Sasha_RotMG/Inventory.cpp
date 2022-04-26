#include "pch.h"
#include "Inventory.h"
#include "utils.h"
#include "ItemBagManager.h"
#include "ItemBag.h"	

Inventory::Inventory(ItemBagManager* pItemBagManager)
	: m_ItemAssets{}
	, m_Columns{ 4 }
	, m_Rows{2}
	, m_GearZone{ Rectf{ 1052.f, 300.f, 220.f, 60.f} }
	, m_InventoryZone{ Rectf{ 1052.f, 150.f, 220.f, 120.f} }
	, m_pActiveWeapon{}
	, m_pActiveSpecial{}
	, m_pActiveArmor{}
	, m_pActiveRing{}
	, m_IsHoldingItem{ false }
	, m_HeldItem{nullptr}
	, m_pItemBagManager{ pItemBagManager }
{
	//m_pItems.push_back(new Weapon{ m_ItemAssets.GetWeapon("Ichimonji") });
	m_pActiveWeapon = new Weapon{ m_ItemAssets.GetWeapon("Ichimonji") };
	m_pActiveSpecial = new Special{ m_ItemAssets.GetSpecial("Basic Star") };
	//m_pActiveArmor = new Armor{ m_ItemAssets.GetArmor("Wolfskin Armor") };
	//m_pActiveRing = new Ring{ m_ItemAssets.GetRing("Ring") };
}

Inventory::~Inventory()
{
	for (size_t i{ 0 }; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i]) delete m_pItems[i];
	}
	if (m_HeldItem) delete m_HeldItem;
	if (m_pActiveWeapon) delete m_pActiveWeapon;
	if (m_pActiveSpecial) delete m_pActiveSpecial;
	if (m_pActiveArmor) delete m_pActiveArmor;
	if (m_pActiveRing) delete m_pActiveRing;
	
}

void Inventory::Draw() const
{
	// draw items
	float offset{55.f};
	Point2f startPos{};
	startPos.x = 1055.f;
	startPos.y = 215.f;
	for (size_t i{ 0 }; i < m_pItems.size(); ++i)
	{
		int row{ int(i) / m_Columns };
		int column{  int(i) % m_Columns};
		Point2f position{ Point2f{ startPos.x + (column * offset), startPos.y - (row * offset) } };
		m_pItems[i]->Draw(position);
		m_pItems[i]->SetPosition(position);
	}

	// draw active weapon;
	startPos.y = 307.f;
	if (m_pActiveWeapon)
	{
		m_pActiveWeapon->Draw(startPos);
		m_pActiveWeapon->SetPosition(startPos);

	}
	// draw active special
	startPos.x += offset;
	if (m_pActiveSpecial)
	{
		m_pActiveSpecial->Draw(startPos);
		m_pActiveSpecial->SetPosition(startPos);
	}
	// draw active armor
	startPos.x += offset;
	if (m_pActiveArmor)
	{
		m_pActiveArmor->Draw(startPos);
		m_pActiveArmor->SetPosition(startPos);
	}
	// draw active ring
	startPos.x += offset;
	if (m_pActiveRing)
	{
		m_pActiveRing->Draw(startPos);
		m_pActiveRing->SetPosition(startPos);
	}
	// draw held item
	if (m_HeldItem) 
	{
		m_HeldItem->Draw();
	}
}

float Inventory::GetDefenseBonus() const
{
	float result{ };
	if (m_pActiveSpecial) { result += m_pActiveSpecial->GetDefenseBonus(); }
	if (m_pActiveArmor) { result += m_pActiveArmor->GetDefense(); }
	if (m_pActiveRing) { result += m_pActiveRing->GetDefenseBonus(); }
	return result;
}
float Inventory::GetAttackBonus() const
{
	float result{ };
	if (m_pActiveSpecial) { result += m_pActiveSpecial->GetAttackBonus(); }
	if (m_pActiveWeapon) { result += m_pActiveWeapon->GetDamage(); }
	if (m_pActiveRing) { result += m_pActiveRing->GetAttackBonus(); }
	return result;
}

void Inventory::UpdateItemPositions(const Point2f& position, bool isClick)
{
	if (!m_IsHoldingItem)
	{

		// For Inventory
		for (Item* pItem : m_pItems)
		{
			if (isClick)
			{
				if (pItem->IsOnItem(position))
				{
					m_IsHoldingItem = true;
					pItem->SetBeingHeld(true);
					
					Weapon* pWeapon{ nullptr };
					Special* pSpecial{ nullptr };
					Armor* pArmor{ nullptr };
					Ring* pRing{ nullptr };

					switch (pItem->GetItemType())
					{
					case Item::ItemType::Weapon:
						pWeapon = static_cast<Weapon*>(pItem);
						if (pWeapon)
						{
							m_HeldItem = new Weapon{ *pWeapon };
							RemoveItem(pItem);
						}
						else
						{
							std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
						}
						break;
					case Item::ItemType::Special:
						pSpecial = static_cast<Special*>(pItem);
						if (pSpecial)
						{
							m_HeldItem = new Special{ *pSpecial };
							RemoveItem(pItem);
						}
						else
						{
							std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
						}
						break;
					case Item::ItemType::Armor:
						pArmor = static_cast<Armor*>(pItem);
						if (pArmor)
						{
							m_HeldItem = new Armor{ *pArmor };
							RemoveItem(pItem);
						}
						else
						{
							std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
						}
						break;
					case Item::ItemType::Ring:
						pRing = static_cast<Ring*>(pItem);
						if (pRing)
						{
							m_HeldItem = new Ring{ *pRing };
							RemoveItem(pItem);
						}
						else
						{
							std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
						}
						break;
					case Item::ItemType::Item:
						m_HeldItem = new Item{ *pItem };
						RemoveItem(pItem);
						break;
					}
					break;
				}
			}
		}

		// For Gear
		if (isClick)
		{
			if (m_pActiveWeapon && m_pActiveWeapon->IsOnItem(position))
			{
				m_IsHoldingItem = true;
				m_pActiveWeapon->SetBeingHeld(true);
				m_HeldItem = new Weapon{ *m_pActiveWeapon };
				delete m_pActiveWeapon;
				m_pActiveWeapon = nullptr;
			}
			else if (m_pActiveSpecial && m_pActiveSpecial->IsOnItem(position))
			{
				m_IsHoldingItem = true;
				m_pActiveSpecial->SetBeingHeld(true);
				m_HeldItem = new Special{ *m_pActiveSpecial };
				delete m_pActiveSpecial;
				m_pActiveSpecial = nullptr;
			}
			else if (m_pActiveArmor && m_pActiveArmor->IsOnItem(position))
			{
				m_IsHoldingItem = true;
				m_pActiveArmor->SetBeingHeld(true);
				m_HeldItem = new Armor{ *m_pActiveArmor };
				delete m_pActiveArmor;
				m_pActiveArmor = nullptr;
			}
			else if (m_pActiveRing && m_pActiveRing->IsOnItem(position))
			{
				m_IsHoldingItem = true;
				m_pActiveRing->SetBeingHeld(true);
				m_HeldItem = new Ring{ *m_pActiveRing };
				delete m_pActiveRing;
				m_pActiveRing = nullptr;
			}
		}
	}
	else if (m_HeldItem) 
	{
		m_HeldItem->SetPosition(position, true);
	}
}

void Inventory::ReleaseItem(const Point2f& mousePosition, const Point2f& playerPosition, Item* pItem)
{
	if (pItem) 
	{
		pItem->SetBeingHeld(false);
	}
	else 
	{
		if (!m_IsHoldingItem) { return; }

		m_IsHoldingItem = false;
		m_HeldItem->SetBeingHeld(false);
		pItem = m_HeldItem;
		m_HeldItem = nullptr;
	}

	if (pItem) 
	{
		if (mousePosition.x >= m_InventoryZone.left
			&& mousePosition.x <= m_InventoryZone.left + m_InventoryZone.width
			&& mousePosition.y >= m_InventoryZone.bottom
			&& mousePosition.y <= m_InventoryZone.bottom + m_InventoryZone.height)
		{
			if (!AddItem(pItem)) 
			{
				DropItem(pItem, playerPosition);
			}
		}
		else if (mousePosition.x >= m_GearZone.left
			&& mousePosition.x <= m_GearZone.left + m_GearZone.width
			&& mousePosition.y >= m_GearZone.bottom
			&& mousePosition.y <= m_GearZone.bottom + m_GearZone.height)
		{
			Weapon* pWeapon{ nullptr };
			Special* pSpecial{ nullptr };
			Armor* pArmor{ nullptr };
			Ring* pRing{ nullptr };

			switch (pItem->GetItemType())
			{
			case Item::ItemType::Weapon:
				pWeapon = new Weapon{ *static_cast<Weapon*>(pItem) };
				if (pWeapon)
				{
					if (m_pActiveWeapon)
					{
						AddItem(new Weapon{ *m_pActiveWeapon });
						delete m_pActiveWeapon;
					}
					m_pActiveWeapon = pWeapon;
					delete pItem;
					return;
				}
				else
				{
					DropItem(pItem, playerPosition);
				}
				break;
			case Item::ItemType::Special:
				pSpecial = new Special{ *static_cast<Special*>(pItem) };
				if (pSpecial)
				{
					if (m_pActiveSpecial)
					{
						AddItem(new Special{ *m_pActiveSpecial });
						delete m_pActiveSpecial;
					}
					m_pActiveSpecial = pSpecial;
					delete pItem;
					return;
				}
				else
				{
					DropItem(pItem, playerPosition);
				}
				break;
			case Item::ItemType::Armor:
				pArmor = new Armor{ *static_cast<Armor*>(pItem) };
				if (pArmor)
				{
					if (m_pActiveArmor)
					{
						AddItem(new Armor{ *m_pActiveArmor });
						delete m_pActiveArmor;
					}
					m_pActiveArmor = pArmor;
					delete pItem;
					return;
				}
				else
				{
					DropItem(pItem, playerPosition);
				}
				break;
			case Item::ItemType::Ring:
				pRing = new Ring{ *static_cast<Ring*>(pItem) };
				if (pRing)
				{
					if (m_pActiveRing)
					{
						AddItem(new Ring{ *m_pActiveRing });
						delete m_pActiveRing;
					}
					m_pActiveRing = pRing;
					delete pItem;
					return;
				}
				else
				{
					DropItem(pItem, playerPosition);
				}
				break;
			case Item::ItemType::Item:
				DropItem(pItem, playerPosition);
				break;
			}
		}
		else
		{
			DropItem(pItem, playerPosition);
		}
	}
	
}


void Inventory::DropItem(Item* pItem, const Point2f& position)
{
	std::vector<Item*> items;

	Weapon* pWeapon{ nullptr };
	Special* pSpecial{ nullptr };
	Armor* pArmor{ nullptr };
	Ring* pRing{ nullptr };

	switch (pItem->GetItemType())
	{
		case Item::ItemType::Weapon:
			pWeapon = dynamic_cast<Weapon*>(pItem);
			if (pWeapon) 
			{
				items.push_back(new Weapon{ *pWeapon });
			}
			else 
			{
				std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
			}
			break;
		case Item::ItemType::Special:
			pSpecial = static_cast<Special*>(pItem);
			if (pSpecial)
			{
				items.push_back(new Special{ *pSpecial });
			}
			else
			{
				std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
			}
			break;
		case Item::ItemType::Armor:
			pArmor = static_cast<Armor*>(pItem);
			if (pArmor)
			{
				items.push_back(new Armor{ *pArmor });
			}
			else
			{
				std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
			}
			break;
		case Item::ItemType::Ring:

			pRing = static_cast<Ring*>(pItem);
			if (pRing)
			{
				items.push_back(new Ring{ *pRing });
			}
			else
			{
				std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
			}
			break;
		case Item::ItemType::Item:
			if (pItem)
			{
				items.push_back(new Item{ *pItem });
			}
			else
			{
				std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
			}
			break;
	}

	delete pItem;

	m_pItemBagManager->AddItemBag(new ItemBag{ items, position });
}

bool Inventory::AddItem(Item* pItem)
{
	if (m_pItems.size() < (size_t(m_Columns) * m_Rows))
	{
		if (!ContainsItem(pItem))
		{
			m_pItems.push_back(pItem);
			return true;
		}
		return false;
	}
	else 
	{
		//std::cout << "Inventory full!\n";
		return false;
	}
}
bool Inventory::ContainsItem(Item* pItem)
{
	for (size_t i{ 0 }; i < m_pItems.size(); ++i)
	{
		if (pItem == m_pItems[i])
		{
			return true;
		}
	}
	return false;
}
void Inventory::RemoveItem(Item* pItem)
{
	for (size_t i{ 0 }; i < m_pItems.size(); ++i)
	{
		if (pItem == m_pItems[i])
		{
			delete m_pItems[i];
			m_pItems[i] = m_pItems.back();
			m_pItems.pop_back();
			return;
		}
	}
}

