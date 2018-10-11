#include QMK_KEYBOARD_H

#include "keymap.h"
#include "underglow.h"
#include "animations.h"

extern rgblight_config_t rgblight_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* 0: ANSI qwerty */
  LAYOUT(
    KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,  x_____x,   KC_BSPC, \
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC, KC_BSLS,             \
    KC_LGUI, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,  KC_ENT,          \
    KC_LSFT, x_____x, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT, x_____x,     \
    KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  KC_SPC ,                   MO(1),   KC_LEFT, KC_DOWN, KC_UP,    KC_RIGHT),

/* 1: Fn layer (FN1) */
  LAYOUT(
    KC_GRV,  KC_F1,   KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  x_____x,  KC_DEL, \
    x_____x, x_____x, x_____x,  x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, KC_PSCR, KC_SLCK, KC_PAUS, x_____x,       \
    KC_CAPS, x_____x, x_____x,  x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, TO(2),       \
    _______, _______, MPR,      MPP,     MPN,     x_____x, x_____x, x_____x, x_____x, VDN,     VUP,     x_____x, x_____x, x_____x, \
    _______, _______, _______,  x_____x, x_____x, x_____x,                   x_____x, x_____x, x_____x, x_____x, x_____x),

/* 2: Lighting layer (FN2) */
// ESC to go back to default layer
  LAYOUT(
    TO(0),   x_____x, x_____x,  x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, RGB_WHT, ANI_SPD, ANI_SPI, x_____x,  x_____x, \
    x_____x, ANI_STA, ANI_BRE,  ANI_SPC, ANI_RNB, ANI_SWI, ANI_PUL, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x,       \
    x_____x, RGB_RCT, x_____x,  x_____x, RGB_FAD, RGB_ALL, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x,       \
    x_____x, x_____x, RGB_TOG,  RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, x_____x, x_____x, x_____x, x_____x, x_____x, \
    x_____x, x_____x, x_____x,  x_____x, x_____x, x_____x,                   x_____x, x_____x, BL_DEC,  BL_TOGG, BL_INC),

};


/* holds keyboard state bit field */
uint8_t STATE_FLAGS = 0;

void matrix_init_user(void) {
  //breathing_disable();

  // if backlight was off, turn it on. (otherwise just leave it as it was)
  if (get_backlight_level() == 0) {
    backlight_level(BACKLIGHT_LEVELS); // max backlight
  }

  // disable caps lock as indicator ?
  // PORTB &= ~(1 << 2);

  // set saturation & value to max values in case they were turned down
  rgblight_config.sat=255;
  rgblight_config.val=255;

  init_leds();
  init_animation();
}

void matrix_scan_user(void) {
  // process custom underglow stuff
  scan_leds();
}

/* handle special chords */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return process_key_rz(keycode, record);
}

void flip_state_bit(uint8_t bit) {
  if (STATE_FLAGS & bit) {
    flip_state_bit_off(bit);
  } else {
    flip_state_bit_on(bit);
  }
}

void flip_state_bit_on(uint8_t bit) {
  STATE_FLAGS |= bit;
}

void flip_state_bit_off(uint8_t bit) {
  STATE_FLAGS &= ~bit;
}
