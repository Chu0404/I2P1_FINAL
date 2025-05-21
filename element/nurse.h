#ifndef NURSE_H_INCLUDED
#define NURSE_H_INCLUDED

#include "element.h"
#include "../shapes/Shape.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

typedef struct _Nurse {
    int x, y;
    int width, height;
    ALLEGRO_BITMAP *img;
    Shape *hitbox;
} Nurse;

Elements *New_Nurse(int label);
void Nurse_draw(Elements *self);
void Nurse_update(Elements *self);
void Nurse_interact(Elements *self, Elements *tar);
void Nurse_destroy(Elements *self);

#endif
