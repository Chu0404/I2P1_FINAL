#include "nurse.h"
#include "../shapes/Rectangle.h"
#include <stdlib.h>
#include <unistd.h>     // for getcwd (Windows: io.h)

Elements *New_Nurse(int label) {
    Nurse *nurse = malloc(sizeof(Nurse));
    if (!nurse) {
        exit(1);
    }

    nurse->img = al_load_bitmap("assets/image/nurse3.png");

    nurse->x = 32*18;
    nurse->y = 32*16.5;
    nurse->width = nurse->img ? al_get_bitmap_width(nurse->img) : 32;
    nurse->height = nurse->img ? al_get_bitmap_height(nurse->img) : 32;
    nurse->hitbox = New_Rectangle(nurse->x, nurse->y,
                                    nurse->x + nurse->width,
                                    nurse->y + nurse->height);

    Elements *ele = New_Elements(label);
    ele->pDerivedObj = nurse;
    ele->Draw = Nurse_draw;
    ele->Update = Nurse_update;
    ele->Interact = Nurse_interact;
    ele->Destroy = Nurse_destroy;

    return ele;
}

void Nurse_draw(Elements *self) {
    Nurse *nurse = (Nurse *)self->pDerivedObj;

    if (nurse->img) {
        float scale = 0.6;  // 調整這個值來縮放大小，例如 0.5 是縮小一半
        al_draw_scaled_bitmap(
            nurse->img,
            0, 0,
            al_get_bitmap_width(nurse->img),
            al_get_bitmap_height(nurse->img),
            nurse->x,
            nurse->y,
            48,
            48,
            0
        );
    }
}
void Nurse_update(Elements *self) {
    // NPC 不需要移動
}

void Nurse_interact(Elements *self, Elements *tar) {
    // 之後可以加互動
}

void Nurse_destroy(Elements *self) {
    Nurse *nurse = (Nurse *)self->pDerivedObj;

    if (nurse->img)
        al_destroy_bitmap(nurse->img);
    if (nurse->hitbox)
        free(nurse->hitbox);

    free(nurse);
    free(self);
}