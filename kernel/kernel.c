#include "core/mem.h"
#include "core/drivers/screen.h"

void main()
{
    init_heap();
    init_screen();

    clear_screen(0x15);
    fill_rect(-0.5f, -0.5f, 0, 0, 0x0F);
    draw_line(0.5f, 0.9f, -1, -1, 0);
    fill_triangle(-0.6f, -0.6f, 0.8f, 0.1f, 0.1f, 0.9f, 0x05);
    flip();
}