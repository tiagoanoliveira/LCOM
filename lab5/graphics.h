#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <lcom/vbe.h>

#include <stdint.h>
#include <string.h>
#include <sys/mman.h>     // vm_map_phys
#include <machine/int86.h> // sys_int86
#include <minix/syslib.h>  // sys_privctl
#include <minix/sysutil.h> // printf

// Apontador global para o frame buffer (zona de memória gráfica)
extern uint8_t *frame_buffer;
// Informação do modo gráfico atual
extern vbe_mode_info_t mode_info;

int set_graphic_mode(uint16_t mode);
int set_frame_buffer(uint16_t mode);
int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

#endif // GRAPHICS_H
