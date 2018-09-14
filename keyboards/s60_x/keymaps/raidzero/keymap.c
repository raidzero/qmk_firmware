#include QMK_KEYBOARD_H

#include "keymap.h"
#include "underglow.h"
#include "animations.h"

extern rgblight_config_t rgblight_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* 0: ANSI qwerty  (spacefn) */
  LAYOUT(
    KC_GESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,  x_____x,   KC_BSPC, \
    KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC, KC_BSLS,       \
    KC_LGUI,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,  x_____x,   KC_ENT ,       \
    KC_LSFT,   x_____x,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT, x_____x, \
    KC_LCTL,   KC_LGUI, KC_LALT,                   SPCFN ,                    KC_RALT, MO(2),  MO(3),  MO(2)),

/* 1: ASNI qwerty, no spacefn */
  LAYOUT(
    _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,  _______,  _______,  _______,   _______, \
    _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,  _______,  _______,  _______,       \
    _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,  _______,  _______,  _______ ,       \
    _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,  _______,  _______, _______, \
    _______,   _______, _______,                   KC_SPC,                    _______, MO(2),  MO(3),  _______),

/* 2: Fn layer (FN0) */
  LAYOUT(
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  x_____x, KC_DEL, \
    x_____x, MS_LCLK, MS_UP, MS_RCLK, x_____x, MS_WHUP, x_____x, x_____x, KC_UP,   x_____x, KC_PSCR, KC_SLCK, KC_PAUS, x_____x,       \
    KC_CAPS, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLT, MS_WHDN, MS_WHRT, KC_LEFT, KC_DOWN, KC_RIGHT, x_____x, x_____x, x_____x, x_____x,       \
    _______, x_____x, MPR, MPP, MPN, VUP, VDN, x_____x, x_____x, x_____x, x_____x, KC_UP,           _______, x_____x, \
    _______, _______, _______,                   TG(1),                   KC_LEFT, KC_DOWN, KC_RIGHT, _______),

/* 3: lighting layer (FN1) */
  LAYOUT(
    x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, ANI_SPD, ANI_SPI, x_____x, x_____x, \
    x_____x, ANI_STA, ANI_BRE, ANI_SPC, ANI_RNB, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x,       \
    _______, RGB_RCT, RGB_VAI, RGB_VAD, RGB_FAD, RGB_ALL, x_____x, x_____x, x_____x, KC_LOCK, x_____x, x_____x, x_____x, x_____x,       \
    _______, x_____x, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, BL_BRTG, BL_DEC,  BL_TOGG, BL_INC,           _______, x_____x, \
    _______, _______, _______,                   x_____x,                   _______, x_____x, x_____x, _______),
};


/* holds keyboard state bit field */
uint8_t STATE_FLAGS = 0;

void matrix_init_user(void) {
  breathing_disable();

  // if backlight was off, turn it on. (otherwise just leave it as it was)
  if (get_backlight_level() == 0) {
    backlight_level(BACKLIGHT_LEVELS); // max backlight
  }

  // set saturation & value to max values in case they were turned down
  rgblight_config.sat=255;
  rgblight_config.val=255;

  init_leds();
}

void matrix_scan_user(void) {
  // process custom underglow stuff
  scan_leds();
}

/* handle special chords */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // make leds do cool stuff when a key is hit
  process_leds(record);

  switch (keycode) {
    case KC_MINS: // RESET EEPROM
      if (RSHIFT_HELD && LSHIFT_HELD && record->event.pressed) {
        rgblight_disable(); // turn off rgb for some visual feedback
        backlight_level(0); // more visual feedback. no backlight
        eeconfig_init();
        return false;
      }
      break;
    case KC_EQL: // BOOTLOADER
      if (RSHIFT_HELD && LSHIFT_HELD && record->event.pressed) {
        backlight_level(0); // disable backlight for visual feedback
        reset_keyboard();
        return false;
      }

      break;
   case RGB_RCT:
      if (record->event.pressed) {
        process_reactive_toggle();
        return false;
      }
      break;
    case RGB_FAD:
      if (record->event.pressed) {
        process_reactive_fade();
        return false;
      }
      break;
    case RGB_ALL:
      if (record->event.pressed) {
        process_reactive_all();
        return false;
      }
      break;
    case RGB_TOG:
      if (record->event.pressed) {
        process_rgb_toggle();
      }
      return true;
    case ANI_SPI:
      if (record->event.pressed) {
        speed_increase();
        return false;
      }
      break;
    case ANI_SPD:
      if (record->event.pressed) {
        speed_decrease();
        return false;
      }
    case ANI_STA:
      if (record->event.pressed) {
        animation_mode(ANIMATION_MODE_STATIC);
        return false;
      }
      break;
    case ANI_BRE:
      if (record->event.pressed) {
        animation_mode(ANIMATION_MODE_BREATHE);
        return false;
      }
      break;
    case ANI_SPC:
      if (record->event.pressed) {
        animation_mode(ANIMATION_MODE_SPECTRUM);
        return false;
      }
      break;
    case ANI_RNB:
      if (record->event.pressed) {
        animation_mode(ANIMATION_MODE_RAINBOW);
        return false;
      }
      break;
  }

  return true;
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
