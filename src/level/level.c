// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
#include <string.h>
#include "header/level.h"
#include "header/screen.h"
#include "header/physic.h"

int level_width = 0, level_height = 0, portal_i = -1;
static Stage stage = VOID;
static Portal portal_arr[30];
static Tile **tiles = NULL;
static Entity *entity[MAX_ENTITY] = {NULL};

/**
 * Spawn an entity.
 * Player must be assigned before any other entities.
 **/
void spawnEntity(Entity* e) {
    Bias bias = {false, true, 0, 0, 0};

    for (int i = 0; i < MAX_ENTITY; i++) {
        Entity *s = entity[i];

        if (s) continue;
        
        e->bias = bias;
        entity[i] = e;
        break;
    }
}

bool despawnEntity(const char* name) {
    for (int i=0; i<MAX_ENTITY; i++) {
        Entity *s = entity[i];

        if (s && strcmp(name, s->name) == 0) {
            entity[i] = NULL;
            return true;
        }
    }
    return false;
}

/**
 * Finds an entity matching the NAME
 * Returns NULL if the function has failed to query
 **/
Entity* getEntity(const char* name) {
    for (int i = 0; i < MAX_ENTITY; i++) {
        Entity *s = entity[i];

        if (s && strcmp(name, s->name) == 0) {
            return s;
        }
    }
    return NULL;
}

/**
 * Returns the pointer of entity matching the index ID
 * The pointer is NULL if the entity is invalid.
 **/
Entity* getEntityByID(int id) {
    if (id >= 0 && id < MAX_ENTITY) {
        Entity *e = entity[id];

        if (e) return e;
    }
    return NULL;
}

/* Returns location at top of the blocks below */
Location getTopLocation(int pos_x) {
    Location loc;
    loc.pos[0] = (float) pos_x;
    loc.spd[0] = 0.0;
    loc.spd[1] = 0.0;
    
    if (tiles != NULL) {
        for (int y = level_height - 1; y > 0; y--) {
            if (tiles[y][pos_x] == BLOCK) {
                loc.pos[1] = y + 3;
                break;
            }
        }
    }

    return loc;
}

void assignPortal(Portal instance) {
    instance.valid = true;
    portal_arr[++portal_i] = instance;
}

/**
 * Returns a warp matching the given tile.
 * (NULL if not found)
 **/
Portal *getPortal(Tile tile) {
    for (int i=0; i<=portal_i; i++) {
        Portal iter = portal_arr[i];

        if (iter.valid && tile == iter.tile) {
            return &portal_arr[i];
        }
    }

    return NULL;
}

void destructLevel() {
    if (stage == DUNGEON)
        destructDungeon();

    if (tiles != NULL) {
        free(tiles);
        tiles = NULL;
    }

    if (portal_i > -1) {
        for (int i=0; i<10; i++) {
            Portal instance = {false};
            portal_arr[i] = instance;
        }
        portal_i = -1;
    }
}

/**
 * Generate the level with a new stage.
 **/
void generateLevel(Stage _stage) {
    if (stage != VOID) {
        destructLevel();
    }
    stage = _stage;
    level_width = 200;
    level_height = 50;

    tiles = calloc(level_height, sizeof(Tile *));

    for (int r=0; r<level_height; r++) {
        tiles[r] = (Tile *) calloc(level_width, sizeof(Tile));
    }

    for (int y=0; y<level_height; y++) {
        for (int x=0; x<level_width; x++) {
            if (y > 5) {
                tiles[y][x] = AIR;
            }
            else {
                tiles[y][x] = BLOCK;
            }
        }
    }

    switch (stage) {
        case LOBBY:
            generateLobby(tiles);
            break;
        case DUNGEON:
            generateDungeon(tiles);
            break;
    }
}

Stage getStage() {
    return stage;
}

char* getStageName(Stage _stage) {
    switch (_stage) {
        case VOID:
            return "Void";
        case LOBBY:
            return "Lobby";
        case SHOP:
            return "Shop";
        case DUNGEON:
            return "Dungeon";
    }

    return NULL;
}

char* getDungeonName(DungeonType type) {
    switch (type) {
        case DUNGEON_1:
            return "Dungeon 1";
        case DUNGEON_2:
            return "Dungeon 2";
        case DUNGEON_3:
            return "Dungeon 3";
    }

    return NULL;
}

Tile getTileAt(int x, int y) {
    if (tiles == NULL || x >= level_width || x < 0 || y >= level_height || y < 0)
        return AIR;

    return tiles[y][x];
}

void setTileAt(int x, int y, Tile tile) {
    if (tiles == NULL || x >= level_width || x < 0 || y >= level_height || y < 0)
        return;
    
    tiles[y][x] = tile;
}