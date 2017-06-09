#pragma once
#include "Zombie.h"
#include "SFML/Graphics/VertexArray.hpp"


using namespace sf;

int createBackground(VertexArray &arenaVertices, IntRect arena);
Zombie* createHorde(int numZombies, IntRect arena);