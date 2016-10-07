#include "Animation_Controller.h"

Animation_Controller::Animation_Controller()
{
    currentAnim = 0;
}

Animation_Controller::~Animation_Controller()
{
    while(!animations.empty())
    {
        delete animations.back();
        animations.pop_back();
    }
    animations.clear();
}

void Animation_Controller::addAnim(Animation* animation)
{
    animations.push_back(animation);
}

void Animation_Controller::update()
{
    if(!animations.empty())
        animations[currentAnim]->updateAnim();
}

sf::Sprite* Animation_Controller::getToDraw()
{
    if(!animations.empty())
        return  animations[currentAnim]->getToDraw();
    return nullptr;
}
