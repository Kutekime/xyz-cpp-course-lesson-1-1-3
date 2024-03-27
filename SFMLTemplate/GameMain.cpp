// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


//Init game constants
const std::string RESOURCES_PATH = "Resources/";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float INITIAL_SPEED = 100.f; // Pixels per seconds
const float PLAYER_SIZE = 20.f;
const float ACCELERATION = 200000.f; // Pixels per seconds per seconds
const int NUM_APPLES = 20;
const float APPLE_SIZE = 20.f;

int main()
{
	int seed = (int)time(nullptr);
	srand(seed);
	//Init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples game!");

	//Init player state
	float playerX = SCREEN_WIDTH / 2.f;
	float playerY = SCREEN_HEIGHT / 2.f;
	float playerSpeed = INITIAL_SPEED;
	int playerDirection = 0; //0 - right, 1 - up, 2 - left, 3 - down

	//Init player shape
	sf::RectangleShape playerShape;
	playerShape.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
	playerShape.setFillColor(sf::Color::Red);
	playerShape.setOrigin(PLAYER_SIZE / 2.f, PLAYER_SIZE / 2.f);
	playerShape.setPosition(playerX, playerY);

	//Init apples
	float appleX[NUM_APPLES];
	float appleY[NUM_APPLES];
	sf::CircleShape	 applesShape[NUM_APPLES];

	//Inits score
	//float scoreText = '0';
	sf::Font font;
	font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf");
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::Yellow);
	scoreText.setPosition(10.f, 10.f);

	for (int i = 0; i < NUM_APPLES; ++i)
	{
		appleX[i] = rand() / (float)RAND_MAX * SCREEN_WIDTH;
		appleY[i] = rand() / (float)RAND_MAX * SCREEN_HEIGHT;

		applesShape[i].setRadius(APPLE_SIZE / 2.f);
		applesShape[i].setFillColor(sf::Color::Green);
		applesShape[i].setOrigin(APPLE_SIZE / 2.f, APPLE_SIZE / 2.f);
		applesShape[i].setPosition(appleX[i], appleY[i]);
	}

	int numEatenApples = 0;
	scoreText.setString("Apples eaten: " + std::to_string(numEatenApples));

	//Init game clock
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();
	
	//Main loop
	while (window.isOpen())
	{
		//calculate time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//Read events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Handle input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			playerDirection = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			playerDirection = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			playerDirection = 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			playerDirection = 3;
		}

		

		//Update player state
		if (playerDirection == 0)
		{
			playerX += playerSpeed * deltaTime;
		}
		else if (playerDirection == 1)
		{
			playerY -= playerSpeed * deltaTime;
		}
		else if (playerDirection == 2)
		{
			playerX -= playerSpeed * deltaTime;
		}
		else if (playerDirection == 3)
		{
			playerY += playerSpeed * deltaTime;
		}

		//Chek boreder screen collision
		if (playerX - PLAYER_SIZE / 2.f < 0.f || playerX + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
			playerY - PLAYER_SIZE / 2.f < 0.f || playerY + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
		{
			//Stop game
			playerSpeed = 0;
			scoreText.setString("GAME OVER!!! Apples eaten: " + std::to_string(numEatenApples) + ". Press Enter!");
			scoreText.setFillColor(sf::Color::Red);
			scoreText.setPosition(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2.5f);
			/*scoreText.setFillColor(sf::Color::Yellow);
			scoreText.setPosition(10.f, 10.f);*/

			//Restart
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				numEatenApples = 0;
				
				scoreText.setString("Apples eaten: " + std::to_string(numEatenApples));
				scoreText.setFillColor(sf::Color::Yellow);
				scoreText.setPosition(10.f, 10.f);
				playerX = SCREEN_WIDTH / 2.f;
				playerY = SCREEN_HEIGHT / 2.f;
				playerSpeed = INITIAL_SPEED;
				playerDirection = 0;
			}
		}

		for (int i = 0; i < NUM_APPLES; ++i)
		{
			//Check colliscions for squares

			/*float dx = fabs(playerX - appleX[i]);
			float dy = fabs(playerY - appleY[i]);
			if (dx <= (APPLE_SIZE + PLAYER_SIZE) / 2.f &&
				dy <= (APPLE_SIZE + PLAYER_SIZE) / 2.f)
			{
				++numEatenApples;
			}*/

			//Check colliscions for circles
			float squareDistance = (playerX - appleX[i]) * (playerX - appleX[i]) +
				(playerY - appleY[i]) * (playerY - appleY[i]);
			float squareRadiusSum = (APPLE_SIZE + PLAYER_SIZE) * (APPLE_SIZE + PLAYER_SIZE) / 4;
			if (squareDistance <= squareRadiusSum)
			{
				++numEatenApples;

				//create new apple
				appleX[i] = rand() / (float)RAND_MAX * SCREEN_WIDTH;
				appleY[i] = rand() / (float)RAND_MAX * SCREEN_HEIGHT;

				applesShape[i].setRadius(APPLE_SIZE / 2.f);
				applesShape[i].setFillColor(sf::Color::Green);
				applesShape[i].setOrigin(APPLE_SIZE / 2.f, APPLE_SIZE / 2.f);
				applesShape[i].setPosition(appleX[i], appleY[i]);
				
				
				// Acceleration
				playerSpeed += ACCELERATION * deltaTime;
				scoreText.setString("Apples eaten: " + std::to_string(numEatenApples));
				
			}
		}
		
		/*if (numEatenApples == NUM_APPLES)
		{
			window.close();
			break;
		}*/

		window.clear();
		playerShape.setPosition(playerX, playerY);
		for (int i = 0; i < NUM_APPLES; ++i)
		{
			window.draw(applesShape[i]);
		}
		window.draw(playerShape);
		window.draw(scoreText);
		window.display();
	}

	return 0;
}
