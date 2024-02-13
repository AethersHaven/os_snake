#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS ((void*)0xA0000)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

void init_screen();

void flip();
void clear_screen(unsigned char palette_index);

void draw_line(float x1, float y1, float x2, float y2, unsigned char palette_index);
void fill_rect(float x1, float y1, float x2, float y2, unsigned char palette_index);
void fill_triangle(float x1, float y1, float x2, float y2, float x3, float y3, unsigned char palette_index);

void draw_character(unsigned int row, unsigned int col, unsigned char character, unsigned palette_index);

#endif