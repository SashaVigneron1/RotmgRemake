#pragma once
#include <vector>
#include "Character.h"

class Obstacle;

class ObstacleManager final
{
public:
	ObstacleManager();
	ObstacleManager(const ObstacleManager& other) = delete;
	ObstacleManager& operator=(const ObstacleManager& other) = delete;
	ObstacleManager(ObstacleManager&& other) = delete;
	ObstacleManager& operator=(ObstacleManager&& other) = delete;
	~ObstacleManager();

	void Draw(const Point2f& playerPos) const;
	void Update(Point2f& playerPosition, float playerSize) const;

	void AddObstacle(Texture* texture, const Point2f& pos, float scale);
	bool CheckHit(const Rectf& rect) const;

private:
	std::vector<Obstacle*> m_pObstacles;
};

