#include "KBC.h"
#include "i8042.h"

// Ler o registo de estado do KBC
int kbc_read_status(uint8_t *status) {
    /* To be completed by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

// Ler dados do buffer de saída do KBC (com verificação de erros)
int kbc_read_output(uint8_t port, uint8_t *data) {
    /* To be completed by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

// Escreve um comando no KBC
int kbc_write_command(uint8_t port, uint8_t cmd) {
    /* To be completed by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}
