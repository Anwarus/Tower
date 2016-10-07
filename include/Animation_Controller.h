#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <SFML/Graphics.hpp>

#include <vector>

#include "Animation.h"

class Animation_Controller
{
    public:
        Animation_Controller();
        ~Animation_Controller();

        std::vector<Animation*> animations;
        int currentAnim;

        void addAnim(Animation* animation);
        void update();
        sf::Sprite* getToDraw();

    protected:

    private:
};

#endif // ANIMATION_CONTROLLER_H
