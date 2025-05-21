// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "graphics.h"
#include "timer.h"
#include "i8042.h"
#include "keyboard.h"
#include <lcom/xpm.h>
//#include "pixmap.h"

extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    //Mapeamento do frame buffer
    if (set_frame_buffer(mode) != 0) {
        printf("Erro ao mapear o frame buffer.\n");
        return 1;
    }

    // Ativar o modo gráfico
    if (set_graphic_mode(mode) != 0) {
        printf("Erro ao ativar o modo gráfico.\n");
        return 1;
    }

    // Aguardar 'delay' segundos
    tickdelay(micros_to_ticks(delay * 1000000));

    // Voltar ao modo de texto
    if (vg_exit() != 0) {
        printf("Erro ao retornar ao modo de texto.\n");
        return 1;
    }

    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  if (set_frame_buffer(mode)) return 1;
  if (set_graphic_mode(mode)) return 1;

  if (draw_rectangle(x, y, width, height, color)) {
    printf("Erro ao desenhar retângulo\n");
    vg_exit();
    return 1;
  }

  int ipc_status;
  message msg;
  uint8_t irq_set = BIT(KEYBOARD_IRQ);
  int r;
  scancode = 0;

  if (kbd_subscribe_int(&irq_set)) return 1;

  while (scancode != ESC_BREAK_CODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            //scancode = get_scancode();
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbd_unsubscribe_int()) return 1;
  if (vg_exit()) return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (set_frame_buffer(mode)) return 1;
  if (set_graphic_mode(mode)) return 1;


  uint16_t width = mode_info.XResolution / no_rectangles;
  uint16_t height = mode_info.YResolution / no_rectangles;

  uint32_t color;

  for (uint8_t row = 0; row < no_rectangles; row++) {
    for (uint8_t col = 0; col < no_rectangles; col++) {

      if (mode_info.MemoryModel == 0x04 || mode_info.MemoryModel == 0x03) { // Indexed
        color = (first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel);

      } else if (mode_info.MemoryModel == 0x06) { // Direct color
        uint8_t red_mask_size = mode_info.RedMaskSize;
        uint8_t green_mask_size = mode_info.GreenMaskSize;
        uint8_t blue_mask_size = mode_info.BlueMaskSize;

        uint8_t red_pos = mode_info.RedFieldPosition;
        uint8_t green_pos = mode_info.GreenFieldPosition;
        uint8_t blue_pos = mode_info.BlueFieldPosition;

        uint32_t red = (first >> red_pos) & ((1 << red_mask_size) - 1);
        uint32_t green = (first >> green_pos) & ((1 << green_mask_size) - 1);
        uint32_t blue = (first >> blue_pos) & ((1 << blue_mask_size) - 1);


        red = (red + col * step) % (1 << red_mask_size);
        green = (green + row * step) % (1 << green_mask_size);
        blue = (blue + (row + col) * step) % (1 << blue_mask_size);

        color = (red << red_pos) | (green << green_pos) | (blue << blue_pos);

      } else {
        printf("Unsupported memory model\n");
        return 1;
      }

      if (draw_rectangle(col * width, row * height, width, height, color)) return 1;
    }
  }

  // Esperar pela tecla ESC
  if (wait_for_esc()) return 1;

  return vg_exit();
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (set_frame_buffer(0x105)) return 1;
  if (set_graphic_mode(0x105)) return 1;

  xpm_image_t img;
  uint8_t *sprite = xpm_load(xpm, XPM_INDEXED, &img);
  if (sprite == NULL) {
    printf("Erro ao carregar o XPM\n");
    vg_exit();
    return 1;
  }

  // Desenhar a imagem pixel a pixel
  for (uint16_t i = 0; i < img.height; i++) {
    for (uint16_t j = 0; j < img.width; j++) {
      uint32_t color = sprite[i * img.width + j]; // Cada pixel é um índice (8 bits)
      if (paint_pixel(x + j, y + i, color)) {
        printf("Erro ao desenhar pixel (%d, %d)\n", x + j, y + i);
        vg_exit();
        return 1;
      }
    }
  }

  // Esperar pelo ESC
  uint8_t irq_set = BIT(KEYBOARD_IRQ);
  int ipc_status, r;
  message msg;
  scancode = 0;

  if (kbd_subscribe_int(&irq_set)) return 1;

  while (scancode != ESC_BREAK_CODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbd_unsubscribe_int()) return 1;
  if (vg_exit()) return 1;

  return 0;
}


int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}
