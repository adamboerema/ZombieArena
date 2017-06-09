#include "stdafx.h"
#include "Bullet.h"

using namespace sf;

Bullet::Bullet() {
	bulletShape.setSize(Vector2f(2, 2));
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY) {
	inFlight = true;
	position.x = startX;
	position.y = startY;

	float gradient = (startX - targetX) / (startY - targetY);

	if (gradient < 0) {
		gradient *= -1;
	}

	float ratioXY = bulletSpeed / (1 + gradient);
	bulletDistanceY = ratioXY;
	bulletDistanceX = ratioXY * gradient;

	if (targetX < startX) {
		bulletDistanceX *= -1;
	}

	if (targetY < startY) {
		bulletDistanceY *= -1;
	}

	float range = 1000;
	minX = startX - range;
	maxX = startX + range;
	minY = startY - range;
	maxY = startY + range;
	bulletShape.setPosition(position);
}

void Bullet::stop() {
	inFlight = false;
}

bool Bullet::isInFlight() {
	return inFlight;
}

FloatRect Bullet::getPosition() {
	return bulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape() {
	return bulletShape;
}

void Bullet::update(float elapsedTime) {
	position.x += bulletDistanceX * elapsedTime;
	position.y += bulletDistanceY * elapsedTime;

	bulletShape.setPosition(position);
	if (position.x < minX 
		|| position.x > maxX 
		|| position.y < minY 
		|| position.y > maxX) {

		inFlight = false;
	}
}