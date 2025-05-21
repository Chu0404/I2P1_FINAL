#include "wall.h"
#include <stdio.h>
/*
   [wall function]
*/
Elements *New_Wall(int label)
{
    Wall *pDerivedObj = (Wall *)malloc(sizeof(Wall));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/wall.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    _Wall_load_map(pDerivedObj);
    pDerivedObj->x = 0;
    pDerivedObj->y = 0;
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Character_L;
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Draw = Wall_draw;
    pObj->Update = Wall_update;
    pObj->Interact = Wall_interact;
    pObj->Destroy = Wall_destory;
    return pObj;
}
void _Wall_load_map(Wall *wall)
{
    FILE *data;
    data = fopen("assets/map/gamescene_map.txt", "r");
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            fscanf(data, "%d", &wall->map_data[i][j]);
        }
    }
    fclose(data);
}
void Wall_update(Elements *ele) {}
void Wall_interact(Elements *self, Elements *tar)
{
    if (tar->label == Character_L)
    {
        Character *chara = (Character *)(tar->pDerivedObj);
        int right_limit = WIDTH - chara->width / 2;
        int left_limit = 0 - chara->width / 2;
        if (chara->x < left_limit)
        {
            _Character_update_position(tar, (left_limit - chara->x), 0);
        }
        else if (chara->x > right_limit)
        {
            _Character_update_position(tar, (right_limit - chara->x), 0);
        }
    }
}
void Wall_draw(Elements *self)
{
}
void Wall_destory(Elements *self)
{
    Wall *Obj = ((Wall *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj);
    free(self);
}
