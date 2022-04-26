#pragma once

class Texture;

class Tile final
{
public:

	Tile(const Texture* texture, const Point2f& pos, bool isWalkable, float scale);
	Tile(const Tile& other) = delete;
	Tile& operator=(const Tile& other) = delete;
	Tile(Tile&& other) = delete;
	Tile& operator=(Tile&& other) = delete;

	void Draw() const;
	Rectf GetRect() const;
	Point2f GetPosition() const;
	bool GetWalkable() const;
	bool IsOverlapping(const Rectf& rect) const;

private:
	const Texture* m_pTexture;
	const Point2f m_Position;
	const bool m_IsWalkable; // Use type: water instead
	const float m_Scale;
};

