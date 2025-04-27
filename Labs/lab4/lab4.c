#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "i8042.h"
#include "timer.h"

extern struct packet mouse_packet;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int ipc_status;         // Status das interrupções
message msg;            // Mensagem da comunicação entre processos

int (mouse_test_packet)(uint32_t cnt) {
  // Subscreve as interrupções do rato
  if (mouse_subscribe_int(&mouse_hook_id) != 0) {
    printf("Error subscribing mouse interrupts\n");
    return 1;
  }
  // Habilita o modo de dados do rato
  if (mouse_enable_data_reporting() != 0) {
    printf("Error enabling data reporting\n");
    return 1;
  }

  // Loop para receber pacotes
  while (cnt > 0) {
    // Espera por uma interrupção do rato
    int r;
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed: %d\n", r);
      continue;
    }

    // Verifica se a mensagem é para o rato
    if (is_ipc_notify(ipc_status)) {
      if (msg.m_notify.interrupts & BIT(mouse_hook_id)) {
        mouse_ih(); // Chama o manipulador de interrupções do rato
        mouse_sync_bytes(); // Sincroniza os bytes recebidos

        // Verifica se temos um pacote completo
        if (byte_index == 3) {
          mouse_bytes_to_packet(); // Converte os bytes para a struct
          mouse_print_packet(&mouse_packet); // Exibe o pacote
          cnt--; // Decrementa o contador de pacotes
        }
      }
    }
  }
  // Desinscreve as interrupções do rato
  mouse_unsubscribe_int();
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  // Subscreve as interrupções do rato
  if (mouse_subscribe_int(&mouse_hook_id) != 0) {
    printf("Error subscribing mouse interrupts\n");
    return 1;
  }

  // Subscreve as interrupções do temporizador
  if (timer_subscribe_int(&timer_hook_id) != 0) {
    printf("Error subscribing timer interrupts\n");
    return 1;
  }

  // Habilita o modo de dados do rato
  if (mouse_enable_data_reporting() != 0) {
    printf("Error enabling data reporting\n");
    return 1;
  }

  // Contador de tempo de inatividade
  uint8_t timer_counter = 0;

  // Loop para receber pacotes até o tempo de inatividade
  while (timer_counter < idle_time) {
    int r;
    message msg;

    // Espera por uma interrupção
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed: %d\n", r);
      continue;
    }

    // Verifica se a mensagem é para o rato ou temporizador
    if (is_ipc_notify(ipc_status)) {
      if (msg.m_notify.interrupts & BIT(mouse_hook_id)) {
        mouse_ih(); // Chama o manipulador de interrupções do rato
        mouse_sync_bytes(); // Sincroniza os bytes recebidos

        // Verifica se temos um pacote completo
        if (byte_index == 3) {
          mouse_bytes_to_packet(); // Converte os bytes para a struct
          mouse_print_packet(&mouse_packet); // Exibe o pacote
        }
      }

      if (msg.m_notify.interrupts & BIT(timer_hook_id)) {
        timer_counter++; // Incrementa o contador de tempo de inatividade
        timer_int_handler(); // Chama o manipulador de interrupções do temporizador
      }
    }
  }

  // Desinscreve as interrupções do rato e do temporizador
  mouse_unsubscribe_int();
  timer_unsubscribe_int();

  return 0;
}

int mouse_test_gesture() {
  // Subscreve as interrupções do rato
  if (mouse_subscribe_int(&mouse_hook_id) != 0) {
    printf("Error subscribing mouse interrupts\n");
    return 1;
  }

  // Habilita o modo de dados do rato
  if (mouse_enable_data_reporting() != 0) {
    printf("Error enabling data reporting\n");
    return 1;
  }

  // Variáveis para rastrear o movimento
  int delta_x = 0, delta_y = 0;

  // Loop para receber pacotes
  while (1) {
    int r;
    message msg;

    // Espera por uma interrupção
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed: %d\n", r);
      continue;
    }

    // Verifica se a mensagem é para o rato
    if (is_ipc_notify(ipc_status)) {
      if (msg.m_notify.interrupts & BIT(mouse_hook_id)) {
        mouse_ih(); // Chama o manipulador de interrupções do rato
        mouse_sync_bytes(); // Sincroniza os bytes recebidos

        // Verifica se temos um pacote completo
        if (byte_index == 3) {
          mouse_bytes_to_packet(); // Converte os bytes para a struct

          // Atualiza os deslocamentos
          delta_x += mouse_packet.delta_x;
          delta_y += mouse_packet.delta_y;

          // Detecta um gesto simples (por exemplo, um movimento significativo)
          if (abs(delta_x) > 50 || abs(delta_y) > 50) {
            printf("Gesture detected: delta_x = %d, delta_y = %d\n", delta_x, delta_y);
            // Reseta os deslocamentos após detectar um gesto
            delta_x = 0;
            delta_y = 0;
          }
        }
      }
    }
  }

  // Desinscreve as interrupções do rato
  mouse_unsubscribe_int();

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  /* This year you need not implement this. */
  printf("%s(%u, %u): under construction\n", __func__, period, cnt);
  return 1;
}
