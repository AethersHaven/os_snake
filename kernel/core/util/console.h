#ifndef TERMINAL_H
#define TERMINAL_H

void init_console();
void draw_console();

void clear_console();
void print_newline();

void print_color(char* str, unsigned char palette_color);
void print(char* str);

void print_int_color(int number, unsigned char palette_color);
void print_int(int number);

void print_hex_color(unsigned number, unsigned char palette_color);
void print_hex(unsigned number);

void print_float_color(float number, unsigned char palette_color);
void print_float(float number);

#endif