#include "master.h"
#include "../shapes/Rectangle.h"
#include "../element/character.h"
#include "../scene/sceneManager.h"  //為了使用全域變數 scene
#include "../scene/gamescene.h" 
#include <stdlib.h>
#include <unistd.h>     // for getcwd (Windows: io.h)
#include <math.h>  // 為了 sqrt()

Elements *New_Master(int label) {
    Master *master = malloc(sizeof(Master));
    if (!master) {
        exit(1);
    }
    master->has_spoken = false;
    master->img = al_load_bitmap("assets/image/master.png");

    master->x = 13*32;
    master->y = 6*32;
    master->width = master->img ? al_get_bitmap_width(master->img) : 48;
    master->height = master->img ? al_get_bitmap_height(master->img) : 48;
    master->hitbox = New_Rectangle(master->x, master->y,
                                    master->x + master->width,
                                    master->y + master->height);

    master->dialog_lines[0] = "Hi!";
    master->dialog_lines[1] = "I'm your guide.";
    master->dialog_lines[2] = "Good luck!";
    master->dialog_lines[3] = "Don't die :)";
    master->dialog_lines[4] = "Bye~";

    master->dialog_index = -1;
    master->talking = false;

    // 設定 dialog 寬高與初始文字（x/y 會在畫面前動態設定）
    dialog_init(&master->dialog, master->dialog_lines[0], 13*32, 6*32, 300, 60);
    dialog_hide(&master->dialog);

    Elements *ele = New_Elements(label);
    ele->pDerivedObj = master;
    ele->Draw = Master_draw;
    ele->Update = Master_update;
    ele->Interact = Master_interact;
    ele->Destroy = Master_destroy;

    return ele;
}

void Master_draw(Elements *self) {
    Master *master = (Master *)self->pDerivedObj;

    if (master->img) {
        al_draw_scaled_bitmap(
            master->img,
            0, 0,
            master->width, master->height,
            master->x, master->y,
            48, 48,
            0
        );
    }
}
void Master_update(Elements *self) {
    Master *master = (Master *)self->pDerivedObj;

    ElementVec allEle = _Get_all_elements(scene);
    for (int i = 0; i < allEle.len; i++) {
        Elements *ele = allEle.arr[i];
        if (ele->label == Character_L) {
            Character *chara = (Character *)ele->pDerivedObj;
            int px = chara->x;
            int py = chara->y;

            float dx = master->x - px;
            float dy = master->y - py;
            float distance = sqrt(dx * dx + dy * dy);
            if (master->has_spoken == -2 && !key_state[ALLEGRO_KEY_SPACE]) master->has_spoken = 0;
            if (master->has_spoken == -1 && key_state[ALLEGRO_KEY_SPACE]) master->has_spoken = -2;
            if (distance < 100 && key_state[ALLEGRO_KEY_SPACE]) {
                if (!master->talking && !master->has_spoken) {
                    master->talking = true;
                    master->dialog_index = -1;
                    master->has_spoken = 1;  // 記住講過了
                    dialog_set_text(&master->dialog, master->dialog_lines[0]);
                    dialog_show(&master->dialog);
                }
            }
        }
    }
}
void Master_interact(Elements *self, Elements *tar) {
    // maybe empty for now
}

void Master_advance_dialog(Master *master) {
    if (!master->talking) return;

    static bool last_space = false;
    bool now_space = key_state[ALLEGRO_KEY_SPACE];

    if (now_space && !last_space) {
        master->dialog_index++;
        if (master->dialog_index < MAX_DIALOG_LINES) {
            dialog_set_text(&master->dialog, master->dialog_lines[master->dialog_index]);
        } else {
            dialog_hide(&master->dialog);
            master->talking = false;
            master->has_spoken = -1;
        }
    }

    last_space = now_space;
}
void Master_destroy(Elements *self) {
    Master *master = (Master *)self->pDerivedObj;

    if (master->img)
        al_destroy_bitmap(master->img);
    if (master->hitbox)
        free(master->hitbox);

    free(master);
    free(self);
}