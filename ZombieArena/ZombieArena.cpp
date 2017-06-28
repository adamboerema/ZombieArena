#include "stdafx.h"
#include <sstream>
#include <fstream>
#include "Player.h"
#include "ZombieArena.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

using namespace std;

int main() {
	enum class State {
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};

	TextureHolder holder;
	State state = State::GAME_OVER;

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Zombie Arena", Style::Fullscreen);

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	Clock clock;
	Time gameTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	Player player;
	IntRect arena;

	//Load background vertex array
	VertexArray background;
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = NULL;

	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	Time lastPressed;

	//Replace mouse pointer with crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	Pickup healthPickup(1);
	Pickup ammoPickup(2);
	int score = 0;
	int highScore = 0;

	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	//Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);
	
	//Load the font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	//Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Pressed Enter \n to continue");

	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	//Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	//Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open()) {
		inputFile >> highScore;
		inputFile.close();
	}

	//High Score
	Text highScoreText;
	highScoreText.setFont(font);
	highScoreText.setCharacterSize(55);
	highScoreText.setFillColor(Color::White);
	highScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "High Score:" << highScore;
	highScoreText.setString(s.str());

	//Zombies remaining
	Text zombieRemainingText;
	zombieRemainingText.setFont(font);
	zombieRemainingText.setCharacterSize(55);
	zombieRemainingText.setFillColor(Color::White);
	zombieRemainingText.setPosition(1500, 980);
	zombieRemainingText.setString("Zombies: 100");

	//Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	//Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	int framesSinceLastHUDUpdate = 0;
	int fpsMeasurementFrameInterval = 1000;

	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");

	Sound hit;
	hit.setBuffer(hitBuffer);

	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);

	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {

				//Paused action
				if (event.key.code == Keyboard::Return
					&& state == State::PLAYING) {
					state = State::PAUSED;
				}

				//Resart while paused
				else if (event.key.code == Keyboard::Return
					&& state == State::PAUSED) {
					state = State::PLAYING;
					clock.restart();
				}

				//Start a new game if Game Over
				else if (event.key.code == Keyboard::Return
					&& state == State::GAME_OVER) {
					state = State::LEVELING_UP;

					//Restart
					wave = 0;
					score = 0;
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					player.resetPlayerState();
				}

				if (state == State::PLAYING) {
					//Reloading
					if (event.key.code == Keyboard::R) {
						if (bulletsSpare >= clipSize) {
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						else if (bulletsSpare > 0) {
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else {
							reloadFailed.play();
						}
					}
				}
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		//WASD
		if (state == State::PLAYING) {

			//Up
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.moveUp();
			}
			else {
				player.stopUp();
			}

			//Down
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.moveDown();
			}
			else {
				player.stopDown();
			}

			//Left
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.moveLeft();
			}
			else {
				player.stopLeft();
			}

			//Right
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.moveRight();
			}
			else {
				player.stopRight();
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate
					&& bulletsInClip > 0) {
					bullets[currentBullet].shoot(
						player.getCenter().x,
						player.getCenter().y,
						mouseWorldPosition.x,
						mouseWorldPosition.y
					);
					currentBullet++;
					if (currentBullet > 99) {
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					shoot.play();
					bulletsInClip--;
				}
			} //End fire a bullet
		} // End playing

		if (state == State::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) {
				fireRate++;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2) {
				clipSize += clipSize;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3) {
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4) {
				player.upgradeSpeed();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5) {
				healthPickup.upgrade();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6) {
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			if (state == State::PLAYING) {
				wave++;

				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);
				
				//Spawn in middle of arena
				player.spawn(arena, resolution, tileSize);

				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//Create a horde of zombies
				numZombies = 5 * wave;
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				//Play powerup sound
				powerup.play();

				//Reset the clock so there isn't a frame jump
				clock.restart();
			}
		} // End leveling up

		if (state == State::PLAYING) {
			//Update delta time
			Time dt = clock.restart();

			//Update the total game time
			gameTimeTotal += dt;

			//Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			//Convert mouse position to world coordinates of mainView
			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			//Set crosshair to the mouse world position
			spriteCrosshair.setPosition(mouseWorldPosition);

			player.update(dtAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());
			mainView.setCenter(player.getCenter());

			//Loop through each zombie and update them
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					bullets[i].update(dtAsSeconds);
				}
			}

			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			//Collision detection
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < numZombies; j++) {
					//Check if bullet is going to hit zombie
					if (bullets[i].isInFlight()
						&& zombies[j].isAlive()) {

						if (bullets[i].getPosition()
							.intersects(zombies[j].gePosition())) {
							bullets[i].stop();

							if (zombies[j].hit()) {
								score += 10;

								if (score >= highScore) {
									highScore = score;
								}
								numZombiesAlive--;
								
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}
							splat.play();
						}
					}
				}
			} // End zombie being shot

			for (int i = 0; i < numZombies; i++) {
				if (player.getPosition().intersects(zombies[i].gePosition())
					&& zombies[i].isAlive()) {
					if (player.hit(gameTimeTotal)) {
					
					}
					if (player.getHealth() <= 0) {
						state = State::GAME_OVER;

						std::ofstream outputFIle("gamedata/scores.txt");
						outputFIle << highScore;
						outputFIle.close();
					}
				}
			} // End player touched

			if (player.getPosition()
				.intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
				player.increaseHealthLevel(healthPickup.gotIt());
				pickup.play();
			}

			if (player.getPosition().intersects(ammoPickup.getPosition())
				&& ammoPickup.isSpawned()) {
				bulletsSpare += ammoPickup.gotIt();
				reload.play();
			}

			//Size up the health bar
			healthBar.setScale(Vector2f(player.getHealth() * 3, 50));
			framesSinceLastHUDUpdate++;

			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) {
				std::stringstream streamAmmo;
				std::stringstream streamScore;
				std::stringstream streamHighScore;
				std::stringstream streamWave;
				std::stringstream streamZombiesAlive;

				streamAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(streamAmmo.str());

				streamHighScore << "High Score:" << highScore;
				highScoreText.setString(streamHighScore.str());

				streamWave << "Wave:" << wave;
				waveNumberText.setString(streamWave.str());

				streamZombiesAlive << "Zombies:" << numZombiesAlive;
				zombieRemainingText.setString(streamZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
			} // End HUD update
		} // End updating the scene

		//Draw the scene
		if (state == State::PLAYING) {
			window.clear();
			window.setView(mainView);
			window.draw(background, &textureBackground);

			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
			}

			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					window.draw(bullets[i].getShape());
				}
			}

			//Draw player
			window.draw(player.getSprite());

			if (ammoPickup.isSpawned()) {
				window.draw(ammoPickup.getSprite());
			}

			if (healthPickup.isSpawned()) {
				window.draw(healthPickup.getSprite());
			}

			window.draw(spriteCrosshair);

			//Draw all the HUD elements
			window.setView(hudView);
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(highScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombieRemainingText);
		}

		if (state == State::LEVELING_UP) {
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::PAUSED) {
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER) {
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(highScoreText);
		}
		window.display();
	}

	delete[] zombies;
	return 0;
}