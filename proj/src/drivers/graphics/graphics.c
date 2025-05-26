#include "graphics.h"

vbe_mode_info_t mode_info;
uint8_t *video_mem = NULL;
uint8_t *frame_buffer = NULL;
int grid_origin_x = 0;
int grid_origin_y = 0;

void set_grid_position(int grid_cols, int grid_rows, int cell_w, int cell_h) {
    grid_origin_x = (h_res - grid_cols * cell_w) / 2;
    grid_origin_y = (v_res - grid_rows * cell_h) / 2;
}

void swap_buffers() {
    if (video_mem == NULL || frame_buffer == NULL) {
        printf("Error: video_mem or frame_buffer not initialized.\n");
        return;
    }

    memcpy(video_mem, frame_buffer, vram_size);
}

void* my_vg_init(uint16_t mode) {
    if (set_frame_buffer(mode) != 0) {
        printf("Error while configuring frame buffer.\n");
        return NULL;
    }

    if (set_graphic_mode(mode) != 0) {
        printf("Error while changing to graphic mode.\n");
        return NULL;
    }

    // Update global variables
    h_res = mode_info.XResolution;
    v_res = mode_info.YResolution;
    bits_per_pixel = mode_info.BitsPerPixel;
    vram_base = mode_info.PhysBasePtr;
    vram_size = h_res * v_res * ((bits_per_pixel + 7) / 8);

    frame_buffer = malloc(vram_size);
    if (frame_buffer == NULL) {
        printf("Error while allocating frame buffer\n");
        return NULL;
    }

    return video_mem;
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    // Check if coordinates are valid
    if (x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;

    // Calculate how many bytes each pixel occupies (round up)
    unsigned bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;

    // Calculate index in frame_buffer array
    unsigned int index = (y * mode_info.XResolution + x) * bytes_per_pixel;

    // Write color to frame buffer
    memcpy(&frame_buffer[index], &color, bytes_per_pixel);

    return 0;
}

int vg_clear_screen(uint32_t color) {
    if (frame_buffer == NULL) return 1;

    unsigned bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned total_pixels = mode_info.XResolution * mode_info.YResolution;

    for (unsigned i = 0; i < total_pixels; ++i) {
        memcpy(&frame_buffer[i * bytes_per_pixel], &color, bytes_per_pixel);
    }

    return 0;
}

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
    // Obtain info about the graphics mode
    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info)) {
        printf("Failed to get VBE mode info\n");
        return 1;
    }

    // Calculate bytes per pixel (round up)
    uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

    // Fill structure with physical addresses of VRAM
    struct minix_mem_range mr;
    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + frame_size;

    // Request permission to map physical space
    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
        printf("Failed to add physical memory range\n");
        return 1;
    }

    // Map physical memory to process's virtual space
    video_mem = vm_map_phys(SELF, (void*) mr.mr_base, frame_size);
    if (video_mem == NULL) {
        printf("Failed to map video memory\n");
        return 1;
    }

    return 0;
}
