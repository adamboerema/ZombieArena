#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Pickup {
private:
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	Sprite sprite;
	IntRect arena;
	int value;
	int type;
	bool spawned;
	float secondsSinceSpawn;
	float secondsSinceDespawn;
	float secondsToLive;
	float secondsToWait;

public:
	Pickup::Pickup(int type);

	void setArena(IntRect arena);
	void spawn();

	FloatRect getPosition();
	Sprite getSprite();
	void update(float elapsedTime);
	bool isSpawned();
	int gotIt();
	void upgrade();
};