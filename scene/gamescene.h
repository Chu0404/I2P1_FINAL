#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include "../algif5/src/algif.h"
#include "../element/element.h"
#include "../element/character.h"
#include "../element/nurse.h"
#include "../element/seller.h"
#include "../element/master.h"
#include "../element/wall.h"
/*
   [game scene object]
*/
typedef enum EleType
{
    Character_L,
    Nurse_L,
    Seller_L,
    Wall_L,
    Master_L
} EleType;
typedef enum RoomType
{
    Outdoor_L = 0,
    Gym1_L,
    Gym2_L,
    Gym3_L,
    Hospital1_L,
    Hospital2_L,
    Hospital3_L,
    House1_L,
    House2_L,
    House3_L,
    Tree1_L,
    Tree2_L,
    Store_L,
    Cave_L,
    Load_L
} RoomType;
typedef struct _GameScene
{
    ALLEGRO_BITMAP *outdoor;
    ALLEGRO_BITMAP *gym;
    ALLEGRO_BITMAP *hospital;
    ALLEGRO_BITMAP *tree;
    ALLEGRO_BITMAP *house;
    ALLEGRO_FONT *font;
    float camera_x;
    float camera_y;
    float camera_zoom;
    RoomType room_type;
    RoomType next_room;
    ALGIF_ANIMATION *loading;
    int load_frame;
} GameScene;
Scene *New_GameScene(int label);
void game_scene_update(Scene *self);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);
void read_map();
RoomType check_enter();

extern int out_map[150][100];
extern int gym_map[30][20];
extern int hospital_map[30][20];
extern int tree_map[30][20];
extern int house_map[30][20];

extern int cx, cy;
extern int chx, chy;
extern RoomType cur_room;
#endif
