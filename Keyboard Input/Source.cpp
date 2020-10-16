// red variables should be declared in the App class in App.h
// and, in most cases, initialized in the Init() method in App.cpp

#include <SFML/Graphics.hpp>

int main()
{
    // start the clock
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
    float length = 100;
    float width = 30;
    sf::RectangleShape paddle(sf::Vector2f(length, width));
    paddle.setFillColor(sf::Color::Green);
    paddle.setPosition(window.getSize().x / 2, window.getSize().y * 0.9);
    float speed = 350;

    while (window.isOpen())
    {
        // compute deltaTime, the time between 
        // start of consecutive frames
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

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
            if (paddle.getPosition().x <= window.getSize().x - length)
            {
                paddle.move(speed * deltaTime, 0);
            }
            
        }

        window.clear();
        window.draw(paddle);
        window.display();
    }

    return 0;
}

// TASK: modify the code so that the paddle cannot go off the screen
