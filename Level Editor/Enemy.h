#pragma once

#include "SFML/Graphics.hpp"

class Enemy : public sf::RectangleShape {

	public:

		sf::Sprite sprite;

		sf::IntRect rect = sf::IntRect(0, 0, 48, 48);

		int spriteSize = 48;

		void SetSpriteSheet(int spriteIndex);

		void SetCollidable(bool isCollidable) {

			collidable = isCollidable;
		}

		void SetSpriteCycle(bool isFirstCycle) {

			cycle = isFirstCycle;
		}


		float GetSpeed() {

			return speed;
		}

		int GetSpriteIndex() {
			return spriteIndex;
		}

		bool GetCollidable() {

			return collidable;
		}

		sf::Vector2f GetSize() {

			return sf::Vector2f(scale * width, scale * height);
		}

		sf::Time GetFireRate() {

			return fireRate;
		}

		sf::Time GetSpriteDuration() {

			return spriteDuration;
		}

		bool GetSpriteCycle() {

			return cycle;
		}

	private:

		const int ROWS = 8;
		const int COLS = 12;

		sf::Texture texture;

		float width = 48;
		float height = 48;
		float scale = 2;

		float speed = 100;

		int spriteIndex = 2;

		sf::Time fireRate = sf::seconds(10.0f);

		sf::Time spriteDuration = sf::seconds(1.0f);

		bool collidable = true;

		bool cycle = true;

		
};