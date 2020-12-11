#pragma once

#include "Enemy.h"
#include <iostream>

using namespace std;


void Enemy::SetSpriteSheet(int spriteIndex) {

	if (!texture.loadFromFile("Assets/Textures/Bird.png")) {

		cout << "Couldn't load Bird.png";
	}

	sprite.setTexture(texture);

	sprite.setTextureRect(rect);

	sprite.setScale(scale, scale);
}