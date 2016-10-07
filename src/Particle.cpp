#include "Particle.h"

Particle::Particle(sf::Vector2f position, Animation* animation)
{
    pos = position;
    anim = animation;
}

sf::Sprite* Particle::getToDraw()
{
    anim->getToDraw()->setPosition(pos);
    return anim->getToDraw();
}

void Particle::update()
{
    anim->updateAnim();
}
