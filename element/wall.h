#ifndef WALL_H_INCLUDED
#define WALL_H_INCLUDED
#include "element.h"
#include "character.h"
#include "../scene/gamescene.h" // for element label
/*
   [wall object]
*/
typedef struct _Wall
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    int map_data[6][6];
} Wall;

Elements *New_Wall(int label);
void _Wall_load_map(Wall *wall);
void Wall_update(Elements *self);
void Wall_interact(Elements *self, Elements *tar);
void Wall_draw(Elements *self);
void Wall_destory(Elements *self);

#endif
