#pragma once

#include "SFML/Graphics.hpp"



class Paddle : public sf::RectangleShape {

public:

	sf::Sprite sprite;

	void SetCollidable(bool isCollidable) {

		collidable = isCollidable;
	}

	void SetSprite(int spriteIndex);

	void SetSize(sf::Vector2f size) {

		this->setSize(size);
	}

	void SetSpeed(float spd);

	bool GetCollidable() {

		return collidable;
	}

	float GetSpeed() {

		return speed;
	}

	float GetDefaultSpeed() {

		return defaultSpeed;
	}

	float GetSizeChange() {

		return sizeChange;
	}

	float GetSpeedChange() {

		return speedChange;
	}

	sf::Vector2f GetSize() {

		return sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
	}

	sf::Vector2f GetDefaultSize() {

		return defaultSize;
	}

private:

	bool collidable;

	float scale = 2;
	float width = 64;
	float height = 20;

	sf::Vector2f defaultSize = sf::Vector2f(scale * width, scale * height);

	float defaultSpeed = 350;
	float speed = 350;
	float maxSpeed = 1000;

	float speedChange = 1.5f;
	float sizeChange = scale * 0.6f;

	sf::Texture texture;
};