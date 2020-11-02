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


	//Paddle Variables
	paddleHeight = 30;
	paddleWidth = 100;

	speed = 350; //Paddle Movement Speed

	pad = 20; //Padding used to smoothen Collision Detection


	//Set Paddle Properties
	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	paddle.setFillColor(sf::Color::Green);
	paddle.setPosition(window.getSize().x / 2, window.getSize().y * 0.9);



	//Brick Variables
	brickHeight = 30;
	brickWidth = 100;

	gap = 50;
	edgeGap = (window.getSize().x - COLS * brickWidth - (COLS - 1) * gap) / 2;



	//Ball
	radius = 25;

	xSpeed = rand() % 100 + 400; //Ball Speed (randomised for variation)

	ySpeed = -(rand() % 100 + 400); //Ball Speed (randomised for variation)

	xStart = paddle.getPosition().x; //Ball Initial x Position

	yStart = paddle.getPosition().y - (2*radius + 1); //Ball Initial y Position


	//Set Ball Properties
	ball.setPosition(xStart, yStart);
	ball.setRadius(radius);
	ball.setFillColor(sf::Color::Yellow);



	//Font Setup
	font.loadFromFile("Assets/Fonts/RetroGaming.ttf");
	


	//Score Text Setup
	scoreColour = sf::Color::White;

	scoreText.setPosition(gap, 0);
	scoreText.setString("Score: " + to_string(playerScore));
	scoreText.setFont(font);
	scoreText.setFillColor(scoreColour);
	scoreText.setCharacterSize(24);



	//Live Text Setup
	livesColour = sf::Color::Green;

	livesText.setPosition(scoreText.getPosition().x + 4*gap, 0);
	livesText.setString("Lives: " + to_string(playerLives));
	livesText.setFont(font);
	livesText.setFillColor(livesColour);
	livesText.setCharacterSize(24);



	//End Screen Text Setup
	endText.setPosition(window.getSize().x / 4, window.getSize().y / 2);
	endText.setString("");
	endText.setFont(font);
	endText.setFillColor(sf::Color::Red);
	endText.setCharacterSize(48);



	//Setup Sound files
	if (!brickBuffer.loadFromFile("Assets/Audio/BrickBreak.wav")) { }
	if (!missedBallBuffer.loadFromFile("Assets/Audio/MissedBall.wav"))
	if (!gameOverBuffer.loadFromFile("Assets/Audio/GameOver.ogg")) { }
	if (!winScreenBuffer.loadFromFile("Assets/Audio/WinSound.ogg")) { }
	if (!gameMusic.openFromFile("Assets/Audio/GameMusic.wav")) { }
	
	brickSound.setBuffer(brickBuffer);
	missedBallSound.setBuffer(missedBallBuffer);
	gameOverSound.setBuffer(gameOverBuffer);
	winScreenSound.setBuffer(winScreenBuffer);


	//Play Game Music on Initialization
	gameMusic.play();


	//Game Logic Variables
	playerScore = 0; //Tracks Players Score - Starts at 0 and increases

	playerLives = 5; //Tracks Players Lives - Starts at 5 and drops to 0

	brickPoints = 10; //Tracks how many points Bricks are worth

	bricksLeft = 0; //Will track bricks remaining

	state = 0; //Tracks the Game State - 0 = Play State

	endState = false; //Tracks if the game has ended

	playingSoundEffect = false; //Tracks if a sound effect is being played



	//Setup the Array of Bricks
	for (int row = 0; row < ROWS; ++row) 
	{
		
		for (int col = 0; col < COLS; ++col) 
		{
			//For every Row and Collumn (or for every brick)
			Bricks[row][col].setSize(sf::Vector2f(brickWidth, brickHeight));
			Bricks[row][col].setPosition(edgeGap + col * (brickWidth + gap), gap + row * (brickHeight + gap));

			collidable[row][col] = true; //Set the brick to be destroyable

			bricksLeft += 1; //Increase bricks left by 1 for every brick created

			window.draw(Bricks[row][col]);
		}

	}

	return true;
}




void App::Update()
{
	/// UPDATE ///


	//Check the Game State
	switch (state)
	{
		case 0:
			// State 0 = Play State
			endText.setString("");
			endState = false; //Game still running
			break;

		case 1:
			// State 1 = Game Over
			endText.setString("Game Over \nBricks Left: " + to_string(bricksLeft));
			endText.setFillColor(sf::Color::Red);

			endState = true; //Game has ended

			gameMusic.stop(); //Music stops playing

			if (!playingSoundEffect) 
			{
				gameOverSound.play(); //Play Game Over sound effect
				playingSoundEffect = true; //Makes sure sound effect plays only once
			}
			break;

		case 2:
			// State 2 = Win Screen
			endText.setString("You Win! \nScore: " + to_string(playerScore));
			endText.setFillColor(sf::Color::Green);
			scoreColour = sf::Color::Green;

			endState = true; //Game has ended

			gameMusic.stop(); //Music stops playing

			if (!playingSoundEffect)
			{
				winScreenSound.play(); //Play Win Screen sound effect
				playingSoundEffect = true; //Makes sure sound effect plays only once
			}
			break;

		default:
			break;
	}


	//Check if the Player has run out of lives
	if (playerLives <= 0)
	{
		//Launch Game Over State
		state = 1;
	}

	/// PADDLE INPUT DETECTION AND MOVEMENT ///
	

	// Checks that the game hasn't ended
	if (!endState)
	{

		// Left Paddle Movement - (Left Key Pressed)
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


		// Right Paddle Movement - (Right Key Pressed)
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

		// Checks the game hasn't ended
		if (!endState)
		{
			playerLives -= 1; //Player loses a life
		}

		missedBallSound.play(); //Play Missed Ball sound effect

		// Reset the ball's position to be on top of the the middle of the paddle
		ball.setPosition(paddle.getPosition().x + (paddleWidth/2) - radius, paddle.getPosition().y - 2*radius);

		// Send the ball towards the bricks
		ySpeed = -ySpeed;
	}



	/// BALL & BRICK COLLISION DETECTION ///
	
	//For every row and collumn (for every brick)
	for (int row = 0; row < ROWS; ++row)
	{

		for (int col = 0; col < COLS; ++col)
		{

			// Check IF the ball has hit the brick AND it hasn't already been hit
			if (ball.getGlobalBounds().intersects(Bricks[row][col].getGlobalBounds()) && collidable[row][col] == true)
			{

				// Destroy the brick - (It will no longer be collidable)
				collidable[row][col] = false;


				brickSound.play(); // Play Brick Break Sound


				playerScore += brickPoints; //Score increased by brick point worth


				bricksLeft -= 1; //Bricks remaining reduced by 1


				// Check if all Bricks have been hit
				if (bricksLeft <= 0)
				{
					//Launch the Win State
					state = 2;
				}

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
	
	// Check if the Ball collided with the Paddle
	if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()))
	{
		
		brickSound.play(); // Play Paddle Sound


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
	
	//Check that the game hasn't ended
	if (!endState)
	{
		// Move the ball based on speed and frame rate
		ball.move(xSpeed* deltaTime, ySpeed* deltaTime);
	}


	/// USER INTERFACE ///

	//Check how many lives the player has
	switch (playerLives)
	{
		case 5:
			//If 5 Lives, Lives Colour = Green
			livesColour = sf::Color::Green;
			break;
		case 4:
			//If 4 Lives, Lives Colour = Lime
			livesColour = sf::Color::Color(128, 255, 0);
			break;
		case 3:
			//If 3 Lives, Lives Colour = Yellow
			livesColour = sf::Color::Yellow;
			break;
		case 2:
			//If 2 Lives, Lives Colour = Orange
			livesColour = sf::Color::Color(255, 128, 0);
			break;
		case 1:
			//If 1 Live, Lives Colour = Red
			livesColour = sf::Color::Red;
			break;
		case 0:
			//If 0 Lives, Lives Colour = Red
			livesColour = sf::Color::Red;
			break;
		default:
			break;
	}


	// Keeps the Score and Lives text updated
	scoreText.setString("Score: " + to_string(playerScore));
	livesText.setString("Lives: " + to_string(playerLives));
	livesText.setFillColor(livesColour);
	scoreText.setFillColor(scoreColour);

}



void App::Draw()
{
	window.clear();
	window.setView(view);

	/// DRAW ///
	
	//Checks the Player is alive and there's bricks left
	if (!endState)
	{
		//Draw the Paddle
		window.draw(paddle);

		//Draw the Ball
		window.draw(ball);
	}
	else if (endState)
	{
		//Draw the Game Over text
		window.draw(endText);
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