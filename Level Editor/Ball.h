#pragma once

#include "SFML/Graphics.hpp"



class Ball : public sf::CircleShape {

public:

	sf::Sprite sprite;

	void SetCollidable(bool isCollidable) {

		collidable = isCollidable;
	}

	void SetSprite(int spriteIndex);

	void SetPower(int pwr);

	bool GetCollidable() {

		return collidable;
	}

	float GetRadius() {

		return 0.5f*sprite.getGlobalBounds().width;
	}

	float GetDefaultRadius() {

		return defaultRadius;
	}

	float GetSizeChange() {

		return sizeChange;
	}

	int GetPower() {

		return power;
	}

	int GetDefaultPower() {

		return defaultPower;
	}

private:

	float radius = 4;
	float scale = 2.0f;
	float defaultRadius = scale * radius;

	float sizeChange = 1.5f;

	bool collidable;

	int defaultPower = 1;
	int power = 1;
	int maxPower = 3;

	sf::Texture texture;
};