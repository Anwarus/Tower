#include "Animation.h"

Animation::Animation(Texture* t, int d, AnimType animType)
{
    tex = t;
    currentCell = 0;
    duration = d;
    frame = 0;
    sprite.setTexture(tex->texture);
    this->animType = animType;

    side = true;
    finished = false;
}

void Animation::updateAnim()
{
    if(frame++ == duration)
    {
        frame = 0;
        if(animType == NORMAL)
        {
            if((currentCell+1) == tex->cells)
            {
                finished = true;
                currentCell = 0;
            }
            else
                currentCell++;
        }
        else if(animType == BACKABLE)
        {
            if((currentCell+1) == tex->cells)
                side = false;
            else if (currentCell-1 == 0)
                side = true;

            if(side) currentCell++;
            else currentCell--;
        }
    }
    else
        frame++;
}

sf::Sprite* Animation::getToDraw()
{
    sprite.setTextureRect(sf::IntRect(currentCell * tex->cellDimensions.x, 0, tex->cellDimensions.x, tex->cellDimensions.y));
    sprite.setOrigin((sprite.getTexture()->getSize().x/tex->cells)/2, sprite.getTexture()->getSize().y/2);
    return &sprite;
}

