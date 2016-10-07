#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <string>

///This file include all NEED stuff
///like constants

const float PI = 3.14159265;

//Current Game Version
const std::string VERSION = "v1.0";

//Native resolution////
const float WIDTH  = 640.0;
const float HEIGHT = 400.0;

//FPS amount
const int FPS = 30;

//Starting chance to spawn enemy
const int CHANCE = 1;
//Max spawned enemies
const int MAX_ENEMIES = 10;
//Distance to collide with tower
const float DISTANCE = 50.0;

enum AnimType {NORMAL, BACKABLE, ONCE, REPEAT};
enum ParticleType {BLAST, BOLT, SMOKE};

#endif // CONFIG_H_INCLUDED
