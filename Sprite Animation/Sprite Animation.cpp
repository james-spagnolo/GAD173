#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;


int main()
{
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Hello SFML World!");

    sf::Texture texture;
    if (!texture.loadFromFile("Bird Sprite Sheet.png"))
    {
        std::cout << "Unable to load image" << std::endl;
    }

    const int ROWS = 8;
    const int COLS = 12;

    float spriteWidth = texture.getSize().x / COLS;
    float spriteHeight = texture.getSize().y / ROWS;

    float duration = 1.0f;

    bool cycle = true;

    sf::IntRect rect(0, 0, spriteWidth, spriteHeight);
    sf::Sprite sprite(texture, rect);
    sprite.scale(20, 20);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        int spriteIndex = 2;

        

        if (clock.getElapsedTime().asSeconds() > duration) {


            //Moving UP
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

                rect.top = 3 * spriteHeight;
            }

            //Moving Left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {

                rect.top = spriteHeight;
            }

            //Moving Down
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

                rect.top = 0;
            }

            //Moving Right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

                rect.top = 2 * spriteHeight;
            }

            if (cycle) {

                rect.left += spriteWidth;

                if (rect.left >= spriteIndex * spriteWidth) {

                    cycle = false;
                }
            }
            else {

                rect.left -= spriteWidth;

                if (rect.left <= 0) {

                    cycle = true;
                }
            }
            

            sprite.setTextureRect(rect);
            clock.restart();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
    return EXIT_SUCCESS;
}

// TASK: play only the first three sprites 
// in the proper order: 1, 2, 3, 2, 1, …

// TASK: move the sprite with user input (WASD)
// and play the appropriate animations

// TASK: transcribe into framework



