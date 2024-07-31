// hello
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>

#define CLEAR_SCR  "\x1b[2J\x1b[H"
#define CLEAR_LINE "\x1b[0G\x1b[K"
#define COLOR_BW "\x1b[38;5;16m\x1b[38;5;15m"
#define COLOR_RESET "\x1b[1;0m"

enum {
    CLEAR,
    RANDOM,
};

typedef struct {
    char *array;
    int w;
    int h;
} matrix_t;

struct termios orig_termios;

void die(const char *str) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(str);
    exit(1);
}

void disableRawMode(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode(void) {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");

    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int readKey(void) {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN)
            die("read");
    }

    return c;
}

int getWindowSize(int *rows, int *cols) {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        die("getWindowSize");
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }

    return -1;
}

void print_array(matrix_t *mat) {
    char *array = mat->array;
    char *line = (char *) malloc(mat->w);

    for (int i = 0; i < mat->h; i++) {
        for (int j = 0; j < mat->w; j++) {
            line[j] = array[mat->w * i + j] ? '#' : ' ';
        }
        printf(COLOR_BW"%s\r\n"COLOR_RESET, line);
    }
    usleep(10000);
}

// thanks sukhvir stackoverflow
int random_bit() {
   int bit;
   double randval;
   randval = (double)rand()/(double)RAND_MAX;
   if (randval<0.5) bit=0;
   else bit=1;
   return bit;
}

/*
   Conway's Game of Life -- UTDZ

   q to quit
   c to clear
   r to regenerate


   PRESS ANY KEY TO CONTINUE
   */

void fill_array(matrix_t *mat, int how) {
    char *array = mat->array;

    for (int i = 0; i < mat->h; i++) {
        for (int j = 0; j < mat->w; j++) {
            array[mat->w * i + j] = how == RANDOM ? random_bit() : 0;
        }
    }
}

int qonsulari_cagir(matrix_t *mat, int x, int y) {
    int count = 0;
    char *array = mat->array;

    if (array[mat->w * (y+1) + (x-1)]) count++;
    if (array[mat->w * (y+1) + (x)]) count++;
    if (array[mat->w * (y+1) + (x+1)]) count++;
    if (array[mat->w * (y) + (x+1)]) count++;
    if (array[mat->w * (y-1) + (x+1)]) count++;
    if (array[mat->w * (y-1) + (x)]) count++;
    if (array[mat->w * (y-1) + (x-1)]) count++;
    if (array[mat->w * (y) + (x-1)]) count++;

    return count;
}

void check_array(matrix_t *mat) {
    char *array = mat->array;
    char *array_new = (char *) malloc(mat->w * mat->h);

    for (int y = 1; y < mat->h - 1; y++) {
        for (int x = 1; x < mat->w - 1; x++) {

            int n = qonsulari_cagir(mat, x, y);

            // bring the dead to life
            if (!array[mat->w * y + x] && n == 3) {
                array_new[mat->w * y + x] = 1;
                continue;
            }

            if (!array[mat->w * y + x]) continue;

            // then kill off the incels
            if (n < 2) {
                array_new[mat->w * y + x] = 0;
            } else if (n <= 3) {
                array_new[mat->w * y + x] = 1;
            } else {
                array_new[mat->w * y + x] = 0;
            }
        }
    }

    mat->array = array_new;
}

void welcome_msg(void) {
    char *msg = "Conway's Game of Life -- UTDZ\r\n\r\nq to quit\r\nc to clear\r\nr to regenerate\r\n\r\n\r\nPRESS ANY KEY TO CONTINUE";
    int msgwidth = 30;
    for (int i = 0; i < height; i++) {
        printf("\r\n");
        if (i == (height/3) - 3) {
            while (msgwidth--) printf(" ");
            printf("%s\r\n", msg);
        }
    }
    (void) readKey();
}

int main(void) {
    enableRawMode();
    srand(time(NULL));

    int width, height;
    getWindowSize(&height, &width);

    matrix_t matrix = (matrix_t) {
        .array = (char *) malloc(width*height),
        .w = width,
        .h = height,
    };


    welcome_msg(width, height);


    fill_array(&matrix, RANDOM);

    int nread;
    int c;
    for (;;) {
        c = 0;

        nread = read(STDIN_FILENO, &c, 1);
        if (nread == -1 && errno != EAGAIN)
            die("read");


        switch (c) {
            case 'r':
                fill_array(&matrix, RANDOM);
                break;
            case 'c':
                fill_array(&matrix, CLEAR);
                break;
            case 'q':
                die("bye");
            default:
                break;

        }



        check_array(&matrix);
        print_array(&matrix);
        printf(CLEAR_SCR);
    }

    return 0;
}
