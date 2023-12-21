#define _USE_MATH_DEFINES
#include "Clock.h"
#include <cmath>
#include <iostream>

using namespace std;

Clock::Clock(float width, float height)
{
    _width = width;
    _height = height;

    _clockWindow.create(sf::VideoMode(_width, _height), "Nolan's Clock");
    _clockWindow.setFramerateLimit(60);

    if (!_theFont.loadFromFile("res/Arial Unicode.ttf"))
    {
        cout << "not found";
        exit(0);
    }

    if (!_texture.loadFromFile("res/nolan.jpg"))
    {
        cout << "nolan.jpg not found";
        exit(0);
    }

    init();
    run();
}

void Clock::init()
{
    _view = _clockWindow.getDefaultView();
    _clockCircle.setPointCount(600);

    _centerX = _width / 2.f;
    _centerY = _height / 2.f;

    sf::Vector2f temp;
    float circleRadius;

    _minMax = _width <= _height ? _width : _height;

    circleRadius = _minMax / 2;

    _clockCircle.setRadius(_minMax / 2);
    _clockCircle.setOrigin(_clockCircle.getRadius(), _clockCircle.getRadius());
    _clockCircle.setPosition(_centerX, _centerY);
    _clockCircle.setFillColor(sf::Color::White);

    _clockCircle.setTexture(&_texture);

    _innerCircle.setRadius(4.0f);
    _innerCircle.setOrigin(_innerCircle.getRadius(), _innerCircle.getRadius());
    _innerCircle.setPosition(_centerX, _centerY);
    _innerCircle.setFillColor(sf::Color::Black);

    temp = sf::Vector2f(_minMax / 160, _minMax / 100);
    _secondTickShape.setSize(temp);
    _secondTickShape.setPosition(_centerX, 0);
    _secondTickShape.setOrigin(_secondTickShape.getSize().x / 2, 0);
    _secondTickShape.setFillColor(sf::Color::White);

    temp = sf::Vector2f(_minMax / 160, _minMax / 30);
    _hourTickShape.setSize(temp);
    _hourTickShape.setPosition(_centerX, 0);
    _hourTickShape.setOrigin(_hourTickShape.getSize().x / 2.f, 0);
    _hourTickShape.setFillColor(sf::Color::White);

    _secondHandShape.setSize(sf::Vector2f(2, circleRadius - 5));
    initRect(_secondHandShape, _centerX, _centerY, 1, circleRadius - 5, sf::Color::Black);

    _minuteHandShape.setSize(sf::Vector2f(2, circleRadius - 15));
    initRect(_minuteHandShape, _centerX, _centerY, 2, circleRadius - 15, sf::Color::Black);

    _hourHandShape.setSize(sf::Vector2f(2, circleRadius - 60));
    initRect(_hourHandShape, _centerX, _centerY, 2, circleRadius - 60, sf::Color::Black);

    _dayShape.setSize(sf::Vector2f(20, 15));
    initRect(_dayShape, _centerX + circleRadius - 60, _centerY - 7, 0, 0, sf::Color(197, 240, 251));
}

void Clock::update()
{
}

void Clock::run()
{
    while (_clockWindow.isOpen())
    {
        while (_clockWindow.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                _clockWindow.close();
            else if (_event.type == sf::Event::Resized)
            {
                // resize my view
                sf::FloatRect visibleArea(0, 0, _event.size.width, _event.size.height);
                if (visibleArea.width < 300 || visibleArea.height < 300)
                {
                    visibleArea.width = 300;
                    visibleArea.height = 300;
                    sf::Vector2u newSize(300, 300);
                    _clockWindow.setSize(newSize);
                }
                _clockWindow.setView(sf::View(visibleArea));

                _width = _clockWindow.getSize().x;
                _height = _clockWindow.getSize().y;

                init();
            }
        }

        update();
        _clockWindow.clear(sf::Color(197, 240, 251));
        draw();
        _clockWindow.display();
    }
}

void Clock::draw()
{
    _clockWindow.draw(_clockCircle);
    _clockWindow.draw(_innerCircle);
    _clockWindow.draw(_dayShape);

    float angle = 0.f;
    _width = _clockWindow.getSize().x;
    _height = _clockWindow.getSize().y;

    for (int i = 0; i < 60; i++)
    {
        angle = i * (360.f / 60.f); // 360 degrees / 60 is radians
        _secondTickShape.setRotation(angle);
        _secondTickShape.setPosition(getX(angle), getY(angle));

        if (i % 5 == 0)
        { // for the hour tick needs to be slightly longer and
            angle = i * (360.f / 12.f);
            _hourTickShape.setRotation(angle);
            _hourTickShape.setPosition(getX(angle), getY(angle));
            _clockWindow.draw(_hourTickShape);
        }
        _clockWindow.draw(_secondTickShape);
    }

    // now draw the hour numbers
    // could have done this in the above loop
    // maybe refactor
    sf::Text myText;
    myText.setFont(_theFont);
    myText.setFillColor(sf::Color::White);
    myText.setStyle(sf::Text::Bold);
    myText.setCharacterSize(16);
    myText.setOutlineColor(sf::Color::Black);

    angle = 0.f;
    float clockRadius = min_value(_width, _height * 0.8 * 0.5);
    int num = 12;
    for (int i = 0; i < 12; i++)
    {
        float x = _centerX + (clockRadius + 5) * cos(-30 * i * (M_PI / 180) + M_PI / 2);
        float y = _centerY - (clockRadius + 5) * sin(-30 * i * (M_PI / 180) + M_PI / 2);

        myText.setString(std::to_string(num));

        const sf::FloatRect bounds(myText.getLocalBounds());

        x = x - bounds.width / 2;
        y = y - bounds.height / 2 - (bounds.left + bounds.top / 2);
        myText.setPosition(x, y);
        _clockWindow.draw(myText);

        num++;
        if (num > 12)
            num = 1;
    }
    _secondHandShape.setRotation(6 * getSecond());
    _minuteHandShape.setRotation(6 * getMinute() + getSecond() * 0.1);
    _hourHandShape.setRotation(30 * getHour() + getMinute() * 0.5);

    const sf::FloatRect dayBounds = myText.getLocalBounds();

    myText.setString(std::to_string(getDay()));

    myText.setPosition(_centerX + (_minMax / 2) - 60, _centerY - dayBounds.height / 2 - (dayBounds.left + dayBounds.top / 2));
    myText.setFillColor(sf::Color::Black);
    myText.setStyle(sf::Text::Bold);
    myText.setCharacterSize(16);

    _clockWindow.draw(myText);
    _clockWindow.draw(_secondHandShape);
    _clockWindow.draw(_minuteHandShape);
    _clockWindow.draw(_hourHandShape);
}

float Clock::min_value(float first, float second)
{
    return first > second ? second : first;
}
float Clock::getX(float angle)
{
    return (_width / 2.f) - ((_minMax / 2.f) * cos((270 - angle) * (M_PI / 180)));
}

float Clock::getY(float angle)
{
    return (_height / 2.f) + ((_minMax / 2.f) * sin((270 - angle) * (M_PI / 180)));
}

void Clock::initRect(sf::RectangleShape &rect, float x, float y, float xz, float yz, sf::Color mycolor)
{
    rect.setFillColor(mycolor);
    rect.setOrigin(xz, yz);
    rect.setPosition(x, y);
};

void Clock::getTime()
{
    //  time_t currentTime;
    //  struct tm *localTime;
    time(&_currentTime);
    _localTime = localtime(&_currentTime);
}

int Clock::getSecond()
{
    getTime();
    return _localTime->tm_sec;
}
int Clock::getMinute()
{
    getTime();
    return _localTime->tm_min;
}
int Clock::getHour()
{
    getTime();
    return _localTime->tm_hour;
}
int Clock::getDay()
{
    getTime();
    return _localTime->tm_mday;
}