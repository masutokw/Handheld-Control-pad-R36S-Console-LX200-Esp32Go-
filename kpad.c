#include "kpad.h"
#include "labels.h"
#include "sdl2utils.h"
//const  char *labels[ROWS * COLS*6] =
const  char *labels[] =
{
    PLANETS,MESSIER, NGC,IC,STARS,
    GOTO,"1","2","3",B_DEL,
    SEARCH, "4", "5","6",CLEAR,
    SYNC, "7", "8","9","+",
    TRACK, ".", "0",":","-",
    UNTRACK,HOME,CONNECT,CLOSE_CONN,ALING,

    /* MODE,STARS, PARK,UNPARK,ALINGC,
     GOTO,SUN,MERCURY,VENUS,MOON,
     SYNC, MARS, JUPITER,SATURN,URANUS,
     "Halt",NEPTUNE,PLUTO," ",COMET,
     TRACK, EAST,WEST,N_POLE,SOUTH,
     UNTRACK,ZENITH,HOME,SETHOME,ALING,*/

    MODE,STARS, N_POLE,SUN,MOON,
    GOTO,SYNC, ZENITH, MERCURY,VENUS,
    TRACK, UNTRACK,WEST,MARS,JUPITER,
    HOME,SETHOME,EAST,SATURN,URANUS,
    PARK, UNPARK,WEST,NEPTUNE,PLUTO,
    ALING," "," "," "," ",



    MODE,"Alp Umi","Alp Tau","Bet Ori","Alp AUR",
    STAR1,"Alp Ori","Alp Cmi","Alp Gem","Alp CMI",
    STAR2,"Bet Gem","Alp Leo","Alp Vir","Alp BOO",
    NORMAL_SYNC,"Alp Sco","Alp Lyr","Alp Aql","Alp Cyg",
    RESET,"Alp Psa"," "," "," ",
    SYNC,GOTO,ALINGC," "," ",

    MODE,"Polaris","Aldebara","Rigel","Capella",
    STAR1,"Betelg","Procion","Castor","Procyon",
    STAR2,"Pollux","Regulus","Spica","Arcturus",
    SYNC,"Antares","Vega","Altair","Deneb",
    RESET,"FomalH","Achenar","Mizar","Alnilam",
    NORMAL_SYNC,GOTO,ALING," "," ",

    GOTO,SEARCH,SYNC,NGC,MESSIER,IC,COMET,CLEAR,
    "a","b","c","d","e","1","2","3",
    "f","g","h", "i","j","4","5","6",
    "k","l", "m","n","o","7","8","9",
    "p", "q","r","s","t","/","0","-",
    "u","w","v","x","y","z"," ",

    ".","*",
    "r","s","t","u"

};




MenuItem items[ROWS*COLS];
MenuItem alp_items[ROWS*ALP_COLS] ;

void init_mat(MenuItem *items)
{
    int idx = 0;
    int cell_W= (WINDOW_WIDTH -COLS*8)/COLS;
    for (int r = 0; r < ROWS; r++)

    {
        for (int c = 0; c < COLS; c++)
        {
            int index=r*COLS+c;
            items[index].label = labels[idx++];
            items[index].rect.x = MARGIN_X + c * (cell_W+ 8);
            items[index].rect.y = MARGIN_Y + r * (CELL_H + 5);
            items[index].rect.w = cell_W;//CELL_W;
            items[index].rect.h = CELL_H;
        }
    }
}

void init_mat_cus(MenuItem *items,int rows,int cols,int offset)
{
    int idx = offset;
    int cell_W= (WINDOW_WIDTH -(cols+1)*MARGIN_X)/cols;
    for (int r = 0; r < rows; r++)

    {
        for (int c = 0; c < cols; c++)
        {
            int index=r*cols+c;
            items[index].label = labels[idx++];
            items[index].rect.x = MARGIN_X + c * (cell_W+ MARGIN_X);
            items[index].rect.y = MARGIN_Y + r * (CELL_H + 5);
            items[index].rect.w = cell_W;
            items[index].rect.h = CELL_H;
        }
    }
}



void changemat(int a,MenuItem *items)
{
    int idx = 0+a*ROWS*COLS;
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            int index=r*COLS+c;
            items[index].label = labels[idx++];

        }
    }
}

void changemat_cus(int a,MenuItem *items,int rows,int cols)
{
    int idx = 0+a*rows*cols;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int index=r*cols+c;
            items[index].label = labels[idx++];

        }
    }
}

int readpad(SDL_Event e,int *last_axis, int *sel,int bsize)
{

    if (e.jaxis.value < -JOY_TRIGGER_ON && *last_axis != -1)
    {
        *sel = (*sel - 1 + bsize) % bsize;
        *last_axis = -1;
    }
    else if (e.jaxis.value > JOY_TRIGGER_ON && *last_axis != 1)
    {
        *sel = (*sel + 1) % bsize;
        *last_axis = 0;
    }
    else if (e.jaxis.value > -JOY_TRIGGER_OFF && e.jaxis.value < JOY_TRIGGER_OFF)
    {
        *last_axis = 0;
    }
    else
    {
        printf("%d empty joy event\n",*sel);
        return 0;
    }

    return 1;
}



