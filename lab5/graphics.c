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
