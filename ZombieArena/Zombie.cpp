#include "stdafx.h"
#include "zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void Zombie::spawn(float startX, float startY, int type, int seed) {
	switch (type) {
	case 0:
		//Bloater
		sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		speed = 40;
		health = 5;
		break;
	case 1:
		sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		speed = 70;
		health = 1;
		break;
	case 2:
		sprite = Sprite(TextureHolder::GetTexture("graphics/crawler"));
		speed = 20;
		health = 3;
		break;
	}

	//Speed modifier is unique per zombie
	srand((int)time(0) * seed);

	float modifier = (rand() % MAX_VARIANCE) + OFFSET;
	modifier /= 100;
	speed *= modifier;
	position.x = startX;
	position.y = startY;
	sprite.setOrigin(25, 25);
	sprite.setPosition(position);
}

bool Zombie::hit() {
	health--;
	if (health < 0) {
		alive = false;
		sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}
	return false;
}

bool Zombie::isAlive() {
	return alive;
}

FloatRect Zombie::gePosition() {
	return sprite.getGlobalBounds();
}

Sprite Zombie::getSprite() {
	return sprite;
}

void Zombie::update(float elapsedTime, Vector2f playerLocation) {
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	if (playerX > position.x) {
		position.x += speed * elapsedTime;
	}
	else {
		position.x -= speed * elapsedTime;
	}

	if (playerY > position.y) {
		position.y += speed * elapsedTime;
	}
	else {
		position.y -= speed * elapsedTime;
	}

	sprite.setPosition(position);
	float angle = (atan2(playerY - position.y, playerX - position.x) * 180) / 3.141;
	sprite.setRotation(angle);

}