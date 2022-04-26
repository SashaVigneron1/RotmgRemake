#include "pch.h"
#include "ItemBagManager.h"
#include "ItemBag.h"
#include "utils.h"

ItemBagManager::ItemBagManager()
	: m_ItemAssets{}
{
}

ItemBagManager::~ItemBagManager()
{
	for (size_t i{0}; i < m_pItemBags.size(); ++i)
	{
		delete m_pItemBags[i];
	}
}
void ItemBagManager::AddRandomItemBag(const Point2f& position)
{
	int maxNrOfItems{ 6 };
	int nrOfItems{ rand() % (maxNrOfItems - 1) + 1 };
	std::vector<Item*> items;
	for (int i{}; i < nrOfItems; ++i)
	{
		int itemType{ rand() % ((int)Item::ItemType::Item + 1) };
		switch (Item::ItemType(itemType))
		{
		case Item::ItemType::Weapon:
			items.push_back(new Weapon{ m_ItemAssets.GetRandomWeapon() });
			break;
		case Item::ItemType::Special:
			items.push_back(new Special{ m_ItemAssets.GetRandomSpecial() });
			break;
		case Item::ItemType::Armor:
			items.push_back(new Armor{ m_ItemAssets.GetRandomArmor() });
			break;
		case Item::ItemType::Ring:
			items.push_back(new Ring{ m_ItemAssets.GetRandomRing() });
			break;
		case Item::ItemType::Item:
			items.push_back(new Item{ m_ItemAssets.GetRandomItem() });
			break;
		default:
			break;
		}
		
	}
	m_pItemBags.push_back(new ItemBag{items, position});
}
void ItemBagManager::AddItemBag(ItemBag* pItemBag) 
{
	m_pItemBags.push_back(pItemBag);
}
void ItemBagManager::DrawItemBags() const
{
	for(size_t i{ 0 }; i < m_pItemBags.size(); ++i)
	{
		m_pItemBags[i]->Draw();
	}
}
void ItemBagManager::DrawUI() const
{
	for (size_t i{ 0 }; i < m_pItemBags.size(); ++i)
	{
		m_pItemBags[i]->DrawUI();
	}
}
bool ItemBagManager::IsOnTopOfBag(const Rectf& rect)
{
	for (ItemBag* pItemBag : m_pItemBags)
	{
		if (utils::IsOverlapping(rect, pItemBag->GetRect())) 
		{
			pItemBag->ShowUI(true);
			return true;
		}
		else 
		{
			pItemBag->ShowUI(false);
		}
	}
	return false;
}


void ItemBagManager::CheckIfBagsAreEmpty()
{
	for (ItemBag* pItemBag : m_pItemBags)
	{
		if (pItemBag->GetSize() < 1 && !pItemBag->IsPlayerOnTop()) 
		{
			DestroyBag(pItemBag);
		}
	}
}
void ItemBagManager::DestroyBag(ItemBag* pItemBag)
{
	for (size_t i{ 0 }; i < m_pItemBags.size(); ++i)
	{
		if (pItemBag == m_pItemBags[i])
		{
			delete m_pItemBags[i];
			m_pItemBags[i] = m_pItemBags.back();
			m_pItemBags.pop_back();
			return;
		}
	}
}

void ItemBagManager::UpdateItemPositions(const Point2f& position, bool isClick)
{
	for (ItemBag* pItemBag : m_pItemBags) 
	{
		if (pItemBag->IsPlayerOnTop()) 
		{
			pItemBag->UpdateItemPositions(position, isClick);
		}
		else 
		{
			pItemBag->ReleaseItem();
		}
	}
	CheckIfBagsAreEmpty();
}

Item* ItemBagManager::GetHeldItem()
{
	for (ItemBag* pItemBag : m_pItemBags)
	{
		if (pItemBag->IsPlayerOnTop())
		{
			Item* pItem = pItemBag->GetHeldItem();
			return pItem;
		}
	}
	return nullptr;
}

void ItemBagManager::AddTimers(float elapsedSec)
{
	const float maxTime{ 20.f };
	for (ItemBag* pItemBag : m_pItemBags)
	{
		pItemBag->AddTime(elapsedSec);
		if (pItemBag->GetTimer() >= maxTime) 
		{
			DestroyBag(pItemBag);
		}
	}
}
