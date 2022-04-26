#pragma once
class Texture;

class Obstacle final
{

public:
	Obstacle(Texture* texture, const Point2f& position, float scale);
	Obstacle(const Obstacle& other) = delete;
	Obstacle& operator=(const Obstacle& other) = delete;
	Obstacle(Obstacle&& other) = delete;
	Obstacle& operator=(Obstacle&& other) = delete;

	Rectf GetRect() const;
	void Draw() const;
	bool CheckHit(const Rectf& rect) const;

private:
	Texture* m_pTexture;
	Rectf m_Rect;
	float m_Scale;
};

