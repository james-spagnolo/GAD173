#include "App.h"

using namespace std;

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
	gap = 50;
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


	//Game Logic Setup
	playerScore = 0;
	playerLives = 5;
	brickPoints = 10;
	bricksLeft = 0;

	//Text Setup
	font.loadFromFile("Assets/Fonts/RetroGaming.ttf");
	scoreColour = sf::Color::White;
	livesColour = sf::Color::Green;

	//Score Text Setup
	scoreText.setPosition(gap, 0);
	scoreText.setString("Score: " + to_string(playerScore));
	scoreText.setFont(font);
	scoreText.setFillColor(scoreColour);
	scoreText.setCharacterSize(24);

	//Live Text Setup
	livesText.setPosition(scoreText.getPosition().x + 4*gap, 0);
	livesText.setString("Lives: " + to_string(playerLives));
	livesText.setFont(font);
	livesText.setFillColor(livesColour);
	livesText.setCharacterSize(24);

	//Game Over Text Setup
	gameOverText.setPosition(window.getSize().x / 4, window.getSize().y / 2);
	gameOverText.setString("Game Over \nBricks Left: " + to_string(bricksLeft));
	gameOverText.setFont(font);
	gameOverText.setFillColor(scoreColour);
	gameOverText.setCharacterSize(48);



	//Setup the Array of Bricks
	for (int row = 0; row < ROWS; ++row)
	{
		for (int col = 0; col < COLS; ++col)
		{
			Bricks[row][col].setSize(sf::Vector2f(brickWidth, brickHeight));
			Bricks[row][col].setPosition(edgeGap + col * (brickWidth + gap), gap + row * (brickHeight + gap));
			collidable[row][col] = true;
			bricksLeft += 1;
			window.draw(Bricks[row][col]);
		}

	}

	return true;
}

void App::Update()
{
	/// UPDATE ///



	/// PADDLE INPUT DETECTION AND MOVEMENT ///
	

	// Checks that Player hasn't run out of lives
	if (playerLives > 0)
	{
		// Left Paddle Movement
		// Check if the left key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			// Checks if the paddle's position is greater than the left bound of the screen
			if (paddle.getPosition().x >= 0)
			{
				// Moves the paddle towards the left by paddle speed
				// Multiplied by deltaTime to keep it frame rate dependant
				paddle.move(-speed * deltaTime, 0);
			}
		}

		// Right Paddle Movement
		// Check if the right key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			// Checks that the paddles right bound won't intersect the right bound of the screen
			if (paddle.getPosition().x <= window.getSize().x - paddleWidth)
			{
				// Moves the paddle towards the right by paddle speed
				// Multiplied by deltaTime to keep it frame rate dependant
				paddle.move(speed * deltaTime, 0);
			}

		}
	}
	


	
	/// BALL & WALL COLLISION DETECTION ///
	
	// Left border collision detection
	// Detects when the ball's left bound meets the screens left bound
	if (ball.getPosition().x <= 0)
	{
		// Reverse horizontal velocity of the ball
		xSpeed = -xSpeed;
	}


	// Right border collision detection
	// Detects when the ball's right bound meets the screens right bound
	if (ball.getPosition().x >= window.getSize().x - 2 * radius)
	{
		// Reverse horizontal velocity of the ball
		xSpeed = -xSpeed;
	}


	// Top border collision detection
	// Detects when the ball's upper bound meets the screens upper bound
	if (ball.getPosition().y <= 0)
	{
		// Reverse the vertical velocity of the ball
		ySpeed = -ySpeed;
	}


	// Bottom border collision detection
	// Detects when the ball's lower bound meets the screens lower bound
	if (ball.getPosition().y >= window.getSize().y - 2 * radius)
	{
		// The player loses a life
		if (playerLives > 0)
		{
			playerLives -= 1;
		}

		// Reset the ball's position to be on top of the the middle of the paddle
		ball.setPosition(paddle.getPosition().x + (paddleWidth/2) - radius, paddle.getPosition().y - 2*radius);

		// Send the ball towards the bricks
		ySpeed = -ySpeed;
	}



	/// BALL & BRICK COLLISION DETECTION ///
	
	//For every row of bricks
	for (int row = 0; row < ROWS; ++row)
	{

		//And For every collumn of bricks
		for (int col = 0; col < COLS; ++col)
		{
			// Check IF the ball has hit the brick AND it hasn't already been hit
			if (ball.getGlobalBounds().intersects(Bricks[row][col].getGlobalBounds()) && collidable[row][col] == true)
			{
				// Destroy the brick - (It will no longer be collidable)
				collidable[row][col] = false;

				// Give the player score
				playerScore += brickPoints;

				// Reduces bricks left by 1
				bricksLeft -= 1;

				// Check IF the ball has hit the bricks Left OR Right border
				if (
					ball.getPosition().x > Bricks[row][col].getPosition().x + brickWidth - pad ||
					ball.getPosition().x + 2 * radius < Bricks[row][col].getPosition().x + pad
					)
				{
					// Reset position

					// Reverse x (horizontal) velocity of ball
					xSpeed = -xSpeed;
				}

				// Check IF the ball has hit the bricks Top OR Bottom border
				if (
					ball.getPosition().y + 2 * radius < Bricks[row][col].getPosition().y + pad ||
					ball.getPosition().y > Bricks[row][col].getPosition().y + brickHeight - pad
					)
				{
					// Reset position

					// Reverse y (vertical) velocity of ball
					ySpeed = -ySpeed;
				}

			}
		}
	}
	


	/// BALL & PADDLE COLLISION DETECTION ///
	// Check if the ball 
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


	/// BALL MOVEMENT ///
	
	//Move the ball if the player still has lives
	if (playerLives > 0)
	{
		// Move the ball based on speed and frame rate
		ball.move(xSpeed* deltaTime, ySpeed* deltaTime);
	}


	/// USER INTERFACE ///

	if (playerLives == 4)
	{
		livesColour = sf::Color::Color(128, 255, 0);
	}
	else if (playerLives == 3)
	{
		livesColour = sf::Color::Color(255, 255, 0);
	}
	else if (playerLives == 2)
	{
		livesColour = sf::Color::Color(255, 128, 0);
	}
	else if (playerLives <= 1)
	{
		livesColour = sf::Color::Color(255, 0, 0);
	}


	// Keeps the Score and Lives updated
	scoreText.setString("Score: " + to_string(playerScore));
	livesText.setString("Lives: " + to_string(playerLives));
	livesText.setFillColor(livesColour);
	gameOverText.setString("Game Over \nBricks Left: " + to_string(bricksLeft));


}



void App::Draw()
{
	window.clear();
	window.setView(view);

	/// DRAW ///
	
	//Checks the Player still has lives
	if (playerLives > 0)
	{
		//Draw the Paddle
		window.draw(paddle);

		//Draw the Ball
		window.draw(ball);
	}
	else
	{
		//Draw the Game Over text
		window.draw(gameOverText);
	}

	//Draw the Array of Bricks
	//For every row
	for (int row = 0; row < ROWS; ++row)
	{
		//For every collumn
		for (int col = 0; col < COLS; ++col)
		{
			//If it hasn't been hit already
			if (collidable[row][col])
			{
				//Draw a brick
				window.draw(Bricks[row][col]);
			}
		}
	}

	//Draw the Text
	window.draw(scoreText);
	window.draw(livesText);

	
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