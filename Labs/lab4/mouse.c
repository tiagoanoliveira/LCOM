#include "mouse.h"
#include "KBC.h"
#include <lcom/lcf.h>

int mouse_subscribe_int(uint8_t *bit_no){
    return 1;
}
int mouse_unsubscribe_int() {
    return 1;
}
void (mouse_ih)() {
    return 1;
}
int mouse_enable_data_reporting() {
    return 1;
}
int mouse_disable_data_reporting() {
    return 1;
}
int mouse_write_cmd(uint8_t cmd) {
    return 1;
}
int mouse_sync_bytes() {
    return 1;
}
void mouse_parse_packet(struct packet *pp) {

}
int mouse_detect_gesture(struct packet *pp, uint8_t x_len, uint8_t tolerance) {
    return 1;
}