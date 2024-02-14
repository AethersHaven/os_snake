#include "core/core.h"
#include "core/drivers/timer.h"
#include "snake.h"

void main()
{
    core_init();

    snake_init();

    float previous_time = get_time();
    while (1)
    {
        float time = get_time();
        snake_update(time - previous_time);
        previous_time = time;
    }

}