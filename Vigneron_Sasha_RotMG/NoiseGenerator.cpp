#include "pch.h"
#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator()
{

}

NoiseGenerator::~NoiseGenerator()
{

}

float** NoiseGenerator::GenerateMapArray(const int width, const int height)
{
	float** seedArray{};
	float** mapArray{};
	mapArray = new float* [width];
	seedArray = new float* [width];

	//To avoid Nullptrs
	for (int x{}; x < width; ++x)
	{
		mapArray[x] = new float[height];
		seedArray[x] = new float[height];
		for (int y{}; y < height; ++y)
		{
			seedArray[x][y] = rand() / float(RAND_MAX);
		}
	}

	GeneratePerlinNoise(width, height, 24, seedArray, mapArray);

	//Make room for new seedarray
	for (int x{}; x < width; ++x)
	{
		delete seedArray[x];
	}
	delete[] seedArray;

	return mapArray;
}

float** NoiseGenerator::GenerateIslandMapArray(const int width, const int height)
{
	float** mapArray{};
	mapArray = new float* [width];

	float** randomizationMapArray{};
	randomizationMapArray = GenerateMapArray(width, height);

	float centerX{ width / 2.0f };
	float centerY{ width / 2.0f };
	float maxPossibleDistance{ sqrtf(centerX * centerX + centerY * centerY) };

	for (int x{}; x < width; ++x)
	{
		mapArray[x] = new float[height];

		float distanceX{ (centerX - x) * (centerX - x) };

		for (int y{}; y < height; ++y)
		{
			
			float distanceY{ (centerY - y) * (centerY - y) };

			float distanceToCenter{ sqrtf(distanceX + distanceY) };
			distanceToCenter /= maxPossibleDistance;	// Must be  0 < x < 1

			mapArray[x][y] = distanceToCenter;

			//Distance is 3/4 of the value, to add randomization we add a random perlin noise that accounts for 1/4th of the value
			mapArray[x][y] = (2*mapArray[x][y] + randomizationMapArray[x][y]) / 3.0f;
			//std::cout << x << ", " << y << ": " << mapArray[x][y] << std::endl;

		}

	}

	for (int x{}; x < width; ++x)
	{
		delete randomizationMapArray[x];
	}
	delete[] randomizationMapArray;

	return mapArray;
}



void NoiseGenerator::GeneratePerlinNoise(int width, int height, int nrOfOctaves, float** seed, float** output)
{
	// Reference https://www.youtube.com/watch?v=6-0UaeJBumA&t=796s&ab_channel=javidx9

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{

			float noise = 0.0f;
			float scaleAcc = 0.0f;
			float scale = 1.0f;

			for (int o = 1; o <= nrOfOctaves; o++)
			{
				int pitch = width / (2*o);
				int sampleX1 = (x / pitch) * pitch;
				int sampleY1 = (y / pitch) * pitch;

				int sampleX2 = (sampleX1 + pitch) % width;
				int sampleY2 = (sampleY1 + pitch) % width;

				float blendX = (float)(x - sampleX1) / (float)pitch;
				float blendY = (float)(y - sampleY1) / (float)pitch;

				float sampleT = (1.0f - blendX) * seed[sampleX1][sampleY1] + blendX * seed[sampleX2][sampleY1];
				float sampleB = (1.0f - blendX) * seed[sampleX1][sampleY2] + blendX * seed[sampleX2][sampleY2];

				scaleAcc += scale;
				noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
				
			}
			
			output[x][y] = noise / scaleAcc;
			//std::cout << x << ", " << y << ": " << fOutput[x][y] << std::endl;
		}
	

}

