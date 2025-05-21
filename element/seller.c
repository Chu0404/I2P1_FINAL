#include "seller.h"
#include "../shapes/Rectangle.h"
#include <stdlib.h>
#include <unistd.h>     // for getcwd (Windows: io.h)

Elements *New_Seller(int label) {
    Seller *seller = malloc(sizeof(Seller));
    if (!seller) {
        exit(1);
    }

    seller->img = al_load_bitmap("assets/image/seller2.png");

    seller->x = 100;
    seller->y = 100;
    seller->width = seller->img ? al_get_bitmap_width(seller->img) : 32;
    seller->height = seller->img ? al_get_bitmap_height(seller->img) : 32;
    seller->hitbox = New_Rectangle(seller->x, seller->y,
                                   seller->x + seller->width,
                                   seller->y + seller->height);

    Elements *ele = New_Elements(label);
    ele->pDerivedObj = seller;
    ele->Draw = Seller_draw;
    ele->Update = Seller_update;
    ele->Interact = Seller_interact;
    ele->Destroy = Seller_destroy;

    return ele;
}

void Seller_draw(Elements *self) {
    Seller *seller = (Seller *)self->pDerivedObj;

    if (seller->img) {
        float scale = 0.6;  // 調整這個值來縮放大小，例如 0.5 是縮小一半
        al_draw_scaled_bitmap(
            seller->img,
            0, 0,
            al_get_bitmap_width(seller->img),
            al_get_bitmap_height(seller->img),
            seller->x,
            seller->y,
            48,
            48,
            0
        );
    }
}
void Seller_update(Elements *self) {
    // NPC 不需要移動
}

void Seller_interact(Elements *self, Elements *tar) {
    // 之後可以加互動
}

void Seller_destroy(Elements *self) {
    Seller *seller = (Seller *)self->pDerivedObj;

    if (seller->img)
        al_destroy_bitmap(seller->img);
    if (seller->hitbox)
        free(seller->hitbox);

    free(seller);
    free(self);
}