#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "kpad.h"
#include "conf.h"
#include "query.h"
#include "sdl2utils.h"
#include "net.h"
#include <math.h>
#include "labels.h"

TTF_Font* font1;
TTF_Font* font2;
TTF_Font* font;
TTF_Font* font3;
char spd[20]="?";
char buffp[200] =" ";
char strlxcoord[256]=" ";
char buff_alt[200] =" ";
char buftarget[1024]=" ";
char str_ra[40];
char str_dec[40];
char str_az[40];
char str_alt[40];
int  pad_page=0;

extern MenuItem items[ROWS*COLS],alp_items[ROWS*ALP_COLS];
uintptr_t sockfd;
char buf[1024]=" ";
char buf3[1024]=" ";
char inputbuffer[200]="M42";
//double lat =36.72 ;
//double lng= -4.12;
double ra,dec;
struct ln_lnlat_posn observer= {-4.12,36.72};
char eqmode='P';
//Prototypes not declared on Headers
void drawMainScreen(SDL_Renderer *renderer,int sel_row, int sel_col) ;
void draw_pad(SDL_Renderer *renderer,int sel_row,int sel_col,TTF_Font *font,MenuItem *items,int rows,int cols);
//--------------------------------------------
void calcular_posicion(const char *nombre)
{
    static double altitude;
    struct ln_equ_posn pos;
    struct lnh_equ_posn hequ;
    struct ln_hrz_posn hrz;
    char temp[100]="   ";
    double JD = ln_get_julian_from_sys();
    if (strcmp(nombre, SUN) == 0)
    {
        ln_get_solar_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre, MERCURY) == 0)
    {
        ln_get_mercury_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre,VENUS) == 0)
    {
        ln_get_venus_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre,MOON) == 0)
    {
        ln_get_lunar_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre, MARS) == 0)
    {
        ln_get_mars_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre,JUPITER) == 0)
    {
        ln_get_jupiter_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre,SATURN) == 0)
    {
        ln_get_saturn_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre,URANUS) == 0)
    {
        ln_get_uranus_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre, NEPTUNE) == 0)
    {
        ln_get_neptune_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre,PLUTO) == 0)
    {
        ln_get_pluto_equ_coords(JD, &pos);
        strcpy(inputbuffer,nombre);
        buffp[0]=0;
    }
    else if (strcmp(nombre, "<-") == 0)
    {
        if (strlen(inputbuffer)<0)
            inputbuffer[strlen(inputbuffer)-1]=0;
        return;
    }
    else if (strcmp(nombre, CLEAR) == 0)
    {
        inputbuffer[0]=0;
        return;
    }
    else if (strcmp(nombre, SEARCH) == 0)
    {
        if (squery(inputbuffer,buffp,&pos,inputbuffer[0])) printf("Found!\n");
        else
        {
            printf("Search\n");
            char * ptr;
            int    ch = ' ';

            ptr = strchr(inputbuffer, ch );
            ch=(ptr==NULL)? 's':'f';
            if (squery(inputbuffer,buffp,&pos,ch)) printf("Found!\n");
            else
            {
                if (squery(inputbuffer,buffp,&pos,'c') ) printf("Found!\n");
                else
                {
                    printf("%s not found!\n",buffp);
                    return;
                }
            }
        }
        // return;
    }
    else if (strcmp(nombre, COMET) == 0)
    {
        if (squery(inputbuffer,buffp,&pos,'c') ) printf("Found!\n");
        else
        {
            printf("%s not found!\n",buffp);
            return;
        }
        // return;
    }
    else if (strcmp(nombre, SEARCHST) == 0)
    {
        printf("Search\n");
        char * ptr;
        int    ch = ' ';

        ptr = strchr(inputbuffer, ch );
        ch=(ptr==NULL)? 's':'f';
        if (squery(inputbuffer,buffp,&pos,ch)) printf("Found!\n");
        else
        {
            printf("%s not found!\n",buffp);
            return;
        }


    }
    else if (strcmp(nombre, GOTO) == 0)
    {
        if (altitude>=0.0)
        {
            sendCmd(sockfd,strlxcoord);
            sendCmd(sockfd,":MS#");
            read(sockfd,temp,4);
        }
        else printf("Bajo el horizonte\n");
        return;
    }
    else if (strcmp(nombre, SYNC) == 0)
    {

        sendCmd(sockfd,strlxcoord);
        sendCmd(sockfd,":CM#");
        read(sockfd,temp,4);
        return;
    }
    else if (strcmp(nombre,CONNECT) == 0)
    {
        close(sockfd);
        sockfd=initsock();
        if (sockfd>=0)
        {
            //render_text( renderer, 20,240,(char*) ip(),font2,RED);
            printf("%s\n",ip());
            read_geo(sockfd);
        }
        else
        {
            //render_text( renderer, 20,240,(char*) "Disconnected   ",font2,RED);
            printf("%s\n","Disconnected");
        }


        return;

    }
    else if (strcmp(nombre, CLOSE_CONN) == 0)
    {
        close(sockfd);

        printf("%s\n","Disconnected");

        return;

    }
    else if (strcmp(nombre, NGC) == 0)
    {
        strcpy(inputbuffer,nombre);
        return;
    }
    else if (strcmp(nombre, MESSIER) == 0)
    {
        strcpy(inputbuffer,"M");
        return;
    }
    else if (strcmp(nombre, "IC") == 0)
        strcpy(inputbuffer,nombre);
    else if ((strlen(nombre)==1) )
    {
        strcat(inputbuffer,nombre);
        return;
    }
    else if (strcmp(nombre, HOME) == 0)
    {
        sendCmd(sockfd, ":pH#");
        return;

    }
    else if (strcmp(nombre,WEST) == 0)
    {
        sendCmd(sockfd, ":hH3#");
        return;

    }
    else if (strcmp(nombre, EAST) == 0)
    {
        sendCmd(sockfd, ":hH2#");
        return;

    }
    else if (strcmp(nombre, SETHOME) == 0)
    {
        sendCmd(sockfd, ":hH2#");
        return;

    }
    else if (strcmp(nombre, ZENITH) == 0)
    {
        sendCmd(sockfd, ":hH1#");
        return;

    }

    else if (strcmp(nombre, N_POLE) == 0)
    {
        sendCmd(sockfd, ":hH0#");
        return;

    }
    else if (strcmp(nombre, PARK) == 0)
    {
        sendCmd(sockfd,":hP#");
        return;
    }
    else if (strcmp(nombre, TRACK) == 0)
    {
        sendCmd(sockfd, ":Qw#:Mt#");
        return;
    }
    else if (strcmp(nombre, UNTRACK) == 0)
    {
        sendCmd(sockfd, ":Mh#");
        return;
    }
    else if (strcmp(nombre, STAR1) == 0)
    {
        sendCmd(sockfd, ":a1#");
        return;
    }
    else if (strcmp(nombre, STAR2) == 0)
    {
        sendCmd(sockfd, ":a2#");
        return;
    }
    else if (strcmp(nombre, "Reset") == 0)
    {
        sendCmd(sockfd, ":a3#");
        return;
    }
    else if (strcmp(nombre, NORMAL_SYNC) == 0)
    {
        sendCmd(sockfd, ":a0#");
        return;
    }
    else if (strcmp(nombre, MODE) == 0)
    {
        pad_page=0;
        changemat( pad_page,items);
        return;
    }
    else if (strcmp(nombre, PLANETS) == 0)
    {
        pad_page=1;
        changemat( pad_page,items);
        return;

    }
    else if (strcmp(nombre, STARS) == 0)
    {
        pad_page=2;
        changemat( pad_page,items);
        return;
    }
    else if (strcmp(nombre, ALING) == 0)
    {
        pad_page=3;
        changemat( pad_page,items);
        return;
    }
    else if (strcmp(nombre, ALINGC) == 0)
    {
        pad_page=4;
        changemat( pad_page,items);
    }

    else
    {
        char * ptr;
        int    ch = ' ';

        ptr = strchr(nombre, ch );
        ch=(ptr==NULL)? 's':'f';

        printf("No calculable: %s\n", nombre);
        // return;
        printf("Search %c \n",ch);
        strcpy(inputbuffer,nombre);
        if (squery(inputbuffer,buffp,&pos,ch)) printf("Found!\n");
        else
        {
            printf("%s not found!\n",buffp);
            return;
        }



    }

    printf("%s - RA: %.4f h, DEC: %.4f°\n", nombre, pos.ra, pos.dec);

    ln_get_equ_prec(&pos,JD,&pos);

    ln_equ_to_hequ (&pos, &hequ);
    int sig=(hequ.dec.neg==1)? -1:1;
    ln_get_hrz_from_equ (&pos, &observer, JD, &hrz);
    sprintf(buftarget,"%s  %02d:%02d:%02.0f/%+03d°%02d\'%02.0f Az %.2f Alt %.2f  %s  ",inputbuffer,
            hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, sig * hequ.dec.degrees, hequ.dec.minutes,
            hequ.dec.seconds,fmod (hrz.az+180.0,360.0),hrz.alt,hrz.alt>0.0? "":"UH");

    sprintf(strlxcoord,":Sr%02d:%02d:%02.0f#:Sd%+03d:%02d:%02.0f#",hequ.ra.hours,
            hequ.ra.minutes, hequ.ra.seconds, sig *hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);


    //  sprintf (buff_alt,"Az  %.4f  Alt %.4f°    ",fmod (hrz.az+180.0,360.0),hrz.alt);
    printf ("Alt %f\n", hrz.alt);
    altitude=hrz.alt;

}
//Thread function  for read esp32go  position and status data
int readsock(void *point)
{
    SDL_Event user_event;
    SDL_zero(user_event);  /* Create custom event to updare screen on socket receive */
    user_event.type = SDL_USEREVENT;
    user_event.user.code = 2;
    user_event.user.data1 = NULL;
    user_event.user.data2 = NULL;
    SDL_PushEvent(&user_event);
    int *sockf= point;
    int sockfd=(uintptr_t) sockf;
    int count;
    struct lnh_equ_posn hequ;
    struct ln_hrz_posn hrz;
    struct ln_equ_posn pos;
    struct lnh_hrz_posn hpos;

    // int n=0;
    int len=0;
    while (true)
    {
        ioctl(sockfd, FIONREAD, &count);
        read(sockfd,buf,count);
        // read(sockfd,buf,100);
        len = read_esp(sockfd,buf3);
        if (len==GX_SIZE || len==GX_SIZE+1)
        {
            hequ.ra.hours= (buf3[0]-48)*10+(buf3[1]-48);
            hequ.ra.minutes=(buf3[3]-48)*10+(buf3[4]-48);
            hequ.ra.seconds=(buf3[6]-48)*10.0+(buf3[7]-48)+(buf3[9]-48)*0.1;
            //ra=hour*3600.0+min*60.0+sec;
            //  printf("ra %d %d %.1lf  \n",hequ.ra.hours,hequ.ra.minutes,hequ.ra.seconds);
            sprintf(str_ra,"%02d:%02d:%04.1lf",hequ.ra.hours,hequ.ra.minutes,hequ.ra.seconds);
            hequ.dec.neg=(buf3[11]=='-')?1:0;
            hequ.dec.degrees= (buf3[12]-48)*10+(buf3[13]-48);
            hequ.dec.minutes=(buf3[16]-48)*10+(buf3[17]-48);
            hequ.dec.seconds=(buf3[19]-48)*10.0+(buf3[20]-48);
            //dec=sg *(deg*3600.0+mind*60.0+secd);
            //printf("dec %d %d %.0lf \n", hequ.dec.degrees,hequ.dec.minutes, hequ.dec.seconds);
            sprintf(str_dec,"%c%02d°%02d'%02.0lf\"",(hequ.dec.neg<<1)+'+',hequ.dec.degrees,hequ.dec.minutes, hequ.dec.seconds);
            ln_hequ_to_equ(&hequ,&pos);
            double JD = ln_get_julian_from_sys();
            ln_get_hrz_from_equ (&pos, &observer, JD, &hrz);
            // printf (" az  %.04f Alt %.04f\n",fmod (hrz.az+180.0,360.0), hrz.alt);
            hrz.az=fmod (hrz.az+180.0,360.0);
            ln_hrz_to_hhrz(&hrz,&hpos);
            sprintf(str_az,"%+03d°%02d'%02.0lf\"",hpos.az.degrees,hpos.az.minutes,hpos.az.seconds);
            sprintf(str_alt,"%c%02d°%02d'%02.0lf\"",(hpos.alt.neg<<1) +'+',hpos.alt.degrees,hpos.alt.minutes,hpos.alt.seconds);
            sprintf(buff_alt,"%+03d°%02d'%02.0lf\" alt %c%02d°%02d'%02.0lf",hpos.az.degrees,hpos.az.minutes,hpos.az.seconds,
                    (hpos.alt.neg<<1) +'+',hpos.alt.degrees,hpos.alt.minutes,hpos.alt.seconds);
            //printf("%s\n",buff_alt);


            SDL_PushEvent(&user_event);
        }
        //  SDL_PushEvent(&user_event);
        SDL_Delay(1000);
    }
    return 0;
}

//Default Screen update  only on SDL event  Updates

void drawMainScreen(SDL_Renderer *renderer,int sel_row, int sel_col)
{
    int status =buf3[47]-48+buf3[48]-48;
    const char state[50];
    const char batt[15]="100%";
    //SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // sprintf(state,"%s %s %s" ,(status&1)?"Tracking":"Stop",(status&2)?"Parked":" ",(status&8)?"Slewing":" ");
    if (status&8) strcpy(( char * restrict) state,SLEWING) ;
    else if (status&2) strcpy(( char * restrict) state,PARKED);
    else  strcpy(( char * restrict) state,(status&1)? TRACKING:STOPPED) ;
    // sprintf(state,"%s " ,(status&1)?"Tracking":"Stop",(status&2)?"Parked":" ",(status&8)?"Slewing":" ");
    SDL_RenderClear(renderer);
    if (pad_page==4)
        draw_pad(renderer,sel_row,sel_col,font,alp_items,ROWS,ALP_COLS);
    else
        draw_pad(renderer,sel_row,sel_col,font,items,ROWS,COLS);

    render_text( renderer,10,240,(char* )buftarget, font3,ORANGE);//lx200 target
    render_text( renderer,10,215,(char* )buffp, font3,ORANGE);//lx200 target

    render_text(renderer,20,20,(char*) str_ra,font1,RED); //ra
    render_text(renderer,20,90,(char*) str_dec,font1,RED);//dec

    render_text(renderer,550,150,(char*) state,font2,RED);
    render_text( renderer,550,170,(char*)spd,font2,RED);
    if (eqmode=='P')
    {
        render_text(renderer,320,20,(char*) str_az,font1,REDW);//az
        render_text(renderer,325,90,(char*) str_alt,font1,REDW);//alt
    }
    else
    {
        render_text(renderer,340,20,(char*) (buf3+23),font1,REDW);//az
        render_text(renderer,325,90,(char*) (buf3+35),font1,REDW);//alt
        //render_text(renderer,325,150,(char*) buff_alt,font2,REDW);//alt
    }
    // render_text(renderer,550,150,(char*) buf3+47,font2,RED);

    //  render_text(renderer,20,20,(char*) buf3,font1,RED); //ra
    //render_text(renderer,20,90,(char*) buf3+11,font1,RED);//dec

    // render_text(renderer,340,20,(char*) (buf3+23),font1,REDW);//az
    // render_text(renderer,325,90,(char*) (buf3+35),font1,REDW);//alt
    // render_text(renderer,325,150,(char*) buff_alt,font2,REDW);//alt;
    sprintf((char * restrict) batt,"%d%c",read_battery(),'%');
    SDL_Rect rect= {5,5,630,140};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    render_text( renderer, 15,7,(char*) RA,font2,WHITEL);
    render_text( renderer, 340,7,(char*) AZ,font2,WHITEL);
    render_text( renderer, 15,75,(char*) DEC,font2,WHITEL);
    render_text( renderer, 340,75,(char*) ALT,font2,WHITEL);
    render_text( renderer, 10,140,"                 ",font1,RED);
    render_text( renderer, 10,140,inputbuffer,font1,ORANGE);
    render_text( renderer, 600,7,(char*) batt,font2,ORANGE);
    // Draw the rectangle (filled)
    SDL_RenderDrawRect(renderer, &rect);
    rect.w = 640/2;
    SDL_RenderDrawRect(renderer, &rect);
    rect= (SDL_Rect)
    {
        5,144,630,120
    };
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderPresent(renderer);

}
int main(int argc, char *argv[])
{
    int i;
    printf("argc is %d\n",argc == 2);
    for(i=0; i<argc; i++)
        printf("argv[%d] is %s\n",i,argv[i]);

    initDB();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        printf("Error SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0)
    {
        printf("Error TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Joystick *joystick = NULL;
    if (SDL_NumJoysticks() > 0)
        joystick = SDL_JoystickOpen(0);

    SDL_Window *window = SDL_CreateWindow("ESP32Go r36s",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    init_fonts();

    init_mat_cus(items,ROWS,COLS,0);
    init_mat_cus(alp_items,ROWS,ALP_COLS,120);
    int sel_row = 0, sel_col = 0;
    int last_haxis = 0, last_vaxis = 0;
    bool running = true;
    SDL_Event e;
    readcc();
    sockfd=initsock();
    if (sockfd>=0)
    {
//        render_text( renderer, 20,400,(char*)ESP_ADDRESS,font2);
        //  render_text( renderer, 20,400,(char*)line,font2);
        SDL_Thread *thread = SDL_CreateThread(readsock, "thread",(void *)sockfd);
        if (thread == NULL)
        {
            printf("Could not create thread! %s\n", SDL_GetError());
            return 1;
        }
    }
    else
    {
        render_text( renderer, 20,200,(char*) "Disconnected",font2,RED);
    }


    calcular_posicion(SEARCH);

    //unsigned int cn=0;
    int jmove=1;

    while (running)

    {
        //  while (SDL_PollEvent(&e))
        if (SDL_WaitEvent(&e))
        {
            if  (e.type ==SDL_JOYHATMOTION)
            {
                switch(e.jhat.value)
                {
                    {
                    case NORTH_HAT: // Move North
                        sendCmd(sockfd,":Mn#");
                        break;
                    case SOUTH_HAT: // Move South
                        sendCmd(sockfd,":Ms#");

                        break;
                    case WEST_HAT : // Move West
                        sendCmd(sockfd,":Mw#");
                        break;
                    case EAST_HAT: // Move East
                        sendCmd(sockfd,":Me#");

                        break;
                    case 0: // Stop
                        sendCmd(sockfd,":Qw#:Qn#");

                        break;
                    }

                }
                printf("The hat with index %d was moved to position %d.\n", e.jhat.hat, e.jhat.value);
            }
            if (e.type == SDL_QUIT) running = false;

            else if (e.type == SDL_JOYAXISMOTION)
            {
                if (e.jaxis.axis == 0)
                {
                    jmove=readpad( e,&last_haxis,&sel_col,pad_page==4? ALP_COLS:COLS);
                }
                else if (e.jaxis.axis == 1)
                {
                    jmove=readpad( e,&last_vaxis,&sel_row,ROWS);
                }


            }
            else if (e.type == SDL_JOYBUTTONDOWN)
            {
                switch(e.jbutton.button)
                {
                case NORTH_BTN: // Fn

                    sendCmd(sockfd, ":Mn#");
                    break;
                case SOUTH_BTN: // Start
                    sendCmd(sockfd, ":Ms#");
                    break;
                case WEST_BTN: // Fn
                    sendCmd(sockfd, ":Mw#");
                    break;
                case EAST_BTN: // Start
                    sendCmd(sockfd, ":Me#");
                    break;
                case GUIDE_BTN: // Start
                    sendCmd(sockfd, ":RG#");
                    strcpy(spd,GUIDE);
                    break;
                case SLEW_BTN: // Start
                    sendCmd(sockfd, ":RS#");
                    strcpy(spd,SLEW);

                    break;
                case CENTER_BTN: // Start
                    sendCmd(sockfd, ":RC#");
                    strcpy(spd,CENTER);

                    break;
                case FIND_BTN: // Start
                    sendCmd(sockfd, ":RM#");
                    strcpy(spd,FIND);

                    break;
                case X_BTN: // Start
                    inputbuffer[strlen(inputbuffer)-1]=0;

                    break;
                case Y_BTN: // Start
                    // if (pad_page==4) pad_page=0;
                    //else
                    pad_page=(pad_page+1) %5;
                    //    pad_page=4;
                    changemat( pad_page,items);

                    break;
                case FN_BTN:
                    close(sockfd);
                    sockfd=initsock();
                    if (sockfd>=0)
                    {
                        render_text( renderer, 20,240,(char*) ip(),font2,RED);
                    }
                    else
                    {
                        render_text( renderer, 20,240,(char*) "Disconnected   ",font2,RED);
                    }


                    break;

                case SELECT_BTN:
                    calcular_posicion(GOTO);
                    break;
                case START_BTN:
                    calcular_posicion(SEARCH);
                    break;

                default:

                    break;
                }

                printf("Button down. Button=%d\n", e.jbutton.button);
                /*  drawText( screenSurface, (char*) buf, 48,  0,0, fgC1, bgC1);
                  SDL_UpdateWindowSurface(window);*/
                if ((e.jbutton.button == JOY1_BTN)  || (e.jbutton.button == A_BTN)) // Botón A
                    //  if (e.jbutton.button == B_BTN)   // Botón A
                {
                    const char *seleccion;
                    if (pad_page==4)

                    {
                        seleccion = alp_items[sel_row*ALP_COLS+sel_col].label;
                    }
                    else
                    {
                        seleccion = items[sel_row*COLS+sel_col].label;
                    }
                    calcular_posicion(seleccion);


                }
                if (e.jbutton.button == B_BTN)   // Botón B
                {
                    running = false;
                }
            }
            else if (e.type == SDL_JOYBUTTONUP)
            {
                switch(e.jbutton.button)
                {
                case NORTH_BTN: //stop N

                    sendCmd(sockfd, ":Qn#");
                    break;
                case SOUTH_BTN: // stop S
                    sendCmd(sockfd, ":Qs#");

                    break;
                case WEST_BTN: // stop W
                    sendCmd(sockfd, ":Qw#");
                    break;
                case EAST_BTN: // Sstop E
                    sendCmd(sockfd, ":Qe#");
                    break;

                case 13: // Start
                    // quit=SDL_TRUE;
                    break;
                //   case Y_BTN: // Start
                // pad_page=(pad_page+1) %6;
                //  pad_page=0;
                //  changemat( pad_page,items);
                //  break;

                default:
                    break;
                }
                printf("Button up. Button=%d\n", e.jbutton.button);
            }
            else if (e.type == SDL_USEREVENT) ;//printf("User event %d\r\n",e.user.code);

        }

         if ((e.type == SDL_JOYAXISMOTION) && (jmove==0)) {printf("not refresh screen \n");} else
         drawMainScreen(renderer,sel_row,sel_col);

    }

    if (joystick) SDL_JoystickClose(joystick);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    closeDB();
    return 0;
}
