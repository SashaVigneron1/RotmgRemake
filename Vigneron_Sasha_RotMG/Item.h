#pragma once

class Texture;

class Item
{
public:

	enum class ItemType 
	{
		Weapon = 0,
		Special = 1,
		Armor = 2,
		Ring = 3, 
		Item = 4
	};

	Item(const std::string& itemName, ItemType itemType, const std::string& texturePath);
	Item(const Item& other);
	Item& operator=(const Item& other) = delete;
	Item(Item&& other);
	Item& operator=(Item&& other) = delete;
	virtual ~Item();

	void Draw(const Point2f& position = Point2f{0,0}) const;
	std::string GetItemName() const;
	void SetBeingHeld(bool value);
	void SetPosition(const Point2f& position, bool startingFromMiddle = false);

	bool IsBeingHeld() const;
	bool IsOnItem(const Point2f& position) const;
	ItemType GetItemType() const;

private:
	std::string m_ItemName;
	std::string m_TexturePath;
	Texture* m_pTexture;
	bool m_IsBeingHeld;
	Point2f m_Position;
	ItemType m_ItemType;
};

