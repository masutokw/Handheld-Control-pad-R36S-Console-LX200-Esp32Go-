#ifndef CONF_H_INCLUDED

#define K36S 1
#define ARKOS4 2
#define PC 3

#ifndef TARGET
#define TARGET PC
#endif



#define NORTH_HAT 1
#define SOUTH_HAT 4
#define WEST_HAT 2
#define EAST_HAT 8
#define GUIDE_BTN 4
#define CENTER_BTN 6
#define SLEW_BTN 5
#define FIND_BTN 7


#if TARGET==K36S
#define NORTH_BTN 14
#define SOUTH_BTN 15
#define WEST_BTN 17
#define EAST_BTN 16
#define JOY1_BTN 11
#define SELECT_BTN 8
#define START_BTN 9
#define X_BTN 2
#define Y_BTN 3
#define A_BTN 0
#define B_BTN  1
#define FN_BTN 13
#define OPEN_DB "/roms/esp32go/deep.db3"

#elif TARGET==ARKOS4

#define NORTH_BTN 8
#define SOUTH_BTN 9
#define WEST_BTN 11
#define EAST_BTN 10
#define JOY1_BTN 14
#define SELECT_BTN 12
#define START_BTN 13
#define X_BTN 2
#define Y_BTN 3
#define A_BTN 1
#define B_BTN  0
#define FN_BTN 16
#define OPEN_DB "/roms/esp32go/deep.db3"

#elif TARGET==PC
#define FN_BTN 90
#define JOY1_BTN 9
#define X_BTN 3
#define Y_BTN 2
#define A_BTN 1
#define B_BTN  0
#define SELECT_BTN 8
#define START_BTN 10
#define OPEN_DB "deep.db3"

#endif


#ifndef NORTH_BTN
#define NORTH_BTN 14
#define SOUTH_BTN 15
#define WEST_BTN 17
#define EAST_BTN 16
#endif

#define ESP_ADDRESS "192.168.1.17"
#define GX_SIZE 49

#ifndef CUSTOMTTF
#define DVSANSBOLD "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
#else
#define DVSANSBOLD "/Library/Fonts/DejaVuSans-Bold.ttf"
#endif


#endif
