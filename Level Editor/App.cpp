#include "App.h"

//Constructor
App::App(const char* title, int screenWidth, int screenHeight, int screenBpp)
{
	window.create(sf::VideoMode(screenWidth, screenHeight, screenBpp), title);
	window.setFramerateLimit(0);
	view = window.getDefaultView();
}

//Destructor
App::~App()
{
	//Release Memory
}


bool App::Init()
{
	//Initialize App data members
	paddleHeight = 30;
	paddleWidth = 100;
	speed = 350;

	radius = 25;
	xSpeed = rand() % 100 + 400;
	ySpeed = rand() % 100 + 400;

	pad = 20;

	brickWidth = 200;
	brickHeight = 100;
	collidable = true;

	//Set Paddle Properties
	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	paddle.setFillColor(sf::Color::Green);
	paddle.setPosition(window.getSize().x / 2, window.getSize().y * 0.9);

	//Set Brick Properties
	brick.setPosition(400, 400);
	brick.setSize(sf::Vector2f(brickWidth, brickHeight));

	//Set Ball Properties
	ball.setPosition(80, 80);
	ball.setRadius(radius);

	return true;
}

void App::Update()
{
	//Update

	// move Paddle left, frame rate independent
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (paddle.getPosition().x >= 0)
		{
			paddle.move(-speed * deltaTime, 0);
		}
	}

	// move Paddle right, frame rate independent
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (paddle.getPosition().x <= window.getSize().x - paddleWidth)
		{
			paddle.move(speed * deltaTime, 0);
		}

	}

	// left border collision detection
	if (ball.getPosition().x <= 0)
	{
		xSpeed = -xSpeed;
	}

	// right border collision detection
	if (ball.getPosition().x >= window.getSize().x - 2 * radius)
	{
		xSpeed = -xSpeed;
	}

	// top border collision detection
	if (ball.getPosition().y <= 0)
	{
		ySpeed = -ySpeed;
	}

	// bottom border collision detection
	if (ball.getPosition().y >= window.getSize().y - 2 * radius)
	{
		ySpeed = -ySpeed;
	}

	// detect collision with collidable brick 
	// i.e. if brick has not been hit
	if (ball.getGlobalBounds().intersects(brick.getGlobalBounds()) && collidable)
	{
		// destroy the brick
		collidable = false;

		// left or right border
		if (
			ball.getPosition().x > brick.getPosition().x + brickWidth - pad ||
			ball.getPosition().x + 2 * radius < brick.getPosition().x + pad
			)
		{
			// reset position

			// reverse x speed
			xSpeed = -xSpeed;
		}

		// top or bottom border
		if (
			ball.getPosition().y + 2 * radius < brick.getPosition().y + pad ||
			ball.getPosition().y > brick.getPosition().y + brickHeight - pad
			)
		{
			// reset position

			// reverse y speed
			ySpeed = -ySpeed;
		}

	}

	// move the ball 
	ball.move(xSpeed * deltaTime, ySpeed * deltaTime);
}



void App::Draw()
{
	window.clear();
	window.setView(view);
	// Draw
	window.draw(paddle);
	window.draw(ball);

	if (collidable)
	{
		window.draw(brick);
	}

	window.display();
	
}



void App::HandleEvents()
{
	if (event.type == sf::Event::Closed)
		window.close();

	// Other Keyboard, Mouse Events
}




void App::Run()
{
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		
		while (window.pollEvent(event))
		{
			HandleEvents();
		}
		Update();
		Draw();
	}
}