#include "Tower.h"

Tower::Tower(sf::Vector2f position, float speed)
{
    centerPoint = position;
    //Reason of minus 78 is MainGun length from tower origin(center point)
    pointerPoint = sf::Vector2f(position.x, position.y - 68);
    rotationSpeed = speed;

    a = 0;
    b = 0;
}

void Tower::update(sf::Vector2i pos)
{
    //Get barrel angle based on centerPoint in radians
    float barrelAngle = atan2(pointerPoint.y - centerPoint.y, pointerPoint.x - centerPoint.x) + PI;
    //Get mouse angle based on centerPoint in radians
    float mouseAngle = atan2(pos.y - centerPoint.y, pos.x - centerPoint.x) + PI;

    //If barrelAngle don't fill to tolerance area
    if(!(barrelAngle < mouseAngle + 0.05 && barrelAngle > mouseAngle - 0.05))
    {
        //Get point of direction to rotate barrel
        float section = fabs(mouseAngle - barrelAngle) * 1.0;
        float rest = fabs((2.0 * PI) - section);

        float angle;

        if(section < rest)
        {
            if(barrelAngle > mouseAngle)
                angle = rotationSpeed*-1;
            else
                angle = rotationSpeed;
        }
        else
        {
            if(barrelAngle > mouseAngle)
                angle = rotationSpeed;
            else
                angle = rotationSpeed*-1;
        }

        //Set new pointerPoint (point of barrel end)
        pointerPoint = sf::Vector2f(cos(angle) * (pointerPoint.x - centerPoint.x) - sin(angle) * (pointerPoint.y - centerPoint.y) + centerPoint.x,
                                    sin(angle) * (pointerPoint.x - centerPoint.x) + cos(angle) * (pointerPoint.y - centerPoint.y) + centerPoint.y);

        if(pointerPoint.x - centerPoint.x == 0)
        {
            a = 0;
            b = 0;
        }
        else
        {
            a = (pointerPoint.y - centerPoint.y) / (pointerPoint.x - centerPoint.x);
            b = centerPoint.y - (centerPoint.x*a);
        }
    }

    pipePoint = sf::Vector2f(centerPoint.x + (centerPoint.x - pointerPoint.x)/1.2, centerPoint.y + (centerPoint.y - pointerPoint.y)/1.2);

    anims.update();
}

sf::Sprite* Tower::getToDraw()
{
    sprite = anims.getToDraw();
    sprite->setPosition(centerPoint);
    sprite->setRotation((atan2(pointerPoint.y - centerPoint.y, pointerPoint.x - centerPoint.x)+PI/2.0)*180.0/PI);
    return sprite;
}
