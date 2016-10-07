#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>

#include <math.h>
#include <iostream>
#include <map>

#include "CONFIG.h"
#include "Animation_Controller.h"

class Tower
{
    public:
        Tower(sf::Vector2f position, float speed);

        sf::Vector2f getPointerPoint() {return pointerPoint;};
        sf::Vector2f getCenterPoint() {return centerPoint;};
        sf::Vector2f getPipePoint() {return pipePoint;};
        float getAFactor(){return a;};
        float getBFactor(){return b;};

        void update(sf::Vector2i pos);
        sf::Sprite* getToDraw();

        Animation_Controller anims;

    protected:

    private:
        sf::Sprite* sprite;

        float rotationSpeed;
        float barrelAngle;

        sf::Vector2f centerPoint;
        sf::Vector2f pointerPoint;
        sf::Vector2f pipePoint;

        //Factors of line /game physics
        float a;
        float b;
};

#endif // TOWER_H
