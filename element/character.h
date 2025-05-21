#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "element.h"
#include "../shapes/Shape.h"
#include <stdbool.h>

typedef struct _Character
{
    int x, y;
    int width, height;
    int dir_x, dir_y; // 方向：水平和垂直
    int anim_frame;   // 目前使用第幾張動畫圖（0、1、2）
    int anim_state_counter; // 控制動畫切換的狀態
    double last_anim_time; // 上次動畫更新時間
    ALLEGRO_BITMAP *walk_left[3];
    ALLEGRO_BITMAP *walk_right[3];
    ALLEGRO_BITMAP *walk_up[3];
    ALLEGRO_BITMAP *walk_down[3];
    Shape *hitbox;
} Character;

Elements *New_Character(int label);
void Character_update(Elements *self);
void Character_draw(Elements *self);
void Character_destory(Elements *self);
void Character_interact(Elements *self, Elements *tar);
void _Character_update_position(Elements *self, int dx, int dy);
int wallCheck(Character *chara);

#endif
