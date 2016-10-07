#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

#include "CONFIG.h"
#include "Texture.h"

class Animation
{
    public:
        Animation(Texture* t, int d, AnimType animType);
        ~Animation(){tex = nullptr;};
        void updateAnim();
        sf::Sprite* getToDraw();
        //You can use this function to check if animation finished first cycle
        bool isFinished(){return finished;};

    protected:

    private:
        sf::Sprite sprite;
        Texture* tex;
        int currentCell;
        int duration;
        int frame;

        AnimType animType;
        //Enabled only when animation is back-able
        bool side;
        //Enabled only when animation is non-repeating
        bool finished;
};

#endif // ANIMATION_H
