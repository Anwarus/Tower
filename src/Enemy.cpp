#include "Enemy.h"

Enemy::Enemy(sf::Vector2f position)
{
    centerPoint = position;

    setWings();

    a = -1.0/((HEIGHT/2.0 - centerPoint.y) / (WIDTH/2.0 - centerPoint.x));
    b = centerPoint.y - (a * centerPoint.x);
}

void Enemy::update(sf::Vector2f destination)
{
    float offsetx = (centerPoint.x - destination.x)/100.0;
    float offsety = (centerPoint.y - destination.y)/100.0;
    centerPoint.x -= offsetx;
    centerPoint.y -= offsety;
    setWings();
    b = centerPoint.y - (a * centerPoint.x);

    anims.update();
}

void Enemy::setWings()
{
    //It defines length between origin of enemy to end
    float x = 18.0;

    if(centerPoint.x == WIDTH/2.0)
    {
        leftWing = sf::Vector2f(centerPoint.x - x, centerPoint.y);
        rightWing = sf::Vector2f(centerPoint.x + x, centerPoint.y);
    }
    else if (centerPoint.y == HEIGHT/2.0)
    {
        leftWing = sf::Vector2f(centerPoint.x, centerPoint.y - x);
        rightWing = sf::Vector2f(centerPoint.x, centerPoint.y + x);
    }
    else
    {
        //A factor of line crossing center point and position
        float a1 = (HEIGHT/2.0 - centerPoint.y) / (WIDTH/2.0 - centerPoint.x);

        //A factor of perpendicural line
        float a2 = -1.0/a1;
        //B factor of perpendicural line
        float b2 = centerPoint.y - (a2 * centerPoint.x);

        //y = a*x + b
        //centerPoint.y = a2*centerPoint.x + b2

        float delta = round(sqrt(round(pow((-2.0*centerPoint.x - 2.0*centerPoint.y*a2 + 2.0*b2*a2), 2) + (-4.0 * (1.0 + a2*a2) * (centerPoint.x*centerPoint.x + centerPoint.y*centerPoint.y - 2.0*centerPoint.y*b2 + b2*b2 - x*x)))));

        leftWing.x = (round(2.0*centerPoint.x + 2.0*centerPoint.y*a2 - 2.0*b2*a2) - delta)/(2.0*(1+a2*a2));
        leftWing.y = a2*leftWing.x + b2;

        rightWing.x = (round(2.0*centerPoint.x + 2.0*centerPoint.y*a2 - 2.0*b2*a2) + delta)/(2.0*(1+a2*a2));
        rightWing.y = a2*rightWing.x + b2;
    }
}

sf::Sprite* Enemy::getToDraw()
{
    sprite = anims.getToDraw();
    sprite->setPosition(centerPoint);
    if(centerPoint.y > HEIGHT/2.0)
        sprite->setRotation((atan2(leftWing.y - centerPoint.y, leftWing.x - centerPoint.x)+PI/2.0)*180.0/PI - 90);
    else
        sprite->setRotation((atan2(leftWing.y - centerPoint.y, leftWing.x - centerPoint.x)+PI/2.0)*180.0/PI + 90);
    return sprite;
}
