#pragma once

#include "SFML/Graphics.hpp"


class Item : public sf::RectangleShape {

public:
	
	sf::Sprite sprite;

	sf::Time spawnTimer = sf::seconds(10.0f);

	sf::Time textTimer = sf::seconds(2.0f);


	void SetCollidable(bool isCollidable);
	void SetSprite(int spriteIndex);

	void SetItem();

	bool GetCollidable() {

		return collidable;
	}

	float GetSpeed() {

		return speed;
	}
	
	sf::Vector2f GetSize() {

		return sf::Vector2f(scale * width, scale * height);
	}



private:

	float width = 32;
	float height = 32;

	float scale = 1;

	float speed = 300;

	bool collidable = false;

	sf::Texture texture;

};