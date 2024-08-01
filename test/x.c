//salam

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define width  150
#define height 50

char charray[height][width];
char next_charray[height][width];
int qonsu(int x, int y);
void print(char * charray);
void printrand(char * charray);
void CLR_SCR();
void move_cursor_to_top_masallah();

int main() {
    printrand(*charray); //generate random grid

    for(;;) {
        print(&charray[0][0]);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int qonsular = qonsu(x, y);
                if (charray[y][x] == '*' && (qonsular < 2 || qonsular > 3)) {
                    next_charray[y][x] = ' ';
                } else if (charray[y][x] == ' ' && qonsular == 3) {
                    next_charray[y][x] = '*';
                } else {
                    next_charray[y][x] = charray[y][x];
                }
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                charray[y][x] = next_charray[y][x]; //transferring cells back to the old array masallah
            }
        }
        usleep(20000); // ms (2000 is 2ms) i think, whatever it is its very fast
        CLR_SCR();
    }

    return 0;
}

/* int main(){ */
/*     printrand(*charray); */
/*     print(&charray[0][0]); */

/*     for(int y = 0; y < height; y++){ */
/*         for(int x = 0; x < width; x++){ */
/*             if(qonsu(x, y) > 1 || qonsu(x,y) < 4){ //survive */
/*                 /1* printf("array at [%i,%i] has %i\n", x ,y , qonsu(x, y)); *1/ */
/*             }else if( charray[y][x] == ' ' && qonsu(x,y) == 3){ //new cell appear */
/*                 charray[y][x] = '*'; */
/*             }else{ //kill cells */
/*                 charray[y][x] = ' '; */
/*             } */
/*         } */
/*     } */

/*     return 0; */
/* } */

int qonsu(int x, int y){
    int qonshi = 0;

    if (charray[y+1] [x-1] == '*') qonshi++;
    if (charray[y+1] [x] == '*') qonshi++;
    if (charray[y+1] [x+1] == '*') qonshi++;
    if (charray[y]   [x+1] == '*') qonshi++;
    if (charray[y-1] [x+1] == '*') qonshi++;
    if (charray[y-1] [x] == '*') qonshi++;
    if (charray[y-1] [x-1] == '*') qonshi++;
    if (charray[y]   [x-1] == '*') qonshi++;

    return qonshi;
}

void print(char * charray){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            printf("%c", charray[width * y + x]);
        }
        printf("\n");
    }
}

void printrand(char * charray){
    srand(time(NULL));
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int random = rand() % 100; // Generate a new random number for each cell
            if(random < 40){
                charray[width * y + x] = '*';
            } else {
                charray[width * y + x] = ' ';
            }
        }
    }
}

void CLR_SCR() {
    /* printf("\033[H\033[J"); */
    printf("\033[2J\033[H");
}

void move_cursor_to_top_masallah() {
    printf("\033[H");
}
