#pragma once
#include <vector>


class NoiseGenerator final
{
public:
	NoiseGenerator();
	NoiseGenerator(const NoiseGenerator& other) = delete;
	NoiseGenerator& operator=(const NoiseGenerator& other) = delete;
	NoiseGenerator(NoiseGenerator&& other) = delete;
	NoiseGenerator& operator=(NoiseGenerator&& other) = delete;
	~NoiseGenerator();

	float** GenerateMapArray(const int width, const int height);
	float** GenerateIslandMapArray(const int width, const int height);

private:
	void GeneratePerlinNoise(int width, int height, int nrOfOctaves, float** seed, float** output);
};

