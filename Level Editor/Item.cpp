#pragma once

#include "Item.h"
#include <iostream>

using namespace std;



void Item::SetCollidable(bool isCollidable) {

	collidable = isCollidable;
}



void Item::SetSprite(int spriteIndex) {


	//Load the Bricks texture
	if (!texture.loadFromFile("Assets/Textures/Breakout.png"))
	{
		//Error
		cout << "Couldn't load Breakout.png";
	}

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(40 + (width + 8)*spriteIndex, 120, width, height));
	sprite.setScale(sf::Vector2f(scale, scale));

}