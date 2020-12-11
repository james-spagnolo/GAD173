#pragma once

#include "Brick.h"
#include <iostream>

using namespace std;


void Brick::SetCollidable(bool isCollidable) {

	collidable = isCollidable;
}



void Brick::SetHealth(int hp) {

	health = hp;

	//if ()
}



void Brick::SetSprite(int spriteIndex) {


	//Load the Bricks texture
	if (!texture.loadFromFile("Assets/Textures/Breakout_Pieces.png"))
	{
		//Error
		cout << "Couldn't load file";
	}

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(8, 8 + 20 * spriteIndex, width, height));
	sprite.setScale(sf::Vector2f(scale, scale));

}



void Brick::ChangeHealth(int healthChange) {

	health += healthChange;

	if (health > 0) {

		this->SetCollidable(true);

		switch (health)
		{
			case 1: this->SetSprite(2); break; //Red
			case 2: this->SetSprite(3); break; //Purple
			case 3: this->SetSprite(0); break; //Blue
			case 4: this->SetSprite(1); break; //Green
			case 5: this->SetSprite(4); break; //Yellow
			case 6: this->SetSprite(6); break; //Brown
			case 7: this->SetSprite(5); break; //Silver
			default: break;
		}
		
	}

	else if (health <= 0) {

		this->SetCollidable(false);
	}
}





