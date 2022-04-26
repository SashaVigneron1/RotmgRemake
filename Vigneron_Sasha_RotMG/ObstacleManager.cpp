#include "pch.h"
#include "ObstacleManager.h"
#include "Obstacle.h"	
#include "Texture.h"

ObstacleManager::ObstacleManager()
{
}

ObstacleManager::~ObstacleManager()
{
	// Delete obstacles
	for (size_t i{ 0 }; i < m_pObstacles.size(); ++i)
	{
		delete m_pObstacles[i];
	}
	// Remove the pointers from the vector
	for (size_t i{ 0 }; i < m_pObstacles.size(); ++i)
	{
		m_pObstacles.pop_back();
	}
}

void ObstacleManager::Draw(const Point2f& playerPos) const
{
	bool isOptimizeMode{ true };
	if (isOptimizeMode)
	{
		float maxDistance{ 820.f };
		for (Obstacle* obstacle : m_pObstacles)
		{
			Point2f position{ Point2f{obstacle->GetRect().left, obstacle->GetRect().bottom } };
			float distance{ sqrtf((playerPos.x - position.x) * (playerPos.x - position.x)
							+ (playerPos.y - position.y) * (playerPos.y - position.y)) };
			if (distance <= maxDistance)
			{
				obstacle->Draw();
			}
		}
	}
	else
	{
		for (Obstacle* obstacle : m_pObstacles)
		{
			obstacle->Draw();
		}
	}
}

void ObstacleManager::Update(Point2f& playerPos, float playerSize) const
{
	float maxDistance{ 150.f };
	float halfOfActualPlayerWidth{ playerSize / 2.f };
	float halfOfActualPlayerHeight{ playerSize / 2.f };

	for (Obstacle* obstacle : m_pObstacles)
	{
		Rectf obstacleRect{ obstacle->GetRect() };
		float distance{ utils::GetDistance(playerPos, Point2f{obstacleRect.left, obstacleRect.bottom}) };

		if (distance <= maxDistance)
		{
			if (playerPos.x + halfOfActualPlayerWidth > obstacleRect.left
				&& playerPos.x + halfOfActualPlayerHeight < obstacleRect.left + obstacleRect.width
				&& playerPos.y < obstacleRect.bottom + obstacleRect.height
				&& playerPos.y > obstacleRect.bottom) 
			{
				playerPos.x = obstacleRect.left - halfOfActualPlayerHeight;
			}
			else if (playerPos.x - halfOfActualPlayerWidth > obstacleRect.left
				&& playerPos.x - halfOfActualPlayerWidth < obstacleRect.left + obstacleRect.width
				&& playerPos.y < obstacleRect.bottom + obstacleRect.height
				&& playerPos.y > obstacleRect.bottom)
			{
				playerPos.x = obstacleRect.left + obstacleRect.width + halfOfActualPlayerHeight;
			}
			else if (playerPos.y + halfOfActualPlayerHeight > obstacleRect.bottom
				&& playerPos.y + halfOfActualPlayerHeight < obstacleRect.bottom + obstacleRect.height
				&& playerPos.x < obstacleRect.left + obstacleRect.width
				&& playerPos.x > obstacleRect.left) 
			{
				playerPos.y = obstacleRect.bottom - halfOfActualPlayerHeight;
			}
			else if (playerPos.y - halfOfActualPlayerHeight > obstacleRect.bottom
				&& playerPos.y - halfOfActualPlayerHeight < obstacleRect.bottom + obstacleRect.height
				&& playerPos.x < obstacleRect.left + obstacleRect.width
				&& playerPos.x > obstacleRect.left) 
			{
				playerPos.y = obstacleRect.bottom + obstacleRect.height + halfOfActualPlayerHeight;
			}
		}
	}
}

void ObstacleManager::AddObstacle(Texture* texture, const Point2f& pos, float scale)
{
	m_pObstacles.push_back(new Obstacle{ texture, pos , scale});
}

bool ObstacleManager::CheckHit(const Rectf& rect) const
{
	bool succes{ false };

	//Optimize
	float maxDistanceToCheck{200.f};
	float minX{ rect.left - maxDistanceToCheck };
	float maxX{ rect.left + maxDistanceToCheck };
	float minY{ rect.bottom - maxDistanceToCheck };
	float maxY{ rect.bottom + maxDistanceToCheck };

	for (Obstacle* obstacle : m_pObstacles) 
	{
		float left{ obstacle->GetRect().left };
		float bottom{ obstacle->GetRect().bottom };
		if (left > minX && left < maxX
			&& bottom > minY && bottom < maxY) 
		{
			succes = obstacle->CheckHit(rect);
			if (succes) 
			{
				return true;
			}
		}
	}
	return false;
}