#include QMK_KEYBOARD_H
#include "raidzero.h"

/* bit field for various keyboard state bools */
#define STATE_LOCKED (1 << 0)

/* state bit field helpers */
void flip_state_bit_on(uint8_t bit);
void flip_state_bit_off(uint8_t bit);
void flip_state_bit(uint8_t bit);
