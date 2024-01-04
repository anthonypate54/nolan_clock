#include <math.h>
#include "SFML/Graphics.hpp"
#include "Clock.h"
#include <iostream>

#define TEST 1

int main(int argc, char *argv[])
{
    if (TEST)
    {
        Clock clock(300, 300);
        return 1;
    }
    else
    { // used to test various things
        sf::RenderWindow window(sf::VideoMode(640, 480),
                                "Rendering the rectangle.");

        // Creating our shape.
        sf::RectangleShape rectangle(sf::Vector2f(128.0f, 128.0f));
        rectangle.setFillColor(sf::Color::White);
        rectangle.setPosition(320, 240);
        rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);

        sf::Font font;
        sf::Text text;
        if (!font.loadFromFile("Arial Unicode.ttf"))
        {
            std::cout << "not found";
            exit(1);
        }
        text.setFont(font);
        text.setString("12");
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Red);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.width / 2, textRect.height / 2);
        text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    // Close window button clicked.
                    window.close();
                }
            }
            window.clear(sf::Color::Black);
            window.draw(rectangle); // Drawing our shape.
            text.setString("23");
            window.draw(text);
            window.display();
        }
    }
}
