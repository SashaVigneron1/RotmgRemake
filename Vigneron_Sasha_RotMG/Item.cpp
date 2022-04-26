#include "pch.h"
#include "Item.h"
#include "Texture.h"

Item::Item(const std::string& itemName, ItemType itemType, const std::string& texturePath )
	: m_ItemName{ itemName }
	, m_TexturePath{ texturePath }
	, m_pTexture{ new Texture{texturePath} }
	, m_IsBeingHeld{ false }
	, m_Position{ Point2f{0,0} }
	, m_ItemType{ itemType }
{
}

Item::Item(const Item& other)
	: m_ItemName{ other.m_ItemName }
	, m_TexturePath{ other.m_TexturePath }
	, m_pTexture{ new Texture{ other.m_TexturePath} }
	, m_IsBeingHeld{other.m_IsBeingHeld }
	, m_Position{ other.m_Position }
	, m_ItemType{ other.m_ItemType }
{

}

Item::~Item()
{
	delete m_pTexture;
}

void Item::Draw(const Point2f& position) const
{
	if (m_IsBeingHeld) 
	{
		m_pTexture->Draw(Rectf{ m_Position.x, m_Position.y, m_pTexture->GetWidth(), m_pTexture->GetHeight() });
	}
	else 
	{
		m_pTexture->Draw(Rectf{ position.x, position.y, m_pTexture->GetWidth(), m_pTexture->GetHeight() });
	}
}

std::string Item::GetItemName() const
{
	return m_ItemName;
}

bool Item::IsBeingHeld() const
{
	return m_IsBeingHeld;
}

void Item::SetBeingHeld(bool value)
{
	m_IsBeingHeld = value;
}

void Item::SetPosition(const Point2f& position, bool startingFromMiddle)
{
	if (startingFromMiddle) 
	{
		m_Position.x = position.x - m_pTexture->GetWidth() / 2.f;
		m_Position.y = position.y - m_pTexture->GetHeight() / 2.f;
	}
	else 
	{
		m_Position = position;
	}
	
}

bool Item::IsOnItem(const Point2f& position) const
{
	if (position.x >= m_Position.x
		&& position.x <= m_Position.x + m_pTexture->GetWidth()
		&& position.y >= m_Position.y
		&& position.y <= m_Position.y + m_pTexture->GetHeight()) 
	{
		return true;
	}
	return false;
}

Item::ItemType Item::GetItemType() const
{
	return m_ItemType;
}
