#include "core/core.h"
#include "core/drivers/screen.h"
#include "core/util/console.h"

void main()
{
    core_init();

    clear_screen(0x00);
    draw_console();
    flip();

}