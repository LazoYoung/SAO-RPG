#ifndef GAME_STRUCT_INCLUDED
#define GAME_STRUCT_INCLUDED

#include "vector.h"

typedef struct {
    bool map[9][9];
    int color;
} Texture;

typedef struct { // Axis Aligned Bounding Box
    Vector centre;
    Vector radius;
} AABB;

typedef struct {
    Vector last_pos;
    Vector last_spd;
    Vector pos;
    Vector spd;
    bool onGround;
    bool wasOnGround;
} Location;

typedef struct { // Present bias of entity movement
    bool left; // Going left
    bool right; // Going right
    bool up; // Toggling jump or fly
    int times; // How many times does the present control carry on
} Bias;

typedef struct entity {
    bool valid;
    const char* name;
    Location loc;
    Bias bias;
    Texture skin;
    AABB hitbox;
    Vector offset;
    struct entity* next;
} Entity;

typedef struct {
    char agility; // 1 ~ 100
    char strength; // 1 ~ 100
    char health; // 0 ~ 100
    char mp; // 1 ~ 100
    unsigned int level;
    unsigned long exp; // Level-up cost: 200 + (50 * level)
    const char* name; // Character name
} PlayerProperty;

typedef struct {
    const char* name;
} Item;

typedef struct {
    Item* item;
    long long skills; // Achieved skills are recorded as codes in bit-manner.
    int coin;
} Inventory;

#endif