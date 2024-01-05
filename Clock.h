#pragma once
#include <math.h>
#include <time.h>
#include "SFML/Graphics.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

class Clock
{
private:
    sf::RenderWindow _clockWindow;

    sf::CircleShape _clockCircle;
    sf::CircleShape _innerCircle;

    sf::RectangleShape _secondTickShape;
    sf::RectangleShape _hourTickShape;
    sf::RectangleShape _secondHandShape;
    sf::RectangleShape _minuteHandShape;
    sf::RectangleShape _hourHandShape;
    sf::RectangleShape _dayShape;

    sf::Text _theText;
    sf::Text _dayText;
    sf::Font _theFont;
    sf::Texture _texture;

    sf::View _view;
    sf::Event _event;
    unsigned int _width, _height;
    float _minMax;
    float _centerX, _centerY;
    time_t _currentTime;
    struct tm *_localTime;

    void init();
    void initResources();
    void update();
    void run();
    void draw();

    float getX(float angle);
    float getY(float angle);

    // time stuff
    void getTime();
    int getSecond();
    int getMinute();
    int getHour();
    int getDay();

    float min_value(float first, float second);
    void initRect(sf::RectangleShape &rect, float x, float y, float xz, float yz, sf::Color mycolor);

public:
    Clock(float width, float height);
    ~Clock();
};
