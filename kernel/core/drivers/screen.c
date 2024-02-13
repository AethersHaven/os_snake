#include "screen.h"

#include "../mem.h"
#include "../math.h"

unsigned char *video_buffer;

void set_pixel(int x, int y, unsigned char palette_index);
int convert_to_screen_cords_x(float x);
int convert_to_screen_cords_y(float y);
void fill_flat_triangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char palette_index);

void init_screen()
{
    video_buffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT);
    clear_screen(0x00);
    flip();
}

void flip()
{
    memcpy(VIDEO_ADDRESS, video_buffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void clear_screen(unsigned char palette_index)
{
    memset8(video_buffer, palette_index, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void draw_line(float x1f, float y1f, float x2f, float y2f, unsigned char palette_index)
{
    int x1 = convert_to_screen_cords_x(x1f);
    int y1 = convert_to_screen_cords_y(y1f);
    int x2 = convert_to_screen_cords_x(x2f);
    int y2 = convert_to_screen_cords_y(y2f);

    if (x2 < x1)
    {
        int tmpX = x1;
        int tmpY = y1;
        x1 = x2;
        y1 = y2;
        x2 = tmpX;
        y2 = tmpY;
    }

    if (abs(x2 - x1) >= abs(y2 - y1))
    {
        int x = x1;
        int y = y1;
        set_pixel(x, y, palette_index);
        int A = 2 * abs(y2 - y1);
        int B = A - 2 * abs(x2 - x1);
        int P = A - abs(x2 - x1);
        while (x < x2)
        {
            x++;
            if (P < 0)
            {
                P += A;
            }
            else
            {
                y += sign(y2 - y1);
                P += B;
            }
            set_pixel(x, y, palette_index);
        }
    }
    else
    {
        if (y2 < y1)
        {
            int tmpX = x1;
            int tmpY = y1;
            x1 = x2;
            y1 = y2;
            x2 = tmpX;
            y2 = tmpY;
        }
        int x = x1;
        int y = y1;
        set_pixel(x, y, palette_index);
        int A = 2 * abs(x2 - x1);
        int B = A - 2 * abs(y2 - y1);
        int P = A - abs(y2 - y1);
        while (y < y2)
        {
            y++;
            if (P < 0)
            {
                P += A;
            }
            else
            {
                x += sign(x2 - x1);
                P += B;
            }
            set_pixel(x, y, palette_index);
        }
    }
}

void fill_rect(float x1, float y1, float x2, float y2, unsigned char palette_index)
{
    int xStart = x1 < x2 ? convert_to_screen_cords_x(x1) : convert_to_screen_cords_x(x2);
    int xEnd = x1 > x2 ? convert_to_screen_cords_x(x1) : convert_to_screen_cords_x(x2);
    int yStart = y1 > y2 ? convert_to_screen_cords_y(y1) : convert_to_screen_cords_y(y2);
    int yEnd = y1 < y2 ? convert_to_screen_cords_y(y1) : convert_to_screen_cords_y(y2);
    for (int x = xStart; x <= xEnd; x++)
    {
        for (int y = yStart; y <= yEnd; y++)
        {
            set_pixel(x, y, palette_index);
        }
    }
}

void fill_triangle(float x1f, float y1f, float x2f, float y2f, float x3f, float y3f, unsigned char palette_index)
{
    // Convert to screen cordinates
    int x1 = x1f * SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2;
    int y1 = -y1f * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2;
    int x2 = x2f * SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2;
    int y2 = -y2f * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2;
    int x3 = x3f * SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2;
    int y3 = -y3f * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2;

    // Sort vertices by ascending y
    if (y2 < y1)
    {
        int tmpX = x1;
        int tmpY = y1;
        x1 = x2;
        y1 = y2;
        x2 = tmpX;
        y2 = tmpY;
    }
    if (y3 < y1)
    {
        int tmpX = x1;
        int tmpY = y1;
        x1 = x3;
        y1 = y3;
        x3 = tmpX;
        y3 = tmpY;
    }
    if (y3 < y2)
    {
        int tmpX = x3;
        int tmpY = y3;
        x3 = x2;
        y3 = y2;
        x2 = tmpX;
        y2 = tmpY;
    }

    if (y2 == y3 || y1 == y2)
    {
        fill_flat_triangle(x1, y1, x2, y2, x3, y3, palette_index);
    }
    else
    {
        int x4 = (int)(x1 + ((float)(y2 - y1) / (float)(y3 - y1)) * (x3 - x1));
        int y4 = y2;
        fill_flat_triangle(x1, y1, x2, y2, x4, y4, palette_index);
        fill_flat_triangle(x2, y2, x4, y4, x3, y3, palette_index);
    }
}

void set_pixel(int x, int y, unsigned char palette_index)
{
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
    {
        video_buffer[y * SCREEN_WIDTH + x] = palette_index;
    }
}

int convert_to_screen_cords_x(float x)
{
    return x * SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2;
}

int convert_to_screen_cords_y(float y)
{
    return -y * SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2;
}

void fill_flat_triangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char palette_index)
{
    if (y1 == y2) // Top flat triangle
    {
        if (x2 < x1)
        {
            int tmpX = x1;
            int tmpY = y1;
            x1 = x2;
            y1 = y2;
            x2 = tmpX;
            y2 = tmpY;
        }

        float invslope1 = (float)(x3 - x1) / (y3 - y1);
        float invslope2 = (float)(x3 - x2) / (y3 - y2);

        float curx1 = x3;
        float curx2 = x3;

        for (int y = y3; y >= y1; y--)
        {
            if (y < 0)
            {
                break;
            }
            for (int x = curx1; x <= curx2; x++)
            {
                if (x >= SCREEN_WIDTH)
                {
                    break;
                }
                if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
                {
                    video_buffer[y * SCREEN_WIDTH + x] = palette_index;
                }
            }
            curx1 -= invslope1;
            curx2 -= invslope2;
        }
    }
    else // Bottom flat triangle
    {
        if (x3 < x2)
        {
            int tmpX = x3;
            int tmpY = y3;
            x3 = x2;
            y3 = y2;
            x2 = tmpX;
            y2 = tmpY;
        }

        float invslope1 = (float)(x2 - x1) / (y2 - y1);
        float invslope2 = (float)(x3 - x1) / (y3 - y1);

        float curx1 = x1;
        float curx2 = x1;

        for (int y = y1; y <= y2; y++)
        {
            if (y >= SCREEN_HEIGHT)
            {
                break;
            }
            for (int x = curx1; x <= curx2; x++)
            {
                if (x >= SCREEN_WIDTH)
                {
                    break;
                }
                if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
                {
                    video_buffer[y * SCREEN_WIDTH + x] = palette_index;
                }
            }
            curx1 += invslope1;
            curx2 += invslope2;
        }
    }
}