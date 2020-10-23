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

	//Paddle
	paddleHeight = 30;
	paddleWidth = 100;
	speed = 350;
	pad = 20;

	//Set Paddle Properties
	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	paddle.setFillColor(sf::Color::Green);
	paddle.setPosition(window.getSize().x / 2, window.getSize().y * 0.9);

	//Bricks
	brickWidth = 100;
	brickHeight = 30;
	gap = 20;
	edgeGap = (window.getSize().x - COLS * brickWidth - (COLS - 1) * gap) / 2;


	//Ball
	radius = 25;
	xSpeed = rand() % 100 + 400;
	ySpeed = -(rand() % 100 + 400);
	xStart = paddle.getPosition().x;
	yStart = paddle.getPosition().y - 2*paddleHeight;

	//Set Ball Properties
	ball.setPosition(xStart, yStart);
	ball.setRadius(radius);


	// initialise the Array of Bricks
	for (int row = 0; row < ROWS; ++row)
	{
		for (int col = 0; col < COLS; ++col)
		{
			Bricks[row][col].setSize(sf::Vector2f(brickWidth, brickHeight));
			Bricks[row][col].setPosition(edgeGap + col * (brickWidth + gap), gap + row * (brickHeight + gap));
			collidable[row][col] = true;
			window.draw(Bricks[row][col]);
		}

	}

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


	
	/// BALL COLLISION DETECTION ///
	// Left border collision detection
	if (ball.getPosition().x <= 0)
	{
		xSpeed = -xSpeed;
	}

	// Right border collision detection
	if (ball.getPosition().x >= window.getSize().x - 2 * radius)
	{
		xSpeed = -xSpeed;
	}

	// Top border collision detection
	if (ball.getPosition().y <= 0)
	{
		ySpeed = -ySpeed;
	}

	// Bottom border collision detection
	if (ball.getPosition().y >= window.getSize().y - 2 * radius)
	{
		ball.setPosition(xStart, yStart);
		ySpeed = -ySpeed;
	}

	// Detect collision with collidable brick 
	// Check if brick has been hit
	for (int row = 0; row < ROWS; ++row)
	{
		for (int col = 0; col < COLS; ++col)
		{
			if (ball.getGlobalBounds().intersects(Bricks[row][col].getGlobalBounds()) && collidable[row][col] == true)
			{
				// Destroy the brick
				collidable[row][col] = false;

				// Left or right border
				if (
					ball.getPosition().x > Bricks[row][col].getPosition().x + brickWidth - pad ||
					ball.getPosition().x + 2 * radius < Bricks[row][col].getPosition().x + pad
					)
				{
					// Reset position

					// Reverse x speed
					xSpeed = -xSpeed;
				}

				// Top or bottom border
				if (
					ball.getPosition().y + 2 * radius < Bricks[row][col].getPosition().y + pad ||
					ball.getPosition().y > Bricks[row][col].getPosition().y + brickHeight - pad
					)
				{
					// Reset position

					// Reverse y speed
					ySpeed = -ySpeed;
				}

			}
		}
	}
	

	// Detect collision between Ball and Paddle
	if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()))
	{
		
		// Left or right border
		if (
			ball.getPosition().x > paddle.getPosition().x + paddleWidth - pad ||
			ball.getPosition().x + 2 * radius < paddle.getPosition().x + pad
			)
		{
			// reset position

			// Reverse x speed
			xSpeed = -xSpeed;
		}

		// Top or bottom border
		if (
			ball.getPosition().y + 2 * radius < paddle.getPosition().y + pad ||
			ball.getPosition().y > paddle.getPosition().y + paddleHeight - pad
			)
		{
			// Reset position

			// Reverse y speed
			ySpeed = -ySpeed;
		}

	}

	// Move the ball 
	ball.move(xSpeed * deltaTime, ySpeed * deltaTime);

}



void App::Draw()
{
	window.clear();
	window.setView(view);
	// Draw
	window.draw(paddle);
	window.draw(ball);

	// draw the bricks
	for (int row = 0; row < ROWS; ++row)
	{
		for (int col = 0; col < COLS; ++col)
		{
			if (collidable[row][col])
			{
				window.draw(Bricks[row][col]);
			}
		}
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