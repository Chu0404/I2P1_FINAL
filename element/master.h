#ifndef Master_H_INCLUDED
#define Master_H_INCLUDED
#define MAX_DIALOG_LINES 5  // 最多幾句話（可調）

#include "element.h"
#include "../shapes/Shape.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../ui/dialog.h"


typedef struct _Master {
    int x, y;
    int width, height;
    ALLEGRO_BITMAP *img;
    Shape *hitbox;
    DialogBox dialog;
    const char* dialog_lines[MAX_DIALOG_LINES];  // 所有對話內容
    int dialog_index;                            // 現在講到第幾句
    bool talking;                                // 是否正在對話中
    int has_spoken;
} Master;

Elements *New_Master(int label);
void Master_draw(Elements *self);
void Master_update(Elements *self);
void Master_interact(Elements *self, Elements *tar);
void Master_destroy(Elements *self);
void Master_advance_dialog(struct _Master *master);

#endif