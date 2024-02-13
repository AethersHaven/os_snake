#include "console.h"

#include "../mem.h"
#include "../drivers/screen.h"

#define CONSOLE_ROWS (SCREEN_HEIGHT / 8)
#define CONSOLE_COLS (SCREEN_WIDTH / 8)

void set_character(unsigned int row, unsigned int col, char character, unsigned char palette_color);
void increment_cursor();

unsigned short *console;
unsigned int cursorRow, cursorCol;

void init_console()
{
    console = malloc(CONSOLE_ROWS * CONSOLE_COLS);
    clear_console();
}

void draw_console()
{
    for (unsigned int row = 0; row < CONSOLE_ROWS; row++)
    {
        for (unsigned int col = 0; col < CONSOLE_COLS; col++)
        {
            char character = console[row * CONSOLE_COLS + col] & 0x00FF;
            unsigned char palette_color = console[row * CONSOLE_COLS + col] >> 8;
            if (console[row * CONSOLE_COLS + col] != 0)
            {
                draw_character(row, col, character, palette_color);
            }
        }
    }
}

void clear_console()
{
    memset16(console, 0, CONSOLE_ROWS * CONSOLE_COLS);
    cursorRow = 0;
    cursorCol = 0;
}

void print_newline()
{
    cursorCol = CONSOLE_COLS;
    increment_cursor();
}

void print_color(char *str, unsigned char palette_color)
{
    for (unsigned int i = 0; str[i] != '\0'; i++)
    {
        char ch = str[i];
        if (ch == '\n')
        {
            print_newline();
            continue;
        }
        if (ch == '\t')
        {
            for (int j = 0; j < 5; j++)
            {
                increment_cursor();
            }
            continue;
        }
        set_character(cursorRow, cursorCol, ch, palette_color);
        increment_cursor();
    }
}

void print(char *str)
{
    print_color(str, 0x0F);
}

void print_int_color(int number, unsigned char palette_color)
{
    if (number < 0)
    {
        set_character(cursorRow, cursorCol, '-', palette_color);
        increment_cursor();
        number = -number;
    }
    unsigned int power = 1;
    while (number / power >= 10)
    {
        power *= 10;
    }
    while (power > 0)
    {
        char character = (number / power) + '0';
        number %= power;
        set_character(cursorRow, cursorCol, character, palette_color);
        increment_cursor();
        power /= 10;
    }
}

void print_int(int number)
{
    print_int_color(number, 0x0F);
}

void print_hex_color(unsigned number, unsigned char palette_color)
{
    if (number >= 16)
    {
        print_hex_color(number / 16, palette_color);
    }
    number %= 16;
    char character = number <= 9 ? '0' + number : 'A' + number - 10;
    set_character(cursorRow, cursorCol, character, palette_color);
    increment_cursor();
}

void print_hex(unsigned number)
{
    print_hex_color(number, 0x0F);
}

void print_float_color(float number, unsigned char palette_color)
{
    print_int_color((int)number, palette_color);

    set_character(cursorRow, cursorCol, '.', palette_color);
    increment_cursor();

    number = number < 0 ? -number : number;
    int decimal = (int)(number * 1000) % 1000;
    if ((int)(number * 10000) % 10 >= 5)
    {
        decimal++;
    }

    print_int_color(decimal, palette_color);
}

void print_float(float number)
{
    print_float_color(number, 0x0F);
}

void set_character(unsigned int row, unsigned int col, char character, unsigned char palette_color)
{
    if (row >= 0 && row < CONSOLE_ROWS && col >= 0 && col < CONSOLE_COLS)
    {
        unsigned short palette_mask = (unsigned short)palette_color << 8;
        console[row * CONSOLE_COLS + col] = palette_mask | character;
    }
}

void debug() {}

void increment_cursor()
{
    int col = cursorCol;
    int row = cursorRow;
    cursorCol++;
    if (cursorCol >= CONSOLE_COLS)
    {
        cursorCol = 0;
        cursorRow++;
        debug();
        if (cursorRow >= CONSOLE_ROWS)
        {
            memcpy(console, console + CONSOLE_COLS, (CONSOLE_ROWS - 1) * CONSOLE_COLS * 2);
            memset16(console + (CONSOLE_ROWS - 1) * CONSOLE_COLS, 0, CONSOLE_COLS);
            cursorRow--;
        }
    }
}