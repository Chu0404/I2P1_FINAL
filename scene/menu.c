#include <allegro5/allegro_primitives.h>
#include "menu.h"
#include <stdbool.h>
/*
   [Menu function]
*/
typedef struct _Name{
    int len, max;
    char str[20];
}Name;
int naming = 0;
Name name;

Scene *New_Menu(int label)
{
    Menu *pDerivedObj = (Menu *)malloc(sizeof(Menu));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->font = al_load_ttf_font("assets/font/Minecraft.ttf", 32, 0);
    // Load sound
    pDerivedObj->song = al_load_sample("assets/sound/menu.mp3");
    pDerivedObj->bg[0] = al_load_bitmap("assets/image/menu.png");
    pDerivedObj->bg[1] = al_load_bitmap("assets/image/naming.png");
    pDerivedObj->title[0] = al_load_bitmap("assets/image/press_start_0.png");
    pDerivedObj->title[1] = al_load_bitmap("assets/image/press_start_1.png");
    
    name.len = 0, name.max = 16, name.str[0] = '\0';
    
    al_reserve_samples(20);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);

    // Loop the song until the display closes
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1);
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = menu_update;
    pObj->Draw = menu_draw;
    pObj->Destroy = menu_destroy;
    return pObj;
}
int enter = 0;
double last_time = 0;
void menu_update(Scene *self)
{
    if(enter == 0)       enter = key_state[ALLEGRO_KEY_ENTER];
    else if(enter == -1 && !key_state[ALLEGRO_KEY_ENTER]) enter = 0;
    if (enter == 1){
        enter = -1;
        if(naming == 0) naming = 1;
        else self->scene_end = window = 1;
    }
    if(naming && (al_get_time() - last_time > 0.1)){
        if (key_state[ALLEGRO_KEY_BACKSPACE] && name.len > 0) {
            last_time = al_get_time();
            name.len--;
            name.str[name.len] = '\0';
        }
    }
    if(naming && (al_get_time() - last_time > 0.3 || now_key != last_key)){
        if(now_key >= ALLEGRO_KEY_A && now_key <= ALLEGRO_KEY_Z && key_state[now_key]){
            last_key = now_key;
            last_time = al_get_time();
            if (name.len < name.max) {
                name.str[name.len++] = (char)(now_key - ALLEGRO_KEY_A + 'A');
                name.str[name.len] = '\0';
            }
        }
    }
    return;
}
void menu_draw(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    if(!naming){
        al_draw_bitmap(Obj->bg[0], 0, 0, 0);
        al_draw_bitmap(Obj->title[(int)(al_get_time()*2) % 2], 326, 340, 0);
    }else{
        al_draw_bitmap(Obj->bg[1], 0, 0, 0);
        al_draw_text(Obj->font, al_map_rgb(0, 0, 0), 480, 347, ALLEGRO_ALIGN_CENTRE, name.str);
    }
    al_play_sample_instance(Obj->sample_instance);
}
void menu_destroy(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    free(Obj);
    free(self);
}