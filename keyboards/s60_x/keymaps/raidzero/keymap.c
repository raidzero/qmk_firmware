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
    _______, RGB_RCT, RGB_VAI, RGB_VAD, RGB_FAD, RGB_ALL, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x, x_____x,       \
    _______, x_____x, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, BL_BRTG, BL_DEC,  BL_TOGG, BL_INC,           _______, x_____x, \
    _______, _______, _______,                   x_____x,                   _______, x_____x, x_____x, _______),
};

// this controls all the reactive rgb bools (start with reactive mode enabled)
uint8_t RGB_FLAGS = LIGHT_RANDOM_LEDS;

// a place to keep references to all the RGB LEDs
static rgbled rgbs[RGBLED_NUM];

// a timer for controlling RGB fadeout
uint16_t rgb_fade_timer = 0;

void matrix_init_user(void) {
  breathing_disable();

  // if backlight was off, turn it on. (otherwise just leave it as it was)
  if (get_backlight_level() == 0) {
    backlight_level(BACKLIGHT_LEVELS); // max backlight
  }

  // set saturation & value to max values in case they were turned down
  rgblight_config.sat=255;
  rgblight_config.val=255;
}

void matrix_scan_user(void) {
  if (RGB_FLAGS & RGB_REACTIVE_ENABLED) {
    set_leds();

    if (RGB_FLAGS & RGB_FADE_OUT) {
      fadeLeds();
    }
  }
}

uint8_t orig_rgb_mode = 255;

/* handle special chords */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  if (RGB_FLAGS & RGB_REACTIVE_ENABLED) {
    if (record->event.pressed) {
      if (RGB_FLAGS & LIGHT_ALL_LEDS) {
        light_all_leds(rand() % 360);
      } else if (RGB_FLAGS & LIGHT_RANDOM_LEDS) {
        light_leds_random_color(rand() % RGBLED_NUM);
      }
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
        if (rgblight_config.enable) {
          flip_rgb_bit_on(RGB_WAS_ENABLED);
        } else {
          flip_rgb_bit_off(RGB_WAS_ENABLED);
        }

        flip_rgb_bit(RGB_REACTIVE_ENABLED);

        if (RGB_FLAGS & RGB_REACTIVE_ENABLED) {
          if (!(RGB_FLAGS & RGB_WAS_ENABLED)) {
            rgblight_disable();
          } else {
            // set original mode back when disabling reactive mode
            rgblight_mode(orig_rgb_mode);
          }
        }

        return false;
      }
      break;
    case RGB_FAD:
      if (record->event.pressed) {
        flip_rgb_bit(RGB_FADE_OUT);
        return false;
      }
      break;
    case RGB_ALL:
      if (record->event.pressed) {
        flip_rgb_bit(LIGHT_ALL_LEDS);
        return false;
      }
      break;
  }

  return true;
}

// lights the given number of random LEDs in a random color
void light_leds_random_color(uint8_t numberOfLeds) {
  rgblight_enable();
  rgblight_mode(1);

  turn_off_all_leds();

  for (int i = 0; i < numberOfLeds; i++) {
    uint8_t ledIndex = rand() % RGBLED_NUM;
    rgbled* led = &rgbs[ledIndex];

    int hue = rand() & 360;
    rgblight_sethsv_at(hue, 255, 255, ledIndex);

    rgb_fade_timer = timer_read();

    led->index = ledIndex;
    led->h = hue;
    led->s = 255;
    led->v = 255;
  }
}

// light an led at the given index in a random color
void light_led_random_color(uint8_t ledIndex) {
  rgblight_enable();
  rgblight_mode(1);

  // first turn off all other leds
  turn_off_all_leds();

  rgbled* led = &rgbs[ledIndex];
  led->h = rand() % 360;
}

// light all leds in the given hue
void light_all_leds(int hue) {
  rgblight_enable();
  rgblight_mode(1);

  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    led->h = hue;
    led->s = 255;
    led->v = 255;
  }
}

// sets all led values to 0
void turn_off_all_leds(void) {
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = & rgbs[i];

    led->v = 0;
  }
}

// reduce all leds value by 10 every 10 ms
void fadeLeds(void) {
  if (timer_elapsed(rgb_fade_timer) > 10) {
    for (int i = 0; i < RGBLED_NUM; i++) {
      rgbled* led = &rgbs[i];

      if (led->v > 9) {
        led->v -= 10;
      } else {
        led->v = 0;
      }

      rgb_fade_timer = timer_read();
    }
  }
}

// updates the actual LEDs with contents of the rgbs array
void set_leds(void) {
  uint8_t allLedsOff = 0;

  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    rgblight_sethsv_at(led->h, led->s, led->v, led->index);

    allLedsOff = led->v == 0;
  }

  if (allLedsOff) {
    //rgblight_mode(orig_rgb_mode);
  }
}


/* probably unnecessary bit field helper functions */
void flip_rgb_bit_on(uint8_t bit) {
  RGB_FLAGS |= bit;
}

void flip_rgb_bit_off(uint8_t bit) {
  RGB_FLAGS &= ~bit;
}

void flip_rgb_bit(uint8_t bit) {
  if (RGB_FLAGS & bit) {
    flip_rgb_bit_off(bit);
  } else {
    flip_rgb_bit_on(bit);
  }
}
