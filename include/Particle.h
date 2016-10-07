#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

#include "Animation.h"

class Particle
{
    public:
        Particle(sf::Vector2f position, Animation* animation);
        ~Particle(){delete anim;};

        sf::Sprite* getToDraw();
        void update();

        sf::Vector2f pos;
        Animation* anim;

    protected:

    private:
};

#endif // PARTICLE_H
