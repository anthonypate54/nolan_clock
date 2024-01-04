#define _USE_MATH_DEFINES
#include "Clock.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <filesystem>
#include <string>
#include <mach-o/dyld.h>

using namespace std;

bool testCase = false;
Clock::Clock(float width, float height)
{
    _width = width;
    _height = height;

    _clockWindow.create(sf::VideoMode(_width, _height), "Nolan's Clock");
    _clockWindow.setFramerateLimit(60);

    initResources();
    init();
    run();
}

void Clock::initResources()
{
    unsigned int bufferSize = 1024;

    char buffer[bufferSize];
    // char *buffer;
    //  buffer = (char *)malloc(bufferSize);

    if (_NSGetExecutablePath(&buffer[0], &bufferSize) == 0)
    {
        cout << "executable path is " << buffer << endl;
    }
    else
    {
        cout << "could not find path " << buffer << endl;
    }

    char pathbuf[1024];
    strcpy(pathbuf, buffer);

    // free(buffer);

    std::string fontPath;
    std::string path = pathbuf;
    size_t found = path.rfind("/");
    path.replace(found, path.length(), "/");

    fontPath = path + "Arial Unicode.ttf";
    //   cout << fontPath + "\n";
    bool exist = std::filesystem::exists(fontPath);
    if (!exist)
    {
        cout << "cant open font file " << fontPath << "\n";
        exit(1);
    }
    else
    {
        // cout << "file exists";
    }
    std::string imagePath = path + "nolan.jpg";
    //   cout << imagePath + "\n";

    if (!_texture.loadFromFile(imagePath))
    {
        cout << "nolan.jpg not found";
        exit(1);
    }

    if (!_theFont.loadFromFile(fontPath))
    {
        cout << "not found";
        exit(1);
    }

    _theText.setFont(_theFont);
    _theText.setFillColor(sf::Color::White);
    _theText.setStyle(sf::Text::Bold);
    _theText.setCharacterSize(16);
    _theText.setOutlineColor(sf::Color::Black);

    _dayText.setFont(_theFont);
    _dayText.setFillColor(sf::Color::Black);
    _dayText.setStyle(sf::Text::Bold);
    _dayText.setCharacterSize(16);
}

Clock::~Clock()
{
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

    int textSize = 15 * (_width / 300);
    _theText.setCharacterSize(textSize);
    _dayText.setCharacterSize(textSize);
    _dayShape.setSize(sf::Vector2f(circleRadius * 0.14666f, circleRadius * 0.1333f));
    initRect(_dayShape, _centerX + (circleRadius * 0.6f), _centerY - 7, 0, 0, sf::Color(197, 240, 251));
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
                testCase = true;
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

    angle = 0.f;

    // the amount to move the text away from the circle
    const float radiusOffset = 0.4166f;
    float radius = min_value(_width * radiusOffset, _height * radiusOffset);
    int num = 12;
    for (int i = 0; i < 12; i++)
    {
        float tmpX = _centerX + (radius)*cos(-30 * i * (M_PI / 180) + M_PI / 2);
        float tmpY = _centerY - (radius)*sin(-30 * i * (M_PI / 180) + M_PI / 2);
        _theText.setString(std::to_string(num));

        const sf::FloatRect bounds(_theText.getLocalBounds());
        _theText.setOrigin(bounds.width / 2, bounds.height / 2);
        _theText.setPosition(tmpX, tmpY);

        _clockWindow.draw(_theText);

        num++;
        if (num > 12)
            num = 1;
    }
    _secondHandShape.setRotation(6 * getSecond());
    _minuteHandShape.setRotation(6 * getMinute() + getSecond() * 0.1);
    _hourHandShape.setRotation(30 * getHour() + getMinute() * 0.5);

    _dayText.setString(std::to_string(getDay()));
    //   _dayText.setString("30");
    sf::FloatRect rectBounds = _dayShape.getGlobalBounds();
    sf::FloatRect textBounds = _dayText.getGlobalBounds();

    _dayText.setPosition(
        rectBounds.left + (rectBounds.width / 2) - (textBounds.width / 2),
        rectBounds.top + (rectBounds.height / 2) - (textBounds.height) + 2);

    _clockWindow.draw(_dayText);
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