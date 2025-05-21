#pragma once
#include <allegro5/allegro_font.h>

typedef struct {
    char text[512];
    int x, y, width, height;
    bool visible;
} DialogBox;

void dialog_init(DialogBox *box, const char *text, int x, int y, int w, int h);
void dialog_draw(DialogBox *box, ALLEGRO_FONT *font);
void dialog_set_text(DialogBox *box, const char *text);
void dialog_show(DialogBox *box);
void dialog_hide(DialogBox *box);