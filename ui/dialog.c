#include "dialog.h"
#include <string.h>
#include <allegro5/allegro_primitives.h>

void dialog_init(DialogBox *box, const char *text, int x, int y, int w, int h) {
    strncpy(box->text, text, sizeof(box->text) - 1);
    box->text[sizeof(box->text) - 1] = '\0'; // 確保有 null terminator
    box->x = x;
    box->y = y;
    box->width = w;
    box->height = h;
    box->visible = true;
}

void dialog_draw(DialogBox *box, ALLEGRO_FONT *font) {
    if (!box->visible) return;

    al_draw_filled_rectangle(
        box->x, box->y, box->x + box->width, box->y + box->height,
        al_map_rgba(0, 0, 0, 200)
    );

    al_draw_rectangle(
        box->x, box->y, box->x + box->width, box->y + box->height,
        al_map_rgb(255, 255, 255), 2
    );

    al_draw_multiline_text(
        font,                         // 使用的字型
        al_map_rgb(255, 255, 255),    // 白色文字
        box->x + 10,                  // 文字區塊左邊邊界（內縮 10 px）
        box->y + 10,                  // 文字區塊頂端（內縮 10 px）
        box->width - 20,             // 欄寬（內縮左右各 10 px）
        al_get_font_line_height(font), // 每行的高度（根據字型自動決定）
        0,                            // 額外的 flag（通常設 0）
        box->text                     // 你要顯示的文字內容
    );
}

void dialog_set_text(DialogBox *box, const char *text) {
    strncpy(box->text, text, sizeof(box->text) - 1);
    box->text[sizeof(box->text) - 1] = '\0'; // 保證有結尾字元
}

void dialog_show(DialogBox *box) {
    box->visible = true;
}

void dialog_hide(DialogBox *box) {
    box->visible = false;
}