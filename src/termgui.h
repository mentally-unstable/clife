// hello
#ifndef TERMGUI_INCLUDE
#define TERMGUI_INCLUDE

void enableRawMode(void);
int getWindowSize(int *rows, int *cols);
int readKey(void);
void die(const char *str);

#endif