#ifndef SELLER_H_INCLUDED
#define SELLER_H_INCLUDED

#include "element.h"
#include "../shapes/Shape.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

typedef struct _Seller {
    int x, y;
    int width, height;
    ALLEGRO_BITMAP *img;
    Shape *hitbox;
} Seller;

Elements *New_Seller(int label);
void Seller_draw(Elements *self);
void Seller_update(Elements *self);
void Seller_interact(Elements *self, Elements *tar);
void Seller_destroy(Elements *self);

#endif
