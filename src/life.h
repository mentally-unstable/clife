// hello
#ifndef LIFE_INCLUDE
#define LIFE_INCLUDE

#define SHOW_CUR "\x1b[?25h"
#define HIDE_CUR "\x1b[?25l"
#define CLEAR_SCR  "\x1b[2J\x1b[H"
#define CLEAR_LINE "\x1b[0G\x1b[K"
#define COLOR_BW "\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m"
#define COLOR_RESET "\x1b[1;0m"

enum {
    FILL_CLEAR,
    FILL_RANDOM,
    COLOR,
    NOCOLOR,
};

void welcome_msg(void);
void life_init(void);
void life_loop(int flag);

#endif
