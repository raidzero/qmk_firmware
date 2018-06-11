#include QMK_KEYBOARD_H
#include "keymap.h"
extern rgblight_config_t rgblight_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* 0: ANSI qwerty  (spacefn) */
  LAYOUT(
    KC_GESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,  x_____x,   KC_BSPC, \
    KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC, KC_BSLS,       \
    KC_LGUI,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,  x_____x,   KC_ENT ,       \
    KC_LSFT,   x_____x,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT, x_____x, \
    KC_LCTL,   KC_LGUI, KC_LALT,                   SPCFN ,                    KC_RALT, MO(2),  MO(3),  KC_RCTL),

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
    _______, x_____x, MPR, MPP, MPN, VUP, VDN, x_____x, x_____x, x_____x, x_____x, x_____x,           _______, x_____x, \
    _______, _______, _______,                   TG(1),                   _______, x_____x, x_____x, _______),

/* 3: lighting layer (FN1) */
  LAYOUT(
    x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, \
    x_____x, RGB_ST, RGB_BR, RGB_RB, RGB_SW, RGB_SN, RGB_KN, RGB_TE, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x,       \
    _______, RGB_RCT, x_____x, RGB_VAI, RGB_VAD, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x,       \
    _______, x_____x, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, BL_BRTG, BL_DEC,  BL_TOGG, BL_INC,           _______, x_____x, \
    _______, _______, _______,                   x_____x,                   _______, x_____x, x_____x, _______),
};

// a place to keep references to all the RGB LEDs
static rgbled rgbs[RGBLED_NUM];
static bool rgb_react_enabled = false;

void matrix_init_user(void) {
  breathing_disable();

  // if backlight was off, turn it on. (otherwise just leave it as it was)
  if (get_backlight_level() == 0) {
    backlight_level(BACKLIGHT_LEVELS); // max backlight
  }

  // set saturation & value to max values in case they were turned down
  rgblight_config.sat=255;
  rgblight_config.val=255;

  // init rgbs array
  for (int i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];
    led->on = false;
  }
}

void matrix_scan_user(void) {
  if (rgb_react_enabled) {
    fadeLeds();
    set_leds();
  }
}

uint8_t orig_rgb_mode = 255;

uint8_t orig_hue = 0;
uint8_t orig_sat = 0;
uint8_t orig_val = 0;

bool was_rgb_enabled = true;

bool lightRandomLeds = true;
bool lightAllLeds = false;
bool fadeOut = true;
bool allLedsOff = false;


uint16_t timer = 0;

/* handle special chords */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  if (rgb_react_enabled) {
    if (record->event.pressed) {
      if (lightRandomLeds) {
        light_leds_random_color(rand() % RGBLED_NUM);
      } else if (lightAllLeds) {
        light_all_leds(rand() % 360);
      }
    } else {
    /*
      if (!was_rgb_enabled) {
        rgblight_disable();
      } else {
        rgblight_sethsv(orig_hue, orig_sat, orig_val);
        rgblight_mode(orig_rgb_mode);
      }
      */
    }

  }

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
        // save original RGB mode
        orig_rgb_mode = rgblight_config.mode;
        was_rgb_enabled = rgblight_config.enable;

        orig_hue = rgblight_config.hue;
        orig_sat = rgblight_config.sat;
        orig_val = rgblight_config.val;

        rgb_react_enabled = !rgb_react_enabled;

        if (!rgb_react_enabled) {

          if (!was_rgb_enabled) {
            rgblight_disable();
          } else {
            // set original mode back when disabling reactive mode
            rgblight_mode(orig_rgb_mode);
          }
        }

        return false;
      }
      break;
  }

  return true;
}

void light_leds_random_color(uint8_t numberOfLeds) {
  rgblight_enable();
  rgblight_mode(1);

  for (int i = 0; i < numberOfLeds; i++) {
    uint8_t ledIndex = rand() % RGBLED_NUM;
    rgbled* led = &rgbs[ledIndex];

    int hue = rand() & 360;
    rgblight_sethsv_at(hue, 255, 255, ledIndex);

    timer = timer_read();

    led->index = ledIndex;
    led->h = hue;
    led->s = 255;
    led->v = 255;
  }
}

void light_led_random_color(uint8_t ledIndex) {
  rgblight_enable();
  rgblight_mode(1);

  rgblight_sethsv_at(rand() % 360, 255, 255, ledIndex);
}

void light_all_leds(int hue) {
  rgblight_enable();
  rgblight_mode(1);

  rgblight_sethsv(hue, 255, 255);
}

void fadeLeds(void) {
  if (timer_elapsed(timer) > 10) {
    for (int i = 0; i < RGBLED_NUM; i++) {
      rgbled* led = &rgbs[i];

      if (led->v > 9) {
        led->v -= 10;
      } else {
        led->v = 0;
      }

      timer = timer_read();
    }
  }
}

void set_leds(void) {
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    rgblight_sethsv_at(led->h, led->s, led->v, led->index);

    allLedsOff = !led->v;
  }

  /*if (allLedsOff) {
    rgblight_mode(orig_rgb_mode);
  }
  */
}
