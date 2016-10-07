#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <ctime>
#include <queue>

#include "CONFIG.h"
#include "Tower.h"
#include "Enemy.h"
#include "Animation.h"
#include "Texture.h"
#include "Particle.h"

///This enum control Game States///
enum State {                    ///
    INTRO,                      ///
    GAME,                       ///
    OVER                        ///
};                              ///
/// ///////////////////////////////

class Game
{
    public:
        Game();
        ~Game();

        void init();
        void loop();

    protected:

    private:
        int width;
        int height;
        int fps;
        bool fullscreen;
        sf::RenderWindow* window;
        //Game scale depends of chosen resolution
        sf::Vector2f scale;
        int frame;
        float timeInSec;

        //Game Textures
        std::map<std::string, Texture> textures;
        //Game Sounds
        std::map<std::string, sf::SoundBuffer> sounds;

        State state;

        void loadTextures();
        void loadSounds();

        void input();
        void update(float dt);
        void draw();

        //Generate random spawn for new enemy
        sf::Vector2f getRandomSpawn();

        //On shoot check collisions;
        void executeShoot();

        //Spawn new enemies depends on game time
        void spawnEnemy();

        //Check if game over
        void gameOverCheck();

        //Set up new game
        void setupGame();

        //Clean up enemies
        void cleanEnemies();

        //Clean up particles
        void cleanParticles();

        //Clean up graves
        void cleanGraves();

        Tower* tower;
        std::vector<Enemy*> enemies;

        //Sprites to draw in queue
        std::queue<sf::Sprite*> draw_queue;

        //Enemy graves
        std::vector<sf::Sprite*> graves;

        //Check if enemy died already
        void checkEnemiesBodies();

        //Particles
        std::vector<Particle*> particles;
        void updateParticles();
        void newParticle(sf::Vector2f pos, ParticleType pT);
        void newParticleCombo(sf::Vector2f pos, int amount);

        //Special effect making sprite pulsing
        void pulse(sf::Sprite* s);
        bool pulseSide;

        //Static sprites
        sf::Sprite background;
        sf::Sprite platform;
        sf::Sprite intro_logo;
        sf::Sprite intro_options;
        sf::Sprite over_logo;
        sf::Sprite over_options;
        sf::Sprite enemy_grave;

        //Sounds
        sf::Sound shot;
        sf::Sound bolt;
        sf::Sound blast;

        //Time survived
        sf::Font font;
        sf::Text text;
};

#endif // GAME_H
