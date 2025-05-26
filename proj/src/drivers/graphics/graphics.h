#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <lcom/vbe.h>

#include <stdint.h>
#include <string.h>
#include <sys/mman.h>       // vm_map_phys
#include <machine/int86.h>  // sys_int86
#include <minix/syslib.h>   // sys_privctl
#include <minix/sysutil.h>  // printf

static uint16_t h_res;
static uint16_t v_res;
static uint8_t bits_per_pixel;
static uint32_t vram_base;
static uint32_t vram_size;

// Global variables for grid dimensions
extern int grid_origin_x;
extern int grid_origin_y;

// Global pointer to the frame buffer (graphics memory area)
extern uint8_t *frame_buffer;
extern uint8_t *video_mem;
// Current graphic mode information
extern vbe_mode_info_t mode_info;

extern int grid_origin_x;
extern int grid_origin_y;

// Define the setup function
void set_grid_position(int grid_cols, int grid_rows, int cell_width, int cell_height);

void draw_grid();
void swap_buffers();
void* my_vg_init(uint16_t mode);
int vg_clear_screen(uint32_t color);
int set_graphic_mode(uint16_t mode);
int set_frame_buffer(uint16_t mode);
int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void draw_piece(uint16_t grid_col, uint16_t grid_row, uint32_t color);
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

#endif // GRAPHICS_H
