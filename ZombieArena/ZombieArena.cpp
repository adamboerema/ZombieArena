#include "stdafx.h"
#include "Player.h"
#include "ZombieArena.h"
#include "SFML/Graphics.hpp"
#include "TextureHolder.h"

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
	Texture textureBackground;
	VertexArray background;
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

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
				}

				if (state == State::PLAYING) {

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
		} // End playing

		if (state == State::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6) {
				state = State::PLAYING;
			}

			if (state == State::PLAYING) {
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);
				
				//Spawn in middle of arena
				player.spawn(arena, resolution, tileSize);

				//Create a horde of zombies
				numZombies = 10;
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

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

			player.update(dtAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());
			mainView.setCenter(player.getCenter());

			//Loop through each zombie and update them
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
		}

		//Draw the scene
		if (state == State::PLAYING) {
			window.clear();
			window.setView(mainView);
			window.draw(background, &textureBackground);

			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
			}

			window.draw(player.getSprite());
		}

		if (state == State::LEVELING_UP) {

		}
		if (state == State::PAUSED) {

		}
		if (state == State::GAME_OVER) {

		}
		window.display();
	}

	delete[] zombies;
	return 0;
}