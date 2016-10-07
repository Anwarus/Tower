#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include <math.h>
#include <iostream>

#include "CONFIG.h"
#include "Animation_Controller.h"

class Enemy
{
    public:
        Enemy(sf::Vector2f position);
        ~Enemy(){sprite = nullptr;};

        sf::Vector2f getCenterPoint(){return centerPoint;};
        sf::Vector2f getLeftWing(){return leftWing;};
        sf::Vector2f getRightWing(){return rightWing;};
        float getAFactor(){return a;};
        float getBFactor(){return b;};

        void update(sf::Vector2f destination);
        sf::Sprite* getToDraw();

        Animation_Controller anims;
        sf::Sprite* sprite;

    protected:

    private:
        sf::Vector2f centerPoint;
        sf::Vector2f leftWing;
        sf::Vector2f rightWing;
        float angle;

        //Factors of line /game physics
        float a;
        float b;

        void setWings();
};

#endif // ENEMY_H
