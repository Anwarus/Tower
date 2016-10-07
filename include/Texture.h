#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

struct Texture
{
    sf::Texture texture;
    sf::Vector2f cellDimensions;
    int cells;
};

#endif // TEXTURE_H_INCLUDED
