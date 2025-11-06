#ifndef KPAD_H_INCLUDED
#include <SDL2/SDL.h>
#define COLS 5
#define ROWS 6
#define ALP_COLS 8
#define CELL_W 120
#define CELL_H 30
#define MARGIN_X 5
#define MARGIN_Y 270
#define JOY_TRIGGER_ON 31000
#define JOY_TRIGGER_OFF 30000
typedef struct
{
    const char *label;
    SDL_Rect rect;
} MenuItem;

void init_mat(MenuItem *items);
void init_mat_cus(MenuItem *items,int rows,int cols,int offset);
void changemat(int a,MenuItem *items);
int readpad(SDL_Event e,int *last_axis, int *sel,int bsize);
#endif
