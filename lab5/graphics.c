#include "graphics.h"

vbe_mode_info_t mode_info;
uint8_t *frame_buffer = NULL;

int set_graphic_mode(uint16_t mode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));       // Inicializa todos os campos da struct a 0

    reg86.intno = 0x10;                     // Interrupção da BIOS para vídeo
    reg86.ah = 0x4F;                        // Comando para usar VBE
    reg86.al = 0x02;                        // Subfunção: set VBE mode
    reg86.bx = mode | BIT(14);              // Modo + ativar mapeamento linear

    if (sys_int86(&reg86) != 0) {
        printf("Error: sys_int86 failed in set_graphic_mode()\n");
        return 1;
    }

    return 0;
}

int set_frame_buffer(uint16_t mode) {
    // Obter info sobre o modo gráfico
    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info)) {
        printf("Failed to get VBE mode info\n");
        return 1;
    }

    // Calcular bytes por pixel (arredondamento por excesso)
    uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

    // Preencher estrutura com os endereços físicos da VRAM
    struct minix_mem_range mr;
    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + frame_size;

    // Pedir permissão para mapear o espaço físico
    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
        printf("Failed to add physical memory range\n");
        return 1;
    }

    // Mapear memória física para espaço virtual do processo
    frame_buffer = vm_map_phys(SELF, (void*) mr.mr_base, frame_size);
    if (frame_buffer == NULL) {
        printf("Failed to map video memory\n");
        return 1;
    }

    return 0;
}

int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height; i++) {
    for (uint16_t j = 0; j < width; j++) {
      if (vg_draw_pixel(x + j, y + i, color) != 0) return 1;
    }
  }
  return 0;
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  // Verificar se as coordenadas são válidas
  if (x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;

  // Calcular quantos bytes cada pixel ocupa (arredonda por excesso)
  unsigned bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;

  // Calcular índice no array frame_buffer
  unsigned int index = (y * mode_info.XResolution + x) * bytes_per_pixel;

  // Escrever a cor no frame buffer
  memcpy(&frame_buffer[index], &color, bytes_per_pixel);

  return 0;
}
