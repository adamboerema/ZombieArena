#include "stdafx.h"
#include "ZombieArena.h"

int createBackground(VertexArray& arenaVertices, IntRect arena) {
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int currentVertex = 0;
	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	arenaVertices.setPrimitiveType(Quads);
	arenaVertices.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	for (int w = 0; w < worldWidth; w++) {
		for (int h = 0; h < worldHeight; h++) {
			//Position each vertex in quad
			arenaVertices[currentVertex + 0].position 
				= Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			
			arenaVertices[currentVertex + 1].position
				= Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);

			arenaVertices[currentVertex + 2].position
				= Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			arenaVertices[currentVertex + 3].position
				= Vector2f(w * TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			//Define the position in the Texture for current quad
			if (h == 0 || h == worldHeight - 1
				|| w == 0 || w == worldWidth - 1) {

				arenaVertices[currentVertex + 0].texCoords
					= Vector2f(0, TILE_TYPES * TILE_SIZE);

				arenaVertices[currentVertex + 1].texCoords
					= Vector2f(TILE_SIZE, TILE_TYPES * TILE_SIZE);

				arenaVertices[currentVertex + 2].texCoords
					= Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);

				arenaVertices[currentVertex + 3].texCoords
					= Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else {
				srand((int)time_t(0) + h * w - h);
				int randomIndex = (rand() % TILE_TYPES);
				int verticalOffset = randomIndex * TILE_SIZE;

				arenaVertices[currentVertex + 0].texCoords
					= Vector2f(0, verticalOffset);

				arenaVertices[currentVertex + 1].texCoords
					= Vector2f(TILE_SIZE, verticalOffset);

				arenaVertices[currentVertex + 2].texCoords
					= Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);

				arenaVertices[currentVertex + 3].texCoords
					= Vector2f(0, TILE_SIZE + verticalOffset);
			}

			//Position ready for the next for vertices
			currentVertex += VERTS_IN_QUAD;
		}
	}

	return TILE_SIZE;
}