// hello
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "termgui.h"
#include "life.h"

#define cell_at(x, y) game.buffer[game.width * (y) + (x)]

typedef struct {
    char *buffer;
    int width;
    int height;
} game_t;
game_t game = {0};

void check_buffer(void);
int qonsulari_cagir(int x, int y);
void fill_buffer(int how);
int random_bit(void);
void print_buffer(void);

void welcome_msg(void) {
    printf(CLEAR_SCR);

    char msg[5][35] = {
        "Conway's Game of Life -- UTDZ\r\n\r\n",
        "q to quit\r\n",
        "c to clear\r\n",
        "r to regenerate\r\n\r\n\r\n",
        "PRESS ANY KEY TO CONTINUE"
    };

    int padding;
    int msgwidth = 35;
    int j = 0;
    for (int i = 0; i < game.height; i++) {
        if (i >= (game.height/2) && j < 5) {
            padding = (game.width / 2) - (msgwidth/2);
            while (padding--) printf(" ");

            printf("%s", &msg[j][0]);
            j++;
            continue;
        }

        printf("\r\n");
    }
    (void) readKey();
}

void life_init(void) {
    srand(time(NULL));
    getWindowSize(&game.height, &game.width);

    game.buffer = (char *) malloc(game.width * game.height);
}

void cleanup(void) {
    printf(SHOW_CUR);
    printf(COLOR_RESET);
}

void life_loop(int flag) {
    printf(CLEAR_SCR);
    printf(HIDE_CUR);

    if (flag == COLOR)
        printf(COLOR_BW);

    fill_buffer(FILL_RANDOM);

    int nread;
    int c;
    for (;;) {
        c = 0;

        nread = read(STDIN_FILENO, &c, 1);
        if (nread == -1 && errno != EAGAIN)
            die("read");


        switch (c) {
            case 'r':
                fill_buffer(FILL_RANDOM);
                break;
            case 'c':
                fill_buffer(FILL_CLEAR);
                break;
            case 'q':
                cleanup();
                die("bye");
            default:
                break;

        }


        check_buffer();
        print_buffer();
        printf(CLEAR_SCR);
    }
}

// Azerbaijani for "invite the neighbors"
// counting the neighboring living cells
int qonsulari_cagir(int x, int y) {
    int count = 0;

    if (cell_at(x-1, y+1)) count++;
    if (cell_at(x, y+1)) count++;
    if (cell_at(x+1, y+1)) count++;
    if (cell_at(x+1, y)) count++;
    if (cell_at(x+1, y-1)) count++;
    if (cell_at(x, y-1)) count++;
    if (cell_at(x-1, y-1)) count++;
    if (cell_at(x-1, y)) count++;

    return count;
}

void check_buffer(void) {
    char *buffer_new = (char *) malloc(game.width * game.height);

    for (int y = 1; y < game.height - 1; y++) {
        for (int x = 1; x < game.width - 1; x++) {

            int n = qonsulari_cagir(x, y);

            // bring the dead to life
            if (!cell_at(x, y) && n == 3) {
                buffer_new[game.width * y + x] = 1;
                continue;
            }

            if (!cell_at(x, y)) continue;

            // then kill off the incels
            if (n < 2) {
                buffer_new[game.width * y + x] = 0;
            } else if (n <= 3) {
                buffer_new[game.width * y + x] = 1;
            } else {
                buffer_new[game.width * y + x] = 0;
            }
        }
    }

    memcpy(game.buffer, buffer_new, game.width*game.height);
}

void print_buffer(void) {
    char *line = (char *) malloc(game.width);

    for (int i = 0; i < game.height; i++) {
        for (int j = 0; j < game.width; j++) {
            line[j] = cell_at(j, i) ? '#' : ' ';
        }
        printf("%s", line);
        if (i < game.height - 1)
            printf("\r\n");
    }
    usleep(10000);
}

// thanks sukhvir stackoverflow
int random_bit() {
    int bit;
    double randval;
    randval = (double) rand() / (double) RAND_MAX;
    if (randval < 0.5) bit = 0;
    else bit = 1;
    return bit;
}

void fill_buffer(int how) {
    for (int i = 0; i < game.height; i++) {
        for (int j = 0; j < game.width; j++) {
            cell_at(j, i) = (how == FILL_RANDOM) ? random_bit() : 0;
        }
    }
}
