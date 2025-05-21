#include "character.h"
#include "../scene/sceneManager.h"
#include "../scene/gamescene.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define SCALE 2 // 放大倍數

void _Character_update_position(Elements *self, int dx, int dy)
{
    Character *chara = (Character *)(self->pDerivedObj);
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

Elements *New_Character(int label)
{
    Character *pDerivedObj = (Character *)malloc(sizeof(Character));
    Elements *pObj = New_Elements(label);

    // Load character images
    pDerivedObj->walk_down[0] = al_load_bitmap("assets/image/front.png");
    pDerivedObj->walk_down[1] = al_load_bitmap("assets/image/front_left.png");
    pDerivedObj->walk_down[2] = al_load_bitmap("assets/image/front_right.png");
    
    
    pDerivedObj->walk_up[0] = al_load_bitmap("assets/image/back.png");
    pDerivedObj->walk_up[1] = al_load_bitmap("assets/image/back_left.png");
    pDerivedObj->walk_up[2] = al_load_bitmap("assets/image/back_right.png");

    pDerivedObj->walk_left[0] = al_load_bitmap("assets/image/left.png");
    pDerivedObj->walk_left[1] = al_load_bitmap("assets/image/left_left.png");
    pDerivedObj->walk_left[2] = al_load_bitmap("assets/image/left_right.png");

    pDerivedObj->walk_right[0] = al_load_bitmap("assets/image/right.png");
    pDerivedObj->walk_right[1] = al_load_bitmap("assets/image/right_left.png");
    pDerivedObj->walk_right[2] = al_load_bitmap("assets/image/right_right.png");

    // Set initial position and state
    pDerivedObj->x = 0;
    pDerivedObj->y = 0;
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->walk_right[0]);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->walk_right[0]);
    pDerivedObj->dir_x = 0;
    pDerivedObj->dir_y = 1;
    pDerivedObj->anim_frame = 0;
    pDerivedObj->last_anim_time = al_get_time();
    pDerivedObj->anim_state_counter = 0;

    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x, pDerivedObj->y, pDerivedObj->x + pDerivedObj->width * SCALE, pDerivedObj->y + pDerivedObj->height * SCALE);

    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Character_update;
    pObj->Draw = Character_draw;
    pObj->Interact = Character_interact;
    pObj->Destroy = Character_destory;
    return pObj;
}
int cd = 0;
void Character_update(Elements *self)
{
    Character *chara = (Character *)(self->pDerivedObj);

    bool moving = false;
    int ox = chara->x, oy = chara->y;

    if (mouse_state[2] && !cd) {
        int mx = mouse.x;
        int my = mouse.y;
        if(cur_room == Outdoor_L) chara->x = (int)((cx + mx) / 4) * 4, chara->y = (int)((cy + my) / 4) * 4;
        else chara->x = (int)(mx / 4) * 4, chara->y = (int)(my / 4) * 4;
    }

    cd = mouse_state[2];

    if(wallCheck(chara)){
        chara->x = ox, chara->y = oy;
    }
    

    if (key_state[ALLEGRO_KEY_A] || key_state[ALLEGRO_KEY_LEFT]) {
        _Character_update_position(self, -4, 0);
        chara->dir_x = -1, chara->dir_y = 0, moving = true;
    }
    else if (key_state[ALLEGRO_KEY_D] || key_state[ALLEGRO_KEY_RIGHT]) {
        _Character_update_position(self, 4, 0);
        chara->dir_x = 1, chara->dir_y = 0,  moving = true;
    }
    chara->x = chara->x < 0 ? 0 : chara->x;
    if(wallCheck(chara)){
        chara->x = ox;
    }
    if (key_state[ALLEGRO_KEY_W] || key_state[ALLEGRO_KEY_UP]) {
        _Character_update_position(self, 0, -4);
        chara->dir_y = -1, chara->dir_x = 0, moving = true;
    }
    else if (key_state[ALLEGRO_KEY_S] || key_state[ALLEGRO_KEY_DOWN]) {
        _Character_update_position(self, 0, 4);
        chara->dir_y = 1, chara->dir_x = 0, moving = true;
    }
    chara->y = chara->y < 0 ? 0 : chara->y;
    if(wallCheck(chara)){
        chara->y = oy;
    }

    if (!moving) {
        chara->anim_frame = 0;
        chara->anim_state_counter = 0;
    } else {
        double now = al_get_time();
        if (now - chara->last_anim_time > 0.15) {
            // Animation: 0 → 1 → 0 → 2 → 0 → ...
            switch (chara->anim_state_counter % 4) {
                case 0: chara->anim_frame = 0; break;
                case 1: chara->anim_frame = 1; break;
                case 2: chara->anim_frame = 0; break;
                case 3: chara->anim_frame = 2; break;
            }
            chara->anim_state_counter++;
            chara->last_anim_time = now;
        }
    }
    chx = chara->x, chy = chara->y;
}

void Character_draw(Elements *self)
{
    Character *chara = (Character *)(self->pDerivedObj);
    ALLEGRO_BITMAP *frame = NULL;

    if (chara->dir_y == -1)
        frame = chara->walk_up[chara->anim_frame];
    else if (chara->dir_y == 1)
        frame = chara->walk_down[chara->anim_frame];
    else if (chara->dir_x == -1)
        frame = chara->walk_left[chara->anim_frame];
    else if (chara->dir_x == 1)
        frame = chara->walk_right[chara->anim_frame];
    else
        frame = chara->walk_down[0];

    if (frame)
        al_draw_scaled_bitmap(
            frame,
            0, 0,
            chara->width, chara->height,
            chara->x, chara->y,
            48, 48,
            0
        );
    else
        printf("❌ 未載入角色圖片 frame = NULL\n");
}

void Character_destory(Elements *self)
{
    Character *chara = (Character *)(self->pDerivedObj);
    for (int i = 0; i < 3; i++) {
        al_destroy_bitmap(chara->walk_left[i]);
        al_destroy_bitmap(chara->walk_right[i]);
        al_destroy_bitmap(chara->walk_up[i]);
        al_destroy_bitmap(chara->walk_down[i]);
    }
    free(chara->hitbox);
    free(chara);
    free(self);
}

void Character_interact(Elements *self, Elements *tar){}

const int w = 32;
int wallCheck(Character *chara){
    if(cur_room == Outdoor_L){
        if(out_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+0.125)] != -1 ||
            out_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1.375)] != -1 ||
            out_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+0.125)] != -1 ||
            out_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1.375)] != -1 ||
            out_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1)] != -1 ||
            out_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1)] != -1 ||
            out_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+1.375)] != -1 ||
            out_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+0.125)] != -1){
            return 1;
        }
    }else if(cur_room == Gym1_L || cur_room == Gym2_L || cur_room == Gym3_L){
        if(gym_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+0.125)] != -1 ||
            gym_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1.375)] != -1 ||
            gym_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+0.125)] != -1 ||
            gym_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1.375)] != -1 ||
            gym_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1)] != -1 ||
            gym_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1)] != -1 ||
            gym_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+1.375)] != -1 ||
            gym_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+0.125)] != -1){
            return 1;
        }
    }else if(cur_room == Hospital1_L || cur_room == Hospital2_L || cur_room == Hospital3_L){
        if(hospital_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+0.125)] != -1 ||
            hospital_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1.375)] != -1 ||
            hospital_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+0.125)] != -1 ||
            hospital_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1.375)] != -1 ||
            hospital_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1)] != -1 ||
            hospital_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1)] != -1 ||
            hospital_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+1.375)] != -1 ||
            hospital_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+0.125)] != -1){
            return 1;
        }
    }else if(cur_room == House1_L || cur_room == House2_L || cur_room == House3_L){
        if(house_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+0.125)] != -1 ||
            house_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1.375)] != -1 ||
            house_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+0.125)] != -1 ||
            house_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1.375)] != -1 ||
            house_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1)] != -1 ||
            house_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1)] != -1 ||
            house_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+1.375)] != -1 ||
            house_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+0.125)] != -1){
            return 1;
        }
    }else if(cur_room == Tree1_L || cur_room == Tree2_L){
        if(tree_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+0.125)] != -1 ||
            tree_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1.375)] != -1 ||
            tree_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+0.125)] != -1 ||
            tree_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1.375)] != -1 ||
            tree_map[(int)((double)chara->x/w+1.375)][(int)((double)chara->y/w+1)] != -1 ||
            tree_map[(int)((double)chara->x/w+0.125)][(int)((double)chara->y/w+1)] != -1 ||
            tree_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+1.375)] != -1 ||
            tree_map[(int)((double)chara->x/w+1)][(int)((double)chara->y/w+0.125)] != -1){
            return 1;
        }
    }
    return 0;
}