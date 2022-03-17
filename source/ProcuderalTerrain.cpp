#include "ProcuderalTerrain.h"

ProcuderalTerrain::ProcuderalTerrain(TerrainInfo& info)
{
	generateTerrain(info);
	setupBuffers();
}

void ProcuderalTerrain::draw(Shader& shader)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
}

unsigned int ProcuderalTerrain::getVertexCount()
{
	return vertexSize;
}

unsigned int ProcuderalTerrain::getNormalCount()
{
	return normalSize;
}

unsigned int ProcuderalTerrain::getIndexCount()
{
	return indexSize;
}

void ProcuderalTerrain::generateTerrain(TerrainInfo& info)
{
	siv::PerlinNoise::seed_type seed = info.seed;
	siv::PerlinNoise perlin{ seed };

	unsigned int counter = 0u;

	for (size_t z = 0; z < info.mapSize; z++)
	{
		for (size_t x = 0; x < info.mapSize; x++)
		{
			double result = perlin.octave2D_01(x * info.frequency, z * info.frequency, 4);
			glm::vec3 position = glm::vec3(x * info.triXTriScale, result * info.triYTriScale, z * info.triZTriScale);
			vertices.push_back(position);

			glm::vec3 index(0.f);

			if (z < info.mapSize - 1)
			{
				if (x < info.mapSize - 1)
				{
					indices.push_back(counter);
					indices.push_back(counter + info.mapSize);
					indices.push_back(counter + 1);

				}

				if (x + 1 < info.mapSize)
				{
					indices.push_back(counter + 1u);
					indices.push_back(counter + info.mapSize);
					indices.push_back(counter + info.mapSize + 1u);
				}
			}

			counter++;
		}
	}

	normals.resize(vertices.size());

	for (size_t i = 0; i < indices.size(); i+=3)
	{
		glm::vec3 normal(0.f);

		glm::vec3 position0 = vertices.at(indices.at(i));
		glm::vec3 position1 = vertices.at(indices.at(i + 1));
		glm::vec3 position2 = vertices.at(indices.at(i + 2));

		glm::vec3 rayOne = position1 - position0;
		glm::vec3 rayTwo = position2 - position0;
		normal = glm::normalize(glm::cross(rayTwo, rayOne));

		normals.at(indices.at(i)) += normal;
		normals.at(indices.at(i + 1)) += normal;
		normals.at(indices.at(i + 2)) += normal;

		glm::normalize(normals.at(indices.at(i)));
		glm::normalize(normals.at(indices.at(i + 1)));
		glm::normalize(normals.at(indices.at(i + 2)));
	}

	normalSize = normals.size();
	indexSize = indices.size();
	vertexSize = vertices.size();
}

void ProcuderalTerrain::setupBuffers()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	//Cleanup Main Memory (RAM).
	vertices.clear();
	indices.clear();
}
