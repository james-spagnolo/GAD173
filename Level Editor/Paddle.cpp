#pragma once

#include "Paddle.h"
#include <iostream>

using namespace std;


void Paddle::SetSprite(int spriteIndex) {

	if (!texture.loadFromFile("Assets/Textures/Breakout_Pieces.png"))
	{
		//Error
		cout << "Couldn't load file";
	}

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(8 + spriteIndex * 68, 151, width, height));

	sprite.setScale(sf::Vector2f(scale, scale));
}



void Paddle::SetSpeed(float spd) {

	speed = spd;

	if (speed >= maxSpeed) {

		speed = maxSpeed;
	}
}