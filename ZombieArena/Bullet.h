#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Bullet {
private:
	Vector2f position;
	RectangleShape bulletShape;
	bool inFlight = false;
	float bulletSpeed = 1000;

	float bulletDistanceX;
	float bulletDistanceY;
	float maxX;
	float minX;
	float maxY;
	float minY;

public:
	Bullet();
	void stop();
	bool isInFlight();
	void shoot(float startX, float startY, float xTarget, float yTarget);
	FloatRect getPosition();
	RectangleShape getShape();
	void update(float elapsedTime);
};