#include "snake.h"

#include "core/mem.h"
#include "core/drivers/keyboard.h"
#include "core/drivers/screen.h"
#include "core/util/console.h"
#include "core/util/rand.h"

float tick_time = 1.0f / FPS;

int *snake_body = NULL;
int snake_length;
bool dead;

int last_input_x, last_input_y;
int input_x, input_y;
int apple_x, apple_y;

void update();
void render();
void spawn_apple();
void get_input();
void draw_square(int x, int y, unsigned char palette_index);
bool is_in_snake(int x, int y);

void snake_init()
{
    if (!snake_body)
    {
        snake_body = malloc(GRID_WIDTH * GRID_HEIGHT * 2 * sizeof(int));
    }
    snake_length = 3;
    dead = false;
    snake_body[0] = GRID_WIDTH / 2 + 1;
    snake_body[1] = GRID_HEIGHT / 2;
    snake_body[2] = snake_body[0] - 1;
    snake_body[3] = snake_body[1];
    snake_body[4] = snake_body[2] - 1;
    snake_body[5] = snake_body[1];
    spawn_apple();
    last_input_x = 1;
    last_input_y = 0;
    input_x = 1;
    input_y = 0;
}

void snake_update(float dt)
{
    get_input();

    tick_time -= dt;
    if (tick_time <= 0)
    {
        update();
        if (!dead)
        {
            render();
            tick_time = 1.0f / FPS;
        }
        else
        {
            tick_time = 1;
            snake_init();
        }
    }
}

void update()
{
    last_input_x = input_x;
    last_input_y = input_y;

    bool ate_apple = snake_body[0] + input_x == apple_x && snake_body[1] + input_y == apple_y;
    snake_length += ate_apple ? 1 : 0;
    for (int i = snake_length - 2; i >= 0; i--)
    {
        snake_body[(i + 1) * 2 + 0] = snake_body[i * 2 + 0];
        snake_body[(i + 1) * 2 + 1] = snake_body[i * 2 + 1];
    }
    snake_body[0] = snake_body[0] + input_x;
    snake_body[1] = snake_body[1] + input_y;

    if (ate_apple)
    {
        spawn_apple();
    }

    if (snake_body[0] < 0 || snake_body[0] >= GRID_WIDTH || snake_body[1] < 0 || snake_body[1] >= GRID_HEIGHT)
    {
        dead = true;
        return;
    }
    for (int i = 1; i < snake_length; i++)
    {
        if (snake_body[0] == snake_body[i * 2 + 0] && snake_body[1] == snake_body[i * 2 + 1])
        {
            dead = true;
            return;
        }
    }
}

void render()
{
    clear_screen(0x00);

    for (int i = 0; i < snake_length; i++)
    {
        draw_square(snake_body[i * 2 + 0], snake_body[i * 2 + 1], 0x0A);
    }

    draw_square(apple_x, apple_y, 0x28);

    draw_console();
    flip();
}

void spawn_apple()
{
    apple_x = rand_range(0, GRID_WIDTH);
    apple_y = rand_range(0, GRID_HEIGHT);

    for (int x = 0; x < GRID_WIDTH; x++)
    {
        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            int new_x = (apple_x + x) % GRID_WIDTH;
            int new_y = (apple_y + y) % GRID_HEIGHT;
            if (!is_in_snake(new_x, new_y))
            {
                apple_x = new_x;
                apple_y = new_y;
                return;
            }
        }
    }
}

void get_input()
{
    if (get_key(KEY_W) && last_input_y != 1)
    {
        input_y = -1;
        input_x = 0;
    }
    else if (get_key(KEY_S) && last_input_y != -1)
    {
        input_y = 1;
        input_x = 0;
    }
    else if (get_key(KEY_A) && last_input_x != 1)
    {
        input_x = -1;
        input_y = 0;
    }
    else if (get_key(KEY_D) && last_input_x != -1)
    {
        input_x = 1;
        input_y = 0;
    }
}

void draw_square(int x, int y, unsigned char palette_index)
{
    int x1 = x * (SCREEN_WIDTH / GRID_WIDTH);
    int y1 = y * (SCREEN_HEIGHT / GRID_HEIGHT);
    int x2 = x1 + (SCREEN_WIDTH / GRID_WIDTH);
    int y2 = y1 + (SCREEN_HEIGHT / GRID_HEIGHT);
    fill_rect_int(x1, y1, x2 - 1, y2 - 1, palette_index);
}

bool is_in_snake(int x, int y)
{
    for (int i = 0; i < snake_length; i++)
    {
        if (x == snake_body[i * 2 + 0] && y == snake_body[i * 2 + 1])
        {
            return true;
        }
    }
    return false;
}