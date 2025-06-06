#include "global.h"
#include "shapes/Shape.h"
#include <stdbool.h>
// variables for global usage
const double FPS = 120.0;
const int WIDTH = 960;
const int HEIGHT = 640;
int window = 0;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_TIMER *fps = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES] = {false};
Point mouse;
bool debug_mode = true;
int now_key = 0;
int last_key = 0;
