#pragma once

#include "SFML/Graphics.hpp"


//#include "App.h"

class Brick : public sf::RectangleShape {

public:
	
	sf::Sprite sprite;

	void SetCollidable(bool isCollidable);
	void SetHealth(int hp);
	void SetSprite(int spriteIndex);

	int GetHealth() {

		return health;
	}

	int GetMaxHealth() {

		return maxHealth;
	}

	bool GetCollidable() {

		return collidable;
	}

	sf::Vector2f GetSize() {

		return sf::Vector2f(scale * width, scale * height);
	}
	
	void ChangeHealth(int healthChange);


private:
	float height = 16;
	float width = 32;

	float scale = 2;

	bool collidable;

	int health;
	int maxHealth = 7;

	sf::Texture texture;
};