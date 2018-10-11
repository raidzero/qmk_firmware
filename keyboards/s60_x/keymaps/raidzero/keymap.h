#include QMK_KEYBOARD_H
#include "raidzero.h"

/* almighty spaceFN */
#define SPCFN LT(2, KC_SPACE)
#define CAPS MT(MOD_LGUI, KC_CAPS)

/* mouse keys */
#define MS_UP     KC_MS_UP
#define MS_LEFT   KC_MS_LEFT
#define MS_DOWN   KC_MS_DOWN
#define MS_RGHT   KC_MS_RIGHT
#define MS_LCLK   KC_MS_BTN1
#define MS_RCLK   KC_MS_BTN2
#define MS_WHDN   KC_MS_WH_DOWN
#define MS_WHUP   KC_MS_WH_UP
#define MS_WHRT   KC_MS_WH_RIGHT
#define MS_WHLT   KC_MS_WH_LEFT

/* bit field for various keyboard state bools */
#define STATE_LOCKED (1 << 0)

/* state bit field helpers */
void flip_state_bit_on(uint8_t bit);
void flip_state_bit_off(uint8_t bit);
void flip_state_bit(uint8_t bit);
