#include "App.h"

//using namespace std;

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
	myfile.close();

	// release the memory for the dynamically allocated array
	for (int i = 0; i < ROWS; ++i) {
		delete[] bricks[i];
		delete[] collidable[i];
	}

	delete[] bricks;
	delete[] collidable;
}


bool App::Init()
{
	//Initialize App data members
	

	myfile.open("Brick Positions.txt");

	SetupGraphics(); //Initializes Sprites & Textures

	SetupAudio(); //Initializes Sounds & Audio

	SetupPaddle(); //Initializes Paddle

	SetupBall(); //Initializes Ball

	SetupUI(); //Initializes User Interface

	SetupGameLogic(); //Initializes GameLogic members

	SetupArrayOfBricks(); //Initializes 2D array of Bricks
	

	return true;
}




void App::Update()
{
	
	CheckGameState(); //Checks GameState every frame

	BallAndPaddle(); //Handles Ball & Paddle Movement / Collision
	
	BallAndWall(); //Handles Ball & Wall collisions

	BallAndBricks(); //Handles Ball & Brick Collisions
		
	RefreshUI(); //Refresh UI every frame

}



void App::Draw()
{
	window.clear();
	window.setView(view);

	/// DRAW ///
	
	DrawGameState(); //Draw current game state

	window.display();
	
}



void App::HandleEvents()
{
	if (event.type == sf::Event::Closed)
		window.close();

	// Other Keyboard, Mouse Events

	ButtonEvent();

	if (gameState == States::levelEditor)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			sf::Vector2i localPosition = sf::Mouse::getPosition(window);

			for (int row = 0; row < ROWS; ++row)
			{
				for (int col = 0; col < COLS; ++col)
				{
					if (bricks[row][col].getGlobalBounds().contains(sf::Vector2f(localPosition)))
					{
						bool click = false;

						if (!click) {

							collidable[row][col] = !collidable[row][col];
							click = true;
						}
						
					}
				}
			}

		}
	}
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



void App::SetupPaddle() {

	//Paddle Variables
	paddleHeight = 20;
	paddleWidth = 64;

	paddleScale = 2;

	scaledPaddleHeight = paddleHeight * paddleScale;
	scaledPaddleWidth = paddleWidth * paddleScale;

	speed = 350; //Paddle Movement Speed

	pad = 25; //Padding used to smoothen Collision Detection

	//Setup Paddle Sprites
	for (int i = 0; i < paddleIndex; i++) {


		paddleSprite[i].setTexture(breakoutParts);
		paddleSprite[i].setScale(sf::Vector2f(paddleScale, paddleScale));
		paddleSprite[i].setPosition(window.getSize().x / 2, window.getSize().y * 0.8);


		paddleSprite[i].setTextureRect(sf::IntRect(8 + i * 68, 151, paddleWidth, paddleHeight));
	}

	currentPaddle = 2;
}



void App::SetupBall() {

	//Ball
	radius = 4;
	ballScale = 1.2;
	scaledRadius = floor(radius * ballScale);


	xSpeed = rand() % 100 + 400; //Ball Speed (randomised for variation)
	ySpeed = -(rand() % 100 + 400); //Ball Speed (randomised for variation)


	xStart = paddleSprite[currentPaddle].getPosition().x; //Ball Initial x Position
	yStart = paddleSprite[currentPaddle].getPosition().y - (2 * scaledRadius + pad); //Ball Initial y Position


	//Setup Ball Sprites
	for (int i = 0; i < ballIndex; i++) {

		ballSprite[i].setTexture(breakoutParts);
		ballSprite[i].setScale(sf::Vector2f(scaledRadius, scaledRadius));
		ballSprite[i].setPosition(xStart, yStart);

		ballSprite[i].setTextureRect(sf::IntRect(48 + 9 * i, 136, 8, 8));
	}

	currentBall = rand() % ballIndex;
}



void App::SetupUI() {

	//Font Setup
	font.loadFromFile("Assets/Fonts/RetroGaming.ttf");


	textGap = 50;

	//Score Text Setup
	scoreColour = sf::Color::White;

	scoreText.setPosition(textGap, 0);
	scoreText.setString("Score: " + to_string(playerScore));
	scoreText.setFont(font);
	scoreText.setFillColor(scoreColour);
	scoreText.setCharacterSize(24);



	//Live Text Setup
	livesColour = sf::Color::Green;

	livesText.setPosition(scoreText.getPosition().x + 4 * textGap, 0);
	livesText.setString("Lives: " + to_string(playerLives));
	livesText.setFont(font);
	livesText.setFillColor(livesColour);
	livesText.setCharacterSize(24);



	//End Screen Text Setup
	endText.setPosition(window.getSize().x / 4, window.getSize().y / 8);
	endText.setString("");
	endText.setFont(font);
	endText.setFillColor(sf::Color::Red);
	endText.setCharacterSize(48);


	//Main Menu Text Setup
	mainText.setPosition(window.getSize().x / 3, window.getSize().y / 4);
	mainText.setString("Breakout");
	mainText.setFont(font);
	mainText.setFillColor(sf::Color::White);
	mainText.setCharacterSize(48);



	//Button Setup

	buttonWidth = 200;
	buttonHeight = 50;
	buttonX = (window.getSize().x / 2) - (buttonWidth / 2);

	for (int i = 0; i < buttonIndex; i++)
	{
		

		if (gameState == States::levelEditor) {

			button[i].setPosition(sf::Vector2f((buttonX - buttonWidth) + buttonWidth*buttonIndex, window.getSize().y - buttonHeight));
			button[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
			button[i].setFillColor(sf::Color::White);

		}

		else {

			button[i].setPosition(sf::Vector2f(buttonX, mainText.getPosition().y + (1.5 * (buttonHeight + (i * buttonHeight)))));
			button[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
			button[i].setFillColor(sf::Color::White);
		}

		buttonText[i].setPosition(sf::Vector2f(button[i].getPosition().x + (buttonWidth / 4), button[i].getPosition().y + (buttonHeight / 4)));
		buttonText[i].setFont(font);
		buttonText[i].setFillColor(sf::Color::Black);
		buttonText[i].setCharacterSize(24);

	}
}



void App::SetupAudio() {

	//Setup Sound files
	if (!brickBuffer.loadFromFile("Assets/Audio/BrickBreak.wav")) {}
	if (!missedBallBuffer.loadFromFile("Assets/Audio/MissedBall.wav"))
	if (!gameOverBuffer.loadFromFile("Assets/Audio/GameOver.ogg")) {}
	if (!winScreenBuffer.loadFromFile("Assets/Audio/WinSound.ogg")) {}
	if (!gameMusic.openFromFile("Assets/Audio/GameMusic.wav")) {}

	brickSound.setBuffer(brickBuffer);
	missedBallSound.setBuffer(missedBallBuffer);
	gameOverSound.setBuffer(gameOverBuffer);
	winScreenSound.setBuffer(winScreenBuffer);


	//Play Game Music on Initialization
	//gameMusic.play();

}



void App::SetupGraphics() {

	//Setup Texture files
	if (!breakoutParts.loadFromFile("Assets/Textures/Breakout_Pieces.png"))
	{
		//Error
	}

	

}



void App::SetupGameLogic() {

	//Game Logic Variables
	playerScore = 0; //Tracks Players Score - Starts at 0 and increases

	playerLives = 5; //Tracks Players Lives - Starts at 5 and drops to 0

	brickPoints = 10; //Tracks how many points Bricks are worth

	bricksLeft = 0; //Will track bricks remaining

	//state = 3; //Tracks the Game State - 0 = Play State

	gameState = States::menu; //Start at menu state

	endState = false; //Tracks if the game has ended

	playingSoundEffect = false; //Tracks if a sound effect is being played
}



void App::SetupArrayOfBricks() {

	//Brick Variables
	brickScale = 2;

	brickHeight = 16 * brickScale;
	brickWidth = 32 * brickScale;

	gap = 50;
	edgeGap = (window.getSize().x - COLS * brickWidth - (COLS - 1) * gap) / 2;

	bricks = new sf::Sprite * [ROWS];
	collidable = new bool* [ROWS];

	for (int row = 0; row < ROWS; ++row) {

		bricks[row] = new sf::Sprite[COLS];
		collidable[row] = new bool[COLS];
	}

	//Setup the Array of Bricks
	for (int row = 0; row < ROWS; ++row)
	{

		//For every Row and Collumn (or for every brick)
		for (int col = 0; col < COLS; ++col)
		{
			
			//Sets the texture for the bricks
			bricks[row][col].setTexture(breakoutParts);

			//Picks a random sprite for the brick
			int randomBrick = rand() % brickIndex;
			
			//Sets the sprite for the brick based on randomBrick
			bricks[row][col].setTextureRect(sf::IntRect(8, 8 + 20 * randomBrick, 32, 16));

			bricks[row][col].setScale(sf::Vector2f(brickScale, brickScale));
			
			

			//Sets the position of each brick
			bricks[row][col].setPosition(edgeGap + col * (brickWidth + gap), gap + row * (brickHeight + gap));

			myfile << bricks[row][col].getPosition().x << "\t" << bricks[row][col].getPosition().y << "\n";


			//collidable[row][col] = true; //Set the brick to be destroyable

			collidable[row][col] = true; //Set the brick to be destroyable

			bricksLeft += 1; //Increase bricks left by 1 for every brick created

			window.draw(bricks[row][col]);
		}

	}
}



void App::CheckGameState() {

	//Check the Game State
	switch (gameState)
	{
	case States::game:
		// State 0 = Play State
		endText.setString("");
		break;

	case States::gameOver:
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

	case States::winScreen:
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

	case States::menu:
		// State 3 = Menu State
		

	case States::levelEditor:
		// State 4 = Level Editor

	default:
		break;
	}




	//Check if the Player has run out of lives
	if (playerLives <= 0)
	{
		//Launch Game Over State
		gameState = States::gameOver;
	}
}



void App::BallAndPaddle() {

	// Handles Ball / Paddle Movement based on game state
	if (gameState == States::game)
	{

		// Left Paddle Movement - (Left Key Pressed)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{

			// Checks if the paddle's position is greater than the left bound of the screen
			if (paddleSprite[currentPaddle].getPosition().x >= 0)
			{

				// Moves the paddle towards the left by paddle speed
				// Multiplied by deltaTime to keep it frame rate dependant
				paddleSprite[currentPaddle].move(-speed * deltaTime, 0);
			}
		}


		// Right Paddle Movement - (Right Key Pressed)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{

			// Checks that the paddles right bound won't intersect the right bound of the screen
			if (paddleSprite[currentPaddle].getPosition().x <= window.getSize().x - scaledPaddleWidth)
			{

				// Moves the paddle towards the right by paddle speed
				// Multiplied by deltaTime to keep it frame rate dependant
				paddleSprite[currentPaddle].move(speed * deltaTime, 0);
			}

		}


		// Move the ball based on speed and frame rate
		ballSprite[currentBall].move(xSpeed * deltaTime, ySpeed * deltaTime);

	}


	// Check if the Ball collided with the Paddle
	if (ballSprite[currentBall].getGlobalBounds().intersects(paddleSprite[currentPaddle].getGlobalBounds())) {

		brickSound.play(); // Play Paddle Sound

		//Generate Random Sign: -1 or +1
		sign = 2 * (rand() % 2) - 1;


		// Left or right border
		if (
			ballSprite[currentBall].getPosition().x > paddleSprite[currentPaddle].getPosition().x + scaledPaddleWidth - pad ||
			ballSprite[currentBall].getPosition().x + 2 * scaledRadius < paddleSprite[currentPaddle].getPosition().x + pad
			)
		{
			// Reset position


			// Reverse x speed
			xSpeed = -xSpeed;
		}


		// Top or bottom border
		if (
			ballSprite[currentBall].getPosition().y + 2 * scaledRadius < paddleSprite[currentPaddle].getPosition().y + pad ||
			ballSprite[currentBall].getPosition().y > paddleSprite[currentPaddle].getPosition().y + scaledPaddleHeight - pad
			)
		{
			// Reset position

			// Reverse y speed
			ySpeed = -ySpeed;

			// Randomise Direction
			xSpeed = sign * xSpeed;
		}

	}
}



void App::BallAndWall() {

	// Left border collision detection
	// Detects when the ball's left bound meets the screens left bound
	if (ballSprite[currentBall].getPosition().x <= 0)
	{
		// Reset Position of Ball
		ballSprite[currentBall].setPosition(0, ballSprite[currentBall].getPosition().y);

		// Reverse horizontal velocity of the ball
		xSpeed = -xSpeed;
	}


	// Right border collision detection
	// Detects when the ball's right bound meets the screens right bound
	if (ballSprite[currentBall].getPosition().x >= window.getSize().x - 2 * scaledRadius)
	{
		// Reset Position of Ball
		ballSprite[currentBall].setPosition((window.getSize().x - 2 * radius), ballSprite[currentBall].getPosition().y);

		// Reverse horizontal velocity of the ball
		xSpeed = -xSpeed;
	}


	// Top border collision detection
	// Detects when the ball's upper bound meets the screens upper bound
	if (ballSprite[currentBall].getPosition().y <= 0)
	{

		// Reset Position of Ball
		ballSprite[currentBall].setPosition(ballSprite[currentBall].getPosition().x, 0);

		// Reverse the vertical velocity of the ball
		ySpeed = -ySpeed;
	}


	// Bottom border collision detection
	// Detects when the ball's lower bound meets the screens lower bound
	if (ballSprite[currentBall].getPosition().y >= window.getSize().y - 2 * scaledRadius) {

		// Checks the game hasn't ended
		if (playerLives > 0)
		{
			playerLives -= 1; //Player loses a life
		}

		missedBallSound.play(); //Play Missed Ball sound effect

		currentBall = rand() % ballIndex;

		// Reset the ball's position to be on top of the the middle of the paddle
		ballSprite[currentBall].setPosition(paddleSprite[currentPaddle].getPosition().x + (scaledPaddleWidth / 2) - scaledRadius, paddleSprite[currentPaddle].getPosition().y - (2 * scaledRadius + pad));

		// Send the ball towards the bricks
		ySpeed = -ySpeed;
	}
}



void App::BallAndBricks() {

	//For every row and collumn (for every brick)
	for (int row = 0; row < ROWS; ++row)
	{

		for (int col = 0; col < COLS; ++col)
		{

			// Check IF the ball has hit the brick AND it hasn't already been hit
			if (ballSprite[currentBall].getGlobalBounds().intersects(bricks[row][col].getGlobalBounds()) && collidable[row][col])
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
					gameState = States::winScreen;
				}

				// Check IF the ball has hit the bricks Left OR Right border
				if (
					ballSprite[currentBall].getPosition().x > bricks[row][col].getPosition().x + brickWidth - pad ||
					ballSprite[currentBall].getPosition().x + 2 * radius < bricks[row][col].getPosition().x + pad
					)
				{
					// Reset position

					// Reverse x (horizontal) velocity of ball
					xSpeed = -xSpeed;
				}

				// Check IF the ball has hit the bricks Top OR Bottom border
				if (
					ballSprite[currentBall].getPosition().y + 2 * radius < bricks[row][col].getPosition().y + pad ||
					ballSprite[currentBall].getPosition().y > bricks[row][col].getPosition().y + brickHeight - pad
					)
				{
					// Reset position

					// Reverse y (vertical) velocity of ball
					ySpeed = -ySpeed;
				}

			}
		}
	}

}



void App::RefreshUI() {

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


	//Update Buttons every frame
	for (int i = 0; i < buttonIndex; i++)
	{

		if (gameState == States::levelEditor) {

			button[i].setPosition(sf::Vector2f((buttonX - 0.5*buttonWidth) + i*(buttonWidth + 5), window.getSize().y - 2*buttonHeight));
		}

		else {

			button[i].setPosition(sf::Vector2f(buttonX, mainText.getPosition().y + (1.5 * (buttonHeight + (i * buttonHeight)))));
		}

		button[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
		button[i].setFillColor(sf::Color::White);

		buttonText[i].setPosition(sf::Vector2f(button[i].getPosition().x + (buttonWidth / 4), button[i].getPosition().y + (buttonHeight / 4)));
		buttonText[i].setFont(font);
		buttonText[i].setFillColor(sf::Color::Black);
		buttonText[i].setCharacterSize(24);

	}


	//Sets button text based on game state
	switch (gameState) {

	case States::game:
		//Play State
		break;

	case States::gameOver:
		//GameOver State
		buttonText[0].setString("Retry");
		buttonText[1].setString("Menu");
		break;

	case States::winScreen:
		//Win State
		buttonText[0].setString("Retry");
		buttonText[1].setString("Menu");
		break;

	case States::menu:
		//Menu State
		buttonText[0].setString("Play");
		buttonText[1].setString("Level Editor");
		break;

	case States::levelEditor:
		//Level Editor
		buttonText[0].setString("Back");
		buttonText[1].setString("Load");
		break;

	default:
		break;
	}
}



void App::RestartGame() {

	SetupPaddle(); //Initializes Paddle

	SetupBall(); //Initializes Ball

	SetupUI(); //Initializes User Interface

	SetupGameLogic(); //Initializes GameLogic members

	SetupArrayOfBricks(); //Initializes 2D array of Bricks
}



void App::ButtonEvent() {

	for (int i = 0; i < buttonIndex; ++i) {

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			sf::Vector2i localPosition = sf::Mouse::getPosition(window);

			if (button[i].getGlobalBounds().contains(sf::Vector2f(localPosition))) {

				switch (gameState) {

				case States::game:
					//Play State
					break;

				case States::gameOver:
					//GameOver State
					//Button 1
					if (i == 0) {

						RestartGame(); //Resets the game
						gameMusic.play();
						gameState = States::game; //Relaunch the game
					}

					//Button 2
					if (i == 1) {

						RestartGame(); //Resets the game
						gameState = States::menu; //Return to main menu
					}
					break;

				case States::winScreen:
					//Win State

					//Button 1
					if (i == 0) {

						RestartGame(); //Resets the game
						gameMusic.play();
						gameState = States::game; //Launch Game
					}

					//Button 2
					if (i == 1) {

						RestartGame(); //Resets the game
						gameState = States::menu; //Return to main menu
					}
					break;

				case States::menu:
					//Menu State

					//Button 1
					if (i == 0) {

						RestartGame(); //Resets the game
						gameMusic.play();
						gameState = States::game; //Launch Game
					}

					//Button 2
					if (i == 1) {

						RestartGame(); //Resets the game
						gameState = States::levelEditor; //Launch Level Editor
					}
					break;

				case States::levelEditor:
					//Level Editor

					//Button 1
					if (i == 0) {

						RestartGame(); //Resets the game
						gameState = States::menu; //Return to Main Menu
					}
				default:
					break;
				}


			}
		}
	}
}



void App::DrawGameState() {

	//Checks the Player is alive and there's bricks left

	switch (gameState)
	{
	case States::game:
		// State 0 = Play State
		//window.draw(paddle);

		window.draw(paddleSprite[currentPaddle]);
		window.draw(ballSprite[currentBall]);

		//Draw the Text
		window.draw(scoreText);
		window.draw(livesText);

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
					window.draw(bricks[row][col]);
				}
			}
		}
		break;

	case States::gameOver:
		// State 1 = GameOver State
		window.draw(endText);

		for (int i = 0; i < buttonIndex; i++) {
			window.draw(button[i]);
			window.draw(buttonText[i]);
		}
		break;

	case States::winScreen:
		// State 2 = Win State
		window.draw(endText);

		for (int i = 0; i < buttonIndex; i++) {
			window.draw(button[i]);
			window.draw(buttonText[i]);
		}

		break;

	case States::menu:
		// State 3 = Menu State
		window.draw(mainText);

		for (int i = 0; i < buttonIndex; i++) {
			window.draw(button[i]);
			window.draw(buttonText[i]);
		}
		break;

	case States::levelEditor:
		// State 4 = Level Editor

		// Draw Array of Bricks
		for (int row = 0; row < ROWS; ++row) {

			//For every collumn
			for (int col = 0; col < COLS; ++col) {

				//If it hasn't been hit already
				if (collidable[row][col]) {

					//Draw a brick
					window.draw(bricks[row][col]);
				}
			}
		}

		for (int i = 0; i < buttonIndex; ++i) {

			window.draw(button[i]);
			window.draw(buttonText[i]);
		}
		

	default:
		break;
	}
}