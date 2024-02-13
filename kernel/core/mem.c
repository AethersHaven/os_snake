#include "mem.h"

void set_header(unsigned int *header_address, unsigned int free, unsigned int chunk_size, unsigned int prev_chunk_size);
unsigned int is_header_free(unsigned int *header_address);
unsigned int get_header_size(unsigned int *header_address);
unsigned int get_header_previous_size(unsigned int *header_address);
void handle_error();

void init_heap()
{
    set_header(HEAP_START, true, (HEAP_SIZE >> 2) - 1, 0U);
}

void *malloc(size_t nbytes)
{
    if (nbytes == 0)
    {
        handle_error();
    }

    size_t requested_blocks = (nbytes % 4 == 0) ? (nbytes / 4) : (nbytes / 4 + 1);

    unsigned int *header_address = HEAP_START;
    unsigned int previous_header_size = 0;
    while (header_address < (unsigned int *)HEAP_START + HEAP_SIZE)
    {
        unsigned int header_size = get_header_size(header_address);
        if (is_header_free(header_address) && header_size >= requested_blocks)
        {
            if (header_size > requested_blocks)
            {
                unsigned int *next_header_address = header_address + 2 + requested_blocks;
                set_header(next_header_address, true, header_size - requested_blocks - 2, requested_blocks);
            }
            set_header(header_address, false, requested_blocks, previous_header_size);
            return header_address + 2;
        }

        previous_header_size = get_header_previous_size(header_address);
        header_address += 2 + header_size;
    }

    handle_error();
}

void free(void *address)
{
    if (address == NULL || is_header_free((unsigned int *)address - 2))
    {
        handle_error();
    }

    unsigned int *header_address = (unsigned int *)address - 2;
    unsigned int size = get_header_size(header_address);
    unsigned int prev_size = get_header_previous_size(header_address);
    set_header(header_address, true, size, prev_size);

    unsigned int *next_header_address = header_address + 2 + size;
    while (next_header_address < (unsigned int *)HEAP_START + HEAP_SIZE && is_header_free(next_header_address))
    {
        size += 2 + get_header_size(next_header_address);
        set_header(header_address, true, size, prev_size);
        next_header_address += 2 + get_header_size(next_header_address);
    }

    unsigned int *prev_header_address = header_address - 2 - prev_size;
    while (prev_header_address >= HEAP_START && is_header_free(prev_header_address))
    {
        size += prev_size + 2;
        set_header(prev_header_address, true, size, get_header_previous_size(prev_header_address));
        prev_header_address -= 2 + get_header_previous_size(prev_header_address);
    }

    *(next_header_address + 1) = size;
}

void *memcpy(void *dest, void *src, size_t nbytes)
{
    char *dest_ptr = dest;
    char *src_ptr = src;
    for (unsigned int i = 0; i < nbytes; i++)
    {
        *(dest_ptr + i) = *(src_ptr + i);
    }
    return dest;
}

void *memset8(void *dest, char value, size_t count)
{
    char *dest_ptr = dest;
    for (unsigned int i = 0; i < count; i++)
    {
        *(dest_ptr + i) = value;
    }
    return dest;
}

void *memset16(void *dest, short value, size_t count)
{
    short *dest_ptr = dest;
    for (unsigned int i = 0; i < count; i++)
    {
        *(dest_ptr + i) = value;
    }
    return dest;
}

void *memset32(void *dest, int value, size_t count)
{
    int *dest_ptr = dest;
    for (unsigned int i = 0; i < count; i++)
    {
        *(dest_ptr + i) = value;
    }
    return dest;
}

void set_header(unsigned int *header_address, unsigned int free, unsigned int chunk_size, unsigned int prev_chunk_size)
{
    free = free << 31;
    chunk_size = (chunk_size << 1) >> 1;
    *header_address = free | chunk_size;
    *(header_address + 1) = prev_chunk_size;
}

unsigned int is_header_free(unsigned int *header_address)
{
    return *header_address >> 31;
}

unsigned int get_header_size(unsigned int *header_address)
{
    return (*header_address << 1) >> 1;
}

unsigned int get_header_previous_size(unsigned int *header_address)
{
    return *(header_address + 1);
}

void handle_error()
{
    while (true) // Hang for now
    {
    }
}