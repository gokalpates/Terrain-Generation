#pragma once

#include <PerlinNoise.hpp>

#include "Shader.h"

//Info for the terrain to be generated.
struct TerrainInfo
{
	unsigned int mapSize = 512u;
	unsigned int seed = 160322u;
	double frequency = 0.01;

	float triXTriScale = 1.f;
	float triYTriScale = 1.f;
	float triZTriScale = 1.f;
};

class ProcuderalTerrain
{
public:
	ProcuderalTerrain(TerrainInfo& info);

	void draw(Shader& shader);

	unsigned int getVertexCount();
	unsigned int getIndexCount();
private:
	void generateTerrain(TerrainInfo& info);
	void setupBuffers();

	unsigned int VAO, VBO, EBO;

	unsigned int vertexSize;
	unsigned int indexSize;

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
};

