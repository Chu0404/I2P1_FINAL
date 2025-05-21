#include "gamescene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
/*
   [GameScene function]
*/
int out_map[150][100];
int gym_map[30][20];
int hospital_map[30][20];
int tree_map[30][20];
int house_map[30][20];

RoomType cur_room;;
int cx = 0, cy = 0, chx = 0, chy = 0;

Scene *New_GameScene(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->outdoor = al_load_bitmap("assets/map/out_map.png");
    pDerivedObj->gym = al_load_bitmap("assets/map/gym_map.png");
    pDerivedObj->hospital = al_load_bitmap("assets/map/hospital_map.png");
    pDerivedObj->house = al_load_bitmap("assets/map/house_map.png");
    pDerivedObj->tree = al_load_bitmap("assets/map/tree_map.png");
    
    pDerivedObj->loading = algif_new_gif("assets/gif/loading.gif", -1);   
    pDerivedObj->load_frame = 0;
    pDerivedObj->room_type = cur_room = Load_L;
    pDerivedObj->next_room = Outdoor_L;
    
    // Initialize camera variables
    pDerivedObj->camera_x = 0;
    pDerivedObj->camera_y = 0;
    pDerivedObj->camera_zoom = 1.0;
    pObj->pDerivedObj = pDerivedObj;
    pDerivedObj->font = al_load_ttf_font("assets/font/Cubic_11.ttf", 24, 0);
    // register element
    _Register_elements(pObj, New_Character(Character_L));
    _Register_elements(pObj, New_Master(Master_L));
    _Register_elements(pObj, New_Nurse(Nurse_L));
    _Register_elements(pObj, New_Seller(Seller_L));
    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    read_map();
    return pObj;
}
void game_scene_update(Scene *self)
{
    // update every element
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_FONT *font = gs->font;
    if(gs->room_type == Outdoor_L){   
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++) {
            Elements *ele = allEle.arr[i];
            if (ele->label == Character_L || ele->label == Master_L) ele->Update(ele);
            if (ele->label == Master_L) {
                Master *master = (Master *)ele->pDerivedObj;
                Master_advance_dialog(master);
            }
        }
        // remove element
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if (ele->dele)
                _Remove_elements(self, ele);
        }
        int nxt = check_enter();
        if(nxt != -1){
            gs->room_type = Load_L;
            gs->next_room = nxt;
        }

    }else if(gs->room_type == Gym1_L || gs->room_type == Gym2_L || gs->room_type == Gym3_L){
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++){
            Elements *ele = allEle.arr[i];
            if (ele->label == Character_L) ele->Update(ele);
        }
        // remove element
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if (ele->dele)
                _Remove_elements(self, ele);
        }
        if(check_enter() == Outdoor_L){
            gs->room_type = Load_L;
            gs->next_room = Outdoor_L;
        }
    }else if(gs->room_type == Hospital1_L || gs->room_type == Hospital2_L || gs->room_type == Hospital3_L){
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++){
            Elements *ele = allEle.arr[i];
            if (ele->label == Character_L) ele->Update(ele);
        }
        // remove element
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if (ele->dele)
                _Remove_elements(self, ele);
        }
        if(check_enter() == Outdoor_L){
            gs->room_type = Load_L;
            gs->next_room = Outdoor_L;
        }
    }else if(gs->room_type == House1_L || gs->room_type == House2_L || gs->room_type == House3_L){
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++){
            Elements *ele = allEle.arr[i];
            if (ele->label == Character_L) ele->Update(ele);
        }
        // remove element
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if (ele->dele)
                _Remove_elements(self, ele);
        }
        if(check_enter() == Outdoor_L){
            gs->room_type = Load_L;
            gs->next_room = Outdoor_L;
        }
    }else if(gs->room_type == Tree1_L || gs->room_type == Tree2_L){
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++){
            Elements *ele = allEle.arr[i];
            if (ele->label == Character_L) ele->Update(ele);
        }
        // remove element
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if (ele->dele)
                _Remove_elements(self, ele);
        }
        if(check_enter() == Outdoor_L){
            gs->room_type = Load_L;
            gs->next_room = Outdoor_L;
        }
    }else if(gs->room_type == Load_L){
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++){
            if(allEle.arr[i]->label == Character_L){
                Character *chara = (Character *)(allEle.arr[i]->pDerivedObj);
                chara->x = chx;
                chara->y = chy;
            }
        }
        gs->load_frame++;
        if(gs->load_frame > 220){
            gs->room_type = cur_room = gs->next_room;
            gs->next_room = -1;
            gs->load_frame = 0;
        }
    }
}
void game_scene_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_FONT *font = gs->font;
    if(gs->room_type == Outdoor_L){   
        // Create and set up camera transform
        ALLEGRO_TRANSFORM camera;
        al_identity_transform(&camera);
        
        // Calculate camera position to center on character
        float camera_x = chx - WIDTH/2;
        float camera_y = chy - HEIGHT/2;
        
        // Clamp camera to map boundaries
        int map_width = al_get_bitmap_width(gs->outdoor);
        int map_height = al_get_bitmap_height(gs->outdoor);
        
        if (camera_x < 0) camera_x = 0;
        if (camera_y < 0) camera_y = 0;
        if (camera_x > map_width - WIDTH) camera_x = map_width - WIDTH;
        if (camera_y > map_height - HEIGHT) camera_y = map_height - HEIGHT;
        
        // Apply camera translation
        al_translate_transform(&camera, -camera_x, -camera_y);
        
        // Use the camera transform
        al_use_transform(&camera);
        
        // Draw background at origin
        al_draw_bitmap(gs->outdoor, 0, 0, 0);
        
        // Draw all elements
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if(ele->label == Character_L || ele->label == Master_L) ele->Draw(ele);
        }
                    for (int i = 0; i < allEle.len; i++) {
        Elements *ele = allEle.arr[i];
        if (ele->label == Master_L) {
            Master *master = (Master *)ele->pDerivedObj;

            // 👇 把對話框畫在角色頭上
            float scale = 2;
            master->dialog.x = master->x + (master->width * scale / 2) - (master->dialog.width / 2);
            master->dialog.y = master->y - master->dialog.height - 10;

            dialog_draw(&master->dialog, font);
        }
    }
        // Reset transform to default
        al_identity_transform(&camera);
        al_use_transform(&camera);
        cx = camera_x, cy = camera_y;
    }else if(gs->room_type == Gym1_L || gs->room_type == Gym2_L || gs->room_type == Gym3_L){
        al_draw_bitmap(gs->gym, 0, 0, 0);
        
        // Draw all elementsm
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if(ele->label == Character_L) ele->Draw(ele);
        }

    }else if(gs->room_type == Hospital1_L || gs->room_type == Hospital2_L || gs->room_type == Hospital3_L){
        al_draw_bitmap(gs->hospital, 0, 0, 0);
        
        // Draw all elementsm
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if(ele->label == Character_L || ele->label == Nurse_L)ele->Draw(ele);
        }

    }else if(gs->room_type == House1_L || gs->room_type == House2_L || gs->room_type == House3_L){
        al_draw_bitmap(gs->house, 0, 0, 0);
        
        // Draw all elementsm
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if(ele->label == Character_L)ele->Draw(ele);
        }

    }else if(gs->room_type == Tree1_L || gs->room_type == Tree2_L){
        al_draw_bitmap(gs->tree, 0, 0, 0);
        
        // Draw all elementsm
        ElementVec allEle = _Get_all_elements(self);
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            if(ele->label == Character_L)ele->Draw(ele);
        }

    }else if(gs->room_type == Load_L){
        ALLEGRO_BITMAP *frame = algif_get_bitmap(gs->loading, al_get_time() * 0.5);
        if(gs->load_frame < 200) al_draw_scaled_bitmap(frame, 0, 0, 282, 300, 0, 0, WIDTH, HEIGHT, 0);
    }
    
}
void game_scene_destroy(Scene *self)
{
    GameScene *Obj = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_BITMAP *outdoor = Obj->outdoor;
    al_destroy_bitmap(outdoor);
    ALLEGRO_BITMAP *gym = Obj->gym;
    al_destroy_bitmap(gym);

    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(self);
}

void read_map()
{
    FILE *data;
    
    data = fopen("assets/map/out_map.csv", "r");
    // Read the file line by line
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 150; j++){
            if(j != 99)fscanf(data, "%d,", &out_map[j][i]);
            else fscanf(data, "%d\n,", &out_map[j][i]);
        }
    }
    fclose(data);
    
    data = fopen("assets/map/gym_map.csv", "r");
    // Read the file line by line
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 30; j++){
            if(j != 19)fscanf(data, "%d,", &gym_map[j][i]);
            else fscanf(data, "%d\n,", &gym_map[j][i]);
        }
    }
    fclose(data);
        
    data = fopen("assets/map/hospital_map.csv", "r");
    // Read the file line by line
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 30; j++){
            if(j != 19)fscanf(data, "%d,", &hospital_map[j][i]);
            else fscanf(data, "%d\n,", &hospital_map[j][i]);
        }
    }
    fclose(data);
        
    data = fopen("assets/map/house_map.csv", "r");
    // Read the file line by line
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 30; j++){
            if(j != 19)fscanf(data, "%d,", &house_map[j][i]);
            else fscanf(data, "%d\n,", &house_map[j][i]);
        }
    }
    fclose(data);
        
    data = fopen("assets/map/tree_map.csv", "r");
    // Read the file line by line
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 30; j++){
            if(j != 19)fscanf(data, "%d,", &tree_map[j][i]);
            else fscanf(data, "%d\n,", &tree_map[j][i]);
        }
    }
    fclose(data);
}

RoomType check_enter(){
    if(cur_room == Outdoor_L){
        if((chx/32 == 31 || chx/32 == 32) && chy/32 == 7){
            chx = 400, chy = 400;
            return Gym1_L;
        }
        if((chx/32 == 56 || chx/32 == 57) && chy/32 == 40){
            chx = 400, chy = 400;
            return Gym2_L;
        }
        if((chx/32 == 78 || chx/32 == 79) && chy/32 == 68){
            chx = 400, chy = 400;
            return Gym3_L;
        }
        if((chx/32 >= 77 && chx/32 <= 79) && chy/32 == 7){
            chx = 8.75*32, chy = 15*32;
            return Hospital1_L;
        }
        if((chx/32 >= 7 && chx/32 <= 9) && chy/32 == 62){
            chx = 8.75*32, chy = 15*32;
            return Hospital2_L;
        }
        if((chx/32 >= 112 && chx/32 <= 114) && chy/32 == 72){
            chx = 8.75*32, chy = 15*32;
            return Hospital3_L;
        }
        if((chx/32 == 78 || chx/32 == 79) && chy/32 == 89){
            chx = 2*32, chy = 16*32;
            return House1_L;
        }
        if((chx/32 == 100 || chx/32 == 101) && chy/32 == 97){
            chx = 2*32, chy = 16*32;
            return House2_L;
        }
        if((chx/32 == 99 || chx/32 == 100) && chy/32 == 77){
            chx = 2*32, chy = 16*32;
            return House3_L;
        }
        if((chx/32 >= 100 && chx/32 <= 102) && chy/32 == 46){
            chx = 1*32, chy = 16*32;
            return Tree1_L;
        }
        if((chx/32 >= 58 && chx/32 <= 60) && chy/32 == 55){
            chx = 1*32, chy = 16*32;
            return Tree2_L;
        }
    }
    if(cur_room == Gym1_L){
        if ((chx/32 == 28 ||chx/32 == 29) && (chy/32 == 2 || chy/32 == 3)){
            chx = 32 * 32, chy = 9 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == Gym2_L){
        if ((chx/32 == 28 ||chx/32 == 29) && (chy/32 == 2 || chy/32 == 3)){
            chx = 57 * 32, chy = 42 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == Gym3_L){
        if ((chx/32 == 28 ||chx/32 == 29) && (chy/32 == 2 || chy/32 == 3)){
            chx = 79 * 32, chy = 70 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == Hospital1_L){
        if ((chx/32 >= 7 && chx/32 <= 9) && (chy/32 == 17 || chy/32 == 18)){
            chx = 78 * 32, chy = 9 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == Hospital2_L){
        if ((chx/32 >= 7 && chx/32 <= 9) && (chy/32 == 17 || chy/32 == 18)){
            chx = 8 * 32, chy = 64 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == Hospital3_L){
        if ((chx/32 >= 7 && chx/32 <= 9) && (chy/32 == 17 || chy/32 == 18)){
            chx = 113 * 32, chy = 74 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == House1_L){
        if ((chx/32 >= 0 && chx/32 <= 2) && (chy/32 == 17 || chy/32 == 18)){
            chx = 79 * 32, chy = 91 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == House2_L){
        if ((chx/32 >= 0 && chx/32 <= 2) && (chy/32 == 17 || chy/32 == 18)){
            chx = 101 * 32, chy = 98 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == House3_L){
        if ((chx/32 >= 0 && chx/32 <= 2) && (chy/32 == 17 || chy/32 == 18)){
            chx = 100 * 32, chy = 79 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == Tree1_L){
        if ((chx/32 >= 1 && chx/32 <= 3) && (chy/32 == 18 || chy/32 == 19)){
            chx = 101 * 32, chy = 48 * 32;
            return Outdoor_L;
        }
    }
    if(cur_room == Tree2_L){
        if ((chx/32 >= 1 && chx/32 <= 3) && (chy/32 == 18 || chy/32 == 19)){
            chx = 59 * 32, chy = 57 * 32;
            return Outdoor_L;
        }
    }
    return -1;
}