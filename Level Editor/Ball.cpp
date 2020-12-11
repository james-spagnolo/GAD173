#pragma once

#include "Ball.h"
#include <iostream>

using namespace std;


void Ball::SetSprite(int spriteIndex) {

	if (!texture.loadFromFile("Assets/Textures/Breakout_Pieces.png"))
	{
		//Error
		cout << "Couldn't load file";
	}

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(48 + 9 * spriteIndex, 136, 2*radius, 2*radius));
	sprite.setScale(sf::Vector2f(scale, scale));
}



void Ball::SetPower(int pwr) {

	power = pwr;

	if (power > maxPower) {

		power = maxPower;
	}
}