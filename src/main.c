// hello
#include "args.h"
#include "life.h"
#include "termgui.h"

int main(int argc, char **argv) {
    args_t *args = parse_args(argc, argv);

    enableRawMode();
    life_init();

    if (!args->nowelcome)
        welcome_msg();

    life_loop(args->nocolor ? NOCOLOR : COLOR);
    return 0;
}
