#include "pch.h"
#include "ItemBag.h"

#include "Item.h"
#include "Weapon.h"
#include "Special.h"
#include "Armor.h"
#include "Ring.h"

ItemBag::ItemBag(const std::vector<Item*>& items, const Point2f& position)
	: m_Texture{ "Resources/Images/Items/ItemBag.png" }
	, m_UITexture{ "Resources/Images/UI/Menu/ItemBagUI.png" }
	, m_Columns{ 4 }
	, m_Rows{ 2 }
	, m_Position{ position }
	, m_ShowingUI{ false }
	, m_HeldItem{ nullptr }
	, m_IsHoldingItem{ false }
	, m_AccTime{}
{
	for (size_t i{ 0 }; i < items.size(); ++i)
	{
		m_pItems.push_back(items[i]);
	}
}

ItemBag::~ItemBag()
{
	for (size_t i{ 0 }; i < m_pItems.size(); ++i)
	{
		delete m_pItems[i];
	}
	if (m_HeldItem) delete m_HeldItem;
	
}

void ItemBag::Draw() const
{
	m_Texture.Draw(m_Position);
}

void ItemBag::DrawUI() const
{
	if (m_ShowingUI) 
	{
		m_UITexture.Draw(Point2f{ 1045.f, 0.f });

		int columns{4};
		float offset{ 55.f };
		Point2f startPos{};
		startPos.x = 1055.f;
		startPos.y = 80.f;
		for (size_t i{ 0 }; i < m_pItems.size(); ++i)
		{
			int row{ int(i) / columns };
			int column{ int(i) % columns };
			Point2f position{ startPos.x + (column * offset), startPos.y - (row * offset) };
			m_pItems[i]->Draw(position);
			m_pItems[i]->SetPosition(position);
		}
	}

	if (m_HeldItem) 
	{
		m_HeldItem->Draw();
	}
}

Rectf ItemBag::GetRect() const
{
	return Rectf{m_Position.x, m_Position.y, m_Texture.GetWidth(), m_Texture.GetHeight()};
}

int ItemBag::GetSize() const
{
	return (int)m_pItems.size();
}

float ItemBag::GetTimer() const
{
	return m_AccTime;
}

void ItemBag::AddTime(float elapsedSec)
{
	m_AccTime += elapsedSec;
}

bool ItemBag::IsPlayerOnTop() const
{
	return m_ShowingUI; // Kind of vague, but the showingUI gets set to true externally if the player is standing on top of the bag; Just for optimization purpose.
}

Item* ItemBag::GetHeldItem()
{
	Item* pItem{ m_HeldItem };
	m_HeldItem = nullptr;
	m_IsHoldingItem = false;
	return pItem;
}

void ItemBag::ReleaseItem()
{
	if (m_HeldItem) AddItem(m_HeldItem);
	m_HeldItem = nullptr;
	m_IsHoldingItem = false;
}

void ItemBag::ShowUI(bool value)
{
	m_ShowingUI = value;
}


void ItemBag::UpdateItemPositions(const Point2f& position, bool isClick)
{
	if (!m_IsHoldingItem)
	{
		m_HeldItem = nullptr;
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
						if (pItem) 
						{
							m_HeldItem = new Item{ *pItem };
							RemoveItem(pItem);
						}
						else 
						{
							std::cerr << "Something seems to have gone wrong, I'm terribly sorry!\n";
						}
						break;
					}
					break;
				}
			}
		}
	}
	else if (m_HeldItem)
	{
		m_HeldItem->SetPosition(position, true);
	}
}

void ItemBag::RemoveItem(Item* pItem)
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


bool ItemBag::AddItem(Item* pItem)
{
	if (m_pItems.size() < (size_t(m_Columns) * m_Rows))
	{
		if (!ContainsItem(pItem))
		{
			m_pItems.push_back(pItem);
			return true;
		}
	}
	return false;

}
bool ItemBag::ContainsItem(Item* pItem)
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