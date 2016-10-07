#include "Game.h"

///Uncomment line under to enable DEBUG MODE
//#define DEBUG_DRAW

Game::Game()
{
    ///Get configuration settings from file if exist///
    ///////////////////////////////////////////////////
    std::fstream config;
    config.open("config.ini", std::ios::in);
    if(!config.good())
    {
        std::cout<<"Missing file!"<<std::endl;
        width = 640;
        height = 400;
        fullscreen = false;
    }
    else
    {
        std::string data;
        getline(config, data);
        width = std::stoi(data);
        getline(config, data);
        height = std::stoi(data);
        getline(config, data);
        fullscreen = std::stoi(data);
        config.close();
    }
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////

    fps = FPS;
    timeInSec = 0;
    frame = 0;
    pulseSide = true;
    init();
}

Game::~Game()
{
    cleanEnemies();
    cleanParticles();
    cleanGraves();
    delete tower;

    delete window;
}

void Game::init()
{
    srand(time(NULL));

    if(fullscreen)
        window = new sf::RenderWindow(sf::VideoMode(width, height), "Tower " + VERSION, sf::Style::Fullscreen);
    else
        window = new sf::RenderWindow(sf::VideoMode(width, height), "Tower " + VERSION);

    scale = sf::Vector2f(width/WIDTH, height/HEIGHT);

    loadTextures();
    loadSounds();

    //Create Tower and define rotate speed
    tower = new Tower(sf::Vector2f(WIDTH/2.0, HEIGHT/2.0), 0.05f);

    //Set starting STATE
    state = INTRO;

    //Load font
    font.loadFromFile("Data/font.ttf");
    text.setFont(font);
    text.setCharacterSize(20);
    text.setColor(sf::Color(94, 94, 94));

    //Set sprites
    background.setTexture(textures["background"].texture);
    platform.setTexture(textures["tower_platform"].texture);
    intro_logo.setTexture(textures["intro_logo"].texture);
    intro_options.setTexture(textures["intro_options"].texture);
    over_logo.setTexture(textures["over_logo"].texture);
    over_options.setTexture(textures["over_options"].texture);
    enemy_grave.setTexture(textures["enemy_grave"].texture);

    intro_logo.setOrigin(intro_logo.getGlobalBounds().width/2.0, intro_logo.getGlobalBounds().height/2.0);
    intro_logo.setPosition(WIDTH/2, HEIGHT/2);
    over_logo.setOrigin(over_logo.getGlobalBounds().width/2.0, over_logo.getGlobalBounds().height/2.0);
    over_logo.setPosition(WIDTH/2, HEIGHT/2);

    //Set sounds
    shot.setBuffer(sounds["shot"]);
    bolt.setBuffer(sounds["bolt"]);
    blast.setBuffer(sounds["blast"]);

    //SetTowerAnimations
    tower->anims.addAnim(new Animation(&textures["tower_idle"], 10, NORMAL));

    //Start game loop
    loop();
}

void Game::loop()
{
    sf::Clock clock;
    float deltaT;
    while(window->isOpen())
    {
        deltaT = clock.getElapsedTime().asSeconds();
        if(deltaT >= 1.0/fps)
        {
            input();
            update(deltaT);
            draw();

            clock.restart();
        }
    }
}

void Game::input()
{
    sf::Event evt;
    while(window->pollEvent(evt))
    {
        switch(state)
        {
        ///INTRO INPUT HANDLING/////////////////////////////
        ////////////////////////////////////////////////////
        case INTRO:
            if(evt.type == sf::Event::KeyReleased)
            {
                if(evt.key.code == sf::Keyboard::Space)
                    state = GAME;
                if(evt.key.code == sf::Keyboard::Escape)
                    window->close();
            }
        break;
        ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////

        ///GAME INPUT HANDLING//////////////////////////////
        ////////////////////////////////////////////////////
        case GAME:
            if(evt.type == sf::Event::KeyReleased)
            {
                if(evt.key.code == sf::Keyboard::Escape)
                   state = INTRO;
            }
            else if(evt.type == sf::Event::MouseButtonReleased)
            {
                if(evt.key.code == sf::Mouse::Left)
                    executeShoot();
            }
        break;
        ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////

        ///OVER INPUT HANDLING//////////////////////////////
        ////////////////////////////////////////////////////
        case OVER:
            if(evt.type == sf::Event::KeyReleased)
            {
                if(evt.key.code == sf::Keyboard::R)
                {
                    state = GAME;
                    setupGame();
                }
                if(evt.key.code == sf::Keyboard::Escape)
                    window->close();
            }
        break;
        ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////
        }
    }
}

void Game::update(float dt)
{
    switch(state)
        {
        ///INTRO UPDATE HANDLING/////////////////////////////
        ////////////////////////////////////////////////////
        case INTRO:
            {
                pulse(&intro_logo);
            }
        break;
        ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////

        ///GAME UPDATE HANDLING//////////////////////////////
        ////////////////////////////////////////////////////
        case GAME:
            {
                //Pass time
                timeInSec += dt;

                checkEnemiesBodies();
                gameOverCheck();
                spawnEnemy();

                tower->update(sf::Vector2i(sf::Mouse::getPosition(*window).x / scale.x, sf::Mouse::getPosition(*window).y / scale.y));
                for(int z=0; z<enemies.size(); z++)
                {
                    if(enemies[z] == nullptr) continue;
                    enemies[z]->update(tower->getCenterPoint());
                }

                updateParticles();
                for(int i=0; i<particles.size(); i++)
                {
                    if(particles[i] == nullptr) continue;
                    particles[i]->update();
                }
                if(frame%10 == 0)
                {
                    newParticle(tower->getPipePoint(), SMOKE);
                    frame = 0;
                }
                text.setString(std::to_string(round(timeInSec*10)/10));
                text.setPosition(width/2 - text.getGlobalBounds().width/2, height/20);
            }
        break;
        ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////

        ///OVER UPDATE HANDLING//////////////////////////////
        ////////////////////////////////////////////////////
        case OVER:
            {
                pulse(&over_logo);
            }
        break;
        ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////
        }

        frame++;
}

void Game::draw()
{
    window->clear(sf::Color::Black);

    draw_queue.push(&background);
    switch(state)
        {
        ///INTRO INPUT HANDLING/////////////////////////////
        ////////////////////////////////////////////////////
        case INTRO:
            {
                draw_queue.push(&intro_logo);
                draw_queue.push(&intro_options);
            }
        break;
        ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////

        ///GAME INPUT HANDLING//////////////////////////////
        ////////////////////////////////////////////////////
        case GAME:
            {
                //Draw graves
                for(int i=0; i<graves.size(); i++)
                {
                    draw_queue.push(graves[i]);
                }
                //Draw enemies
                for(int i=0; i<enemies.size(); i++)
                {
                    if(enemies[i] == nullptr) continue;
                    draw_queue.push(enemies[i]->getToDraw());
                }
                //Draw tower
                draw_queue.push(tower->getToDraw());
                //Draw particles
                for(int i=0; i<particles.size(); i++)
                {
                    if(particles[i] == nullptr) continue;
                    draw_queue.push(particles[i]->getToDraw());
                }

            #ifdef DEBUG_DRAW
            sf::VertexArray v(sf::Lines, 2);
            v[0] = sf::Vertex(sf::Vector2f(tower->getCenterPoint().x * scale.x, tower->getCenterPoint().y * scale.y), sf::Color::White);
            v[1] = sf::Vertex(sf::Vector2f(tower->getPointerPoint().x * scale.x, tower->getPointerPoint().y * scale.y), sf::Color::White);
            window->draw(v);
            for(int i=0; i<enemies.size(); i++)
            {
                if(enemies[i] != nullptr)
                {
                    v[0] = sf::Vertex(sf::Vector2f(enemies[i]->getLeftWing().x * scale.x, enemies[i]->getLeftWing().y * scale.y), sf::Color::White);
                    v[1] = sf::Vertex(sf::Vector2f(enemies[i]->getRightWing().x * scale.x, enemies[i]->getRightWing().y * scale.y), sf::Color::White);
                    window->draw(v);
                }
            }
            #endif // DEBUG_DRAW
            }
        break;
        ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////

        ///OVER INPUT HANDLING//////////////////////////////
        ////////////////////////////////////////////////////
        case OVER:
            {
                draw_queue.push(&over_logo);
                draw_queue.push(&over_options);
            }
        break;
        ////////////////////////////////////////////////////
        ////////////////////////////////////////////////////
        }

    ///All Release stuff is drawing here///
    #ifndef DEBUG_DRAW
    while(!draw_queue.empty())
    {
        sf::Sprite s = *draw_queue.front();
        s.setScale(s.getScale().x * scale.x, s.getScale().y * scale.y);
        s.setPosition(s.getPosition().x * scale.x, s.getPosition().y * scale.y);
        window->draw(s);
        draw_queue.pop();
    }
    window->draw(text);

    #endif // DEBUG_DRAW

    window->display();
}

sf::Vector2f Game::getRandomSpawn()
{
    int x = rand()%4;

    if(x == 0)
        return sf::Vector2f(rand()%int(WIDTH), 0);
    else if(x == 1)
        return sf::Vector2f(rand()%int(WIDTH), HEIGHT);
    else if(x == 2)
        return sf::Vector2f(0, rand()%int(HEIGHT)+1);
    else
        return sf::Vector2f(WIDTH-1, rand()%int(HEIGHT)+1);
}

void Game::executeShoot()
{
    shot.play();
    newParticle(tower->getPointerPoint(), BLAST);
    for(int i=0; i<enemies.size(); i++)
    {
        if(enemies[i] != nullptr)
        {
            //Check if lines isn't parallel
            if(enemies[i]->getAFactor() == tower->getAFactor()) continue;

            float x = (enemies[i]->getBFactor() - tower->getBFactor()) / (tower->getAFactor() - enemies[i]->getAFactor());
            float y = tower->getAFactor()*x + tower->getBFactor();
            sf::Vector2f hitPoint = sf::Vector2f(x, y);

            //Check if you hit properly
            if((enemies[i]->getLeftWing().x < hitPoint.x && hitPoint.x < enemies[i]->getRightWing().x) || (enemies[i]->getLeftWing().x > hitPoint.x && hitPoint.x > enemies[i]->getRightWing().x))
            {
                if((hitPoint.x >= tower->getPointerPoint().x && tower->getPointerPoint().x >= tower->getCenterPoint().x) ||
                   (hitPoint.y >= tower->getPointerPoint().y && tower->getPointerPoint().y >= tower->getCenterPoint().y) ||
                   (hitPoint.x <= tower->getPointerPoint().x && tower->getPointerPoint().x <= tower->getCenterPoint().x) ||
                   (hitPoint.y <= tower->getPointerPoint().y && tower->getPointerPoint().y <= tower->getCenterPoint().y))
                {
                    newParticleCombo(hitPoint, 5);
                    newParticleCombo(sf::Vector2f(hitPoint.x + rand()%80 - 40, hitPoint.y), 5);
                    newParticleCombo(sf::Vector2f(hitPoint.x, hitPoint.y + rand()%80 - 40), 5);
                    newParticleCombo(sf::Vector2f(hitPoint.x + rand()%80 - 40, hitPoint.y), 5);
                    newParticleCombo(sf::Vector2f(hitPoint.x, hitPoint.y + rand()%80 - 40), 5);
                    bolt.play();
                    blast.play();

                    enemies[i]->anims.currentAnim = 1;
                    return;
                }
            }
        }
    }
}

void Game::spawnEnemy()
{
    int x = 0;
    for(int i=0; i<enemies.size(); i++)
    {
        if(enemies[i] != nullptr)
            x++;
    }

    if(x < MAX_ENEMIES)
    {
        if(rand()%100 < round(CHANCE+round(timeInSec)/10.0))
        {
            std::cout<<enemies.size()<<std::endl;
            for(int i=0; i<enemies.size(); i++)
            {
                if(enemies[i] == nullptr)
                {
                    Enemy* tmp = new Enemy(getRandomSpawn());
                    enemies[i] = tmp;
                    //std::cout<<"GOOSE: "<<enemies[i]->getAFactor()<<std::endl;
                    enemies[i]->anims.addAnim(new Animation(&textures["enemy_idle"], 10, BACKABLE));
                    enemies[i]->anims.addAnim(new Animation(&textures["enemy_destroyed"], 2, NORMAL));
                    return;
                }
            }
            enemies.push_back(new Enemy(getRandomSpawn()));
            enemies[enemies.size()-1]->anims.addAnim(new Animation(&textures["enemy_idle"], 10, BACKABLE));
            enemies[enemies.size()-1]->anims.addAnim(new Animation(&textures["enemy_destroyed"], 2, NORMAL));
        }
    }
    return;
}

void Game::gameOverCheck()
{
    for(int i=0; i<enemies.size(); i++)
    {
        if(enemies[i] != nullptr)
        {
            if(sqrt(pow(enemies[i]->getCenterPoint().x - tower->getCenterPoint().x, 2) + pow(enemies[i]->getCenterPoint().y - tower->getCenterPoint().y, 2)) <= DISTANCE)
            {
                state = OVER;
                break;
            }
        }
    }
}

void Game::cleanEnemies()
{
    for(std::vector<Enemy*>::iterator i=enemies.end(); i<enemies.begin(); i--)
    {
        if(*i != nullptr)
        {
            delete (*i);
            enemies.pop_back();
        }
    }
}

void Game::cleanParticles()
{
    for(std::vector<Particle*>::iterator i=particles.end(); i<particles.begin(); i--)
    {
        if(*i != nullptr)
        {
           delete (*i);
           particles.pop_back();
        }
    }
}

void Game::cleanGraves()
{
    for(int i=graves.size()-1; i>=0; i--)
    {
        delete graves[i];
        graves.pop_back();
    }
}

void Game::updateParticles()
{
    for(int i=0; i<particles.size(); i++)
    {
        if(particles[i] == nullptr) continue;
        if(particles[i]->anim->isFinished())
        {
            delete particles[i];
            particles[i] = nullptr;
        }
    }
}

void Game::newParticle(sf::Vector2f pos, ParticleType pT)
{
    int x = rand()%2;
    std::string name;
    if(pT == BLAST)
    {
        if(x==0) name = "particles_blast1";
        else name = "particles_blast2";
    }
    else if(pT == BOLT)
    {
        if(x==0) name = "particles_bolt1";
        else name = "particles_bolt2";
    }
    else if(pT == SMOKE)
    {
        if(x==0) name = "particles_smoke1";
        else name = "particles_smoke2";
    }

   for(int i=0; i<particles.size(); i++)
    {
        if(particles[i] == nullptr)
        {
            particles[i] = new Particle(pos, new Animation(&textures[name], 4, NORMAL));
            return;
        }
    }
    particles.push_back(new Particle(pos, new Animation(&textures[name], 4, NORMAL)));
}

void Game::newParticleCombo(sf::Vector2f pos, int amount)
{
    for(int i=0; i<amount; i++)
    {
        int x = rand()%3;
        if(x == 0)
            newParticle(pos, BLAST);
        else if(x == 1)
            newParticle(pos, BOLT);
        else
            newParticle(pos, SMOKE);
    }
}

void Game::setupGame()
{
    timeInSec = 0;
    cleanEnemies();
    cleanGraves();
    enemies.clear();
}

void Game::checkEnemiesBodies()
{
    for(int i=0; i<enemies.size(); i++)
    {
        if(enemies[i] == nullptr) continue;
        if(enemies[i]->anims.currentAnim == 1)
        {
            if(enemies[i]->anims.animations[1]->isFinished())
            {
                graves.push_back(new sf::Sprite());
                graves[graves.size()-1]->setTexture(textures["enemy_grave"].texture, false);
                graves[graves.size()-1]->setPosition(enemies[i]->getCenterPoint());
                delete enemies[i];
                enemies[i] = nullptr;
            }
        }
    }
}

void Game::pulse(sf::Sprite* s)
{
    if(s->getScale().x >= 1.2 || s->getScale().x <= 0.9)
        pulseSide = !pulseSide;

    if(pulseSide)
        s->setScale(s->getScale().x + 0.01, s->getScale().y + 0.01);
    if(!pulseSide)
        s->setScale(s->getScale().x - 0.01, s->getScale().y - 0.01);
}

void Game::loadTextures()
{
    Texture texTmp;
    sf::Texture tex;

    std::string PATH = "Data/textures/";

    tex.loadFromFile(PATH + "background.png");
    texTmp.texture = tex;
    texTmp.cells = 1;
    texTmp.cellDimensions = sf::Vector2f(640, 400);
    textures["background"] = texTmp;


    PATH = "Data/textures/tower/";

    tex.loadFromFile(PATH + "idle.png");
    texTmp.texture = tex;
    texTmp.cells = 5;
    texTmp.cellDimensions = sf::Vector2f(74, 126);
    textures["tower_idle"] = texTmp;

    tex.loadFromFile(PATH + "platform.png");
    texTmp.texture = tex;
    texTmp.cells = 1;
    texTmp.cellDimensions = sf::Vector2f(78, 84);
    textures["tower_platform"] = texTmp;


    PATH = "Data/textures/intro/";

    tex.loadFromFile(PATH + "logo.png");
    texTmp.texture = tex;
    texTmp.cells = 1;
    texTmp.cellDimensions = sf::Vector2f(640, 400);
    textures["intro_logo"] = texTmp;

    tex.loadFromFile(PATH + "options.png");
    texTmp.texture = tex;
    texTmp.cells = 1;
    texTmp.cellDimensions = sf::Vector2f(640, 400);
    textures["intro_options"] = texTmp;


    PATH = "Data/textures/enemy/";

    tex.loadFromFile(PATH + "idle.png");
    texTmp.texture = tex;
    texTmp.cells = 5;
    texTmp.cellDimensions = sf::Vector2f(39, 58);
    textures["enemy_idle"] = texTmp;

    tex.loadFromFile(PATH + "attack.png");
    texTmp.texture = tex;
    texTmp.cells = 3;
    texTmp.cellDimensions = sf::Vector2f(39, 58);
    textures["enemy_attack"] = texTmp;

    tex.loadFromFile(PATH + "destroyed.png");
    texTmp.texture = tex;
    texTmp.cells = 5;
    texTmp.cellDimensions = sf::Vector2f(39, 58);
    textures["enemy_destroyed"] = texTmp;

    tex.loadFromFile(PATH + "grave.png");
    texTmp.texture = tex;
    texTmp.cells = 1;
    texTmp.cellDimensions = sf::Vector2f(58, 58);
    textures["enemy_grave"] = texTmp;


    PATH = "Data/textures/over/";

    tex.loadFromFile(PATH + "logo.png");
    texTmp.texture = tex;
    texTmp.cells = 1;
    texTmp.cellDimensions = sf::Vector2f(640, 400);
    textures["over_logo"] = texTmp;

    tex.loadFromFile(PATH + "options.png");
    texTmp.texture = tex;
    texTmp.cells = 1;
    texTmp.cellDimensions = sf::Vector2f(640, 400);
    textures["over_options"] = texTmp;


    PATH = "Data/textures/particle/";

    tex.loadFromFile(PATH + "blast1.png");
    texTmp.texture = tex;
    texTmp.cells = 5;
    texTmp.cellDimensions = sf::Vector2f(21, 20);
    textures["particles_blast1"] = texTmp;

    tex.loadFromFile(PATH + "blast2.png");
    texTmp.texture = tex;
    texTmp.cells = 5;
    texTmp.cellDimensions = sf::Vector2f(44, 32);
    textures["particles_blast2"] = texTmp;

    tex.loadFromFile(PATH + "bolt1.png");
    texTmp.texture = tex;
    texTmp.cells = 5;
    texTmp.cellDimensions = sf::Vector2f(14, 22);
    textures["particles_bolt1"] = texTmp;

    tex.loadFromFile(PATH + "bolt2.png");
    texTmp.texture = tex;
    texTmp.cells = 5;
    texTmp.cellDimensions = sf::Vector2f(43, 36);
    textures["particles_bolt2"] = texTmp;

    tex.loadFromFile(PATH + "smoke1.png");
    texTmp.texture = tex;
    texTmp.cells = 5;
    texTmp.cellDimensions = sf::Vector2f(30, 30);
    textures["particles_smoke1"] = texTmp;

    tex.loadFromFile(PATH + "smoke2.png");
    texTmp.texture = tex;
    texTmp.cells = 6;
    texTmp.cellDimensions = sf::Vector2f(42, 33);
    textures["particles_smoke2"] = texTmp;

    tex.loadFromFile(PATH + "husk.png");
    texTmp.texture = tex;
    texTmp.cells = 1;
    texTmp.cellDimensions = sf::Vector2f(5, 13);
    textures["particles_husk"] = texTmp;
}

void Game::loadSounds()
{
    sf::SoundBuffer tmp;

    std::string PATH = "Data/sounds/";

    tmp.loadFromFile(PATH + "shot.wav");
    sounds["shot"] = tmp;

    tmp.loadFromFile(PATH + "bolt.wav");
    sounds["bolt"] = tmp;

    tmp.loadFromFile(PATH + "blast.wav");
    sounds["blast"] = tmp;
}
