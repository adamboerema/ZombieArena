#include "stdafx.h"
#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup(int type) {
	this->type = type;

	if (type == 1) {
		sprite = Sprite(TextureHolder::GetTexture("graphics/health_pickup.png"));
		value = HEALTH_START_VALUE;
	}
	else {
		sprite = Sprite(TextureHolder::GetTexture("graphics/ammo_pickup.png"));
		value = AMMO_START_VALUE;
	}

	sprite.setOrigin(25, 25);
	secondsToLive = START_SECONDS_TO_LIVE;
	secondsToWait = START_WAIT_TIME;
}

void Pickup::setArena(IntRect arena) {
	arena.left = arena.left + 50;
	arena.width = arena.width - 50;
	arena.top = arena.top + 50;
	arena.height = arena.height - 50;
	spawn();
}

void Pickup::spawn() {
	srand((int)time(0) / type);
	int x = (rand() % arena.width);
	srand((int)time(0) * type);
	int y = (rand() % arena.height);

	secondsSinceSpawn = 0;
	spawned = true;
	sprite.setPosition(x, y);
}

FloatRect Pickup::getPosition() {
	return sprite.getGlobalBounds();
}

Sprite Pickup::getSprite() {
	return sprite;
}

bool Pickup::isSpawned() {
	return spawned;
}

int Pickup::gotIt() {
	spawned = false;
	secondsSinceDespawn = 0;
	return value;
}

void Pickup::update(float elapsedTime) {
	if (spawned) {
		secondsSinceDespawn += elapsedTime;
	}
	else {
		secondsSinceDespawn += elapsedTime;
	}

	if (secondsSinceSpawn > secondsToLive && spawned) {
		spawned = false;
		secondsSinceDespawn = 0;
	}

	if (secondsSinceDespawn > secondsToWait && !spawned) {
		spawn();
	}
}

void Pickup::upgrade() {
	if (type == 1) {
		value += (HEALTH_START_VALUE * .5);
	}
	else {
		value += (AMMO_START_VALUE * .5);
	}

	secondsToLive += (START_SECONDS_TO_LIVE / 10);
	secondsToWait -= (START_WAIT_TIME / 10);
}