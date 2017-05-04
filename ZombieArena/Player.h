#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	Vector2f position;
	Vector2f resolution;
	Sprite sprite;
	Texture texture;
	IntRect arena;
	Time lastHit;

	int tileSize;
	int health;
	int maxHealth;
	float speed;

	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;

public:

	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	void resetPlayerState();

	bool hit(Time timeHit);

	Time getLastHitTime();

	FloatRect getPosition();

	Vector2f getCenter();

	float getRotation();

	Sprite getSprite();

	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	void stopLeft();

	void stopRight();

	void stopUp();

	void stopDown();

	void update(float elapsedTime, Vector2i mousePosition);

	void upgradeSpeed();

	void upgradeHealth();

	void increaseHealthLevel(int amount);

	int getHealth();
};