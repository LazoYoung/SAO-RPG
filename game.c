// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncursesw/curses.h>
#include "game.h"
#include "level.h"

PlayerProperty p_attr = {
    .agility = 1,
    .strength = 1,
    .health = 100,
    .mp = 100,
    .level = 1,
    .exp = 0,
    .name = "UNDEFINED"
};
Inventory inv = {
    .item = NULL, // TODO dynamic allocation may be required for this array
    .skills = 0,
    .coin = 0
};
static Entity player;
bool inGame = false;
const float deltaTime = 50 / 1000.0;
const int fps = 1000 / 50;

void startGame() {
    AABB hitbox = {{0.0, 0.0}, {1.0, 1.0}};
    Bias bias = {false, false, false, 0};
    Location loc = getSpawnLocation(5);
    bool map[9][9] = {false};
    Texture skin;

    map[4][0] = map[5][0] = true;
    skin.color = COLOR_CYAN;
    memcpy(skin.map, map, sizeof(map));
    player.valid = true;
    player.name = p_attr.name;
    player.loc = loc;
    player.hitbox = hitbox;
    player.offset[0] = 0.0;
    player.offset[1] = 0.0;
    player.skin = skin;
    player.bias = bias;
    inGame = true;

    spawnEntity(&player);
}

/* Assigns a given skill into the player's inventory */
void assignSkill(char skill_code) {
    inv.skills |= (1 << skill_code);
}

/* Determines whether or not a given skill is achieved by the player */
bool hasSkill(char skill_code) {
    long long filter = 1 << skill_code;
    return (inv.skills & filter) == filter;
}

bool doTick(int key) {
    if (!inGame)
        return true;
    
    if (player.valid) {
        updateControl(key, &player.bias);
    }

    updateEntities();
    return true;
}

/* Returns the rounded count of frames being made during the given time-frame (ms) */
int getFramesDuringTime(int miliseconds) {
    return round(fps * miliseconds / 1000);
}