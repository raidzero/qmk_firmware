#include "underglow.h"

extern rgblight_config_t rgblight_config;

// this controls all the reactive rgb bools
uint8_t RGB_FLAGS = LIGHT_RANDOM_LEDS | RGB_FADE_OUT;

// a place to keep references to all the RGB LEDs
static rgbled rgbs[RGBLED_NUM];

// a timer for controlling RGB fadeout
uint16_t rgb_fade_timer = 0;
uint8_t fade_speed = 20; // 20ms steps for fading LEDs

uint8_t orig_rgb_mode = 255;

void process_leds(keyrecord_t* record) {
  if (RGB_FLAGS & RGB_REACTIVE_ENABLED) {
    if (record->event.pressed) {
      if (RGB_FLAGS & LIGHT_ALL_LEDS) {
        light_all_leds(rand() % 360);
      } else if (RGB_FLAGS & LIGHT_RANDOM_LEDS) {
        light_leds_random_color(rand() % RGBLED_NUM);
      }
    }
  }
}

void scan_leds(void) {
if (RGB_FLAGS & RGB_REACTIVE_ENABLED) {
    set_leds();

    if (RGB_FLAGS & RGB_FADE_OUT) {
      fadeLeds();
    }
  }
}

void process_reactive_toggle() {
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
}

void process_reactive_fade() {
  flip_rgb_bit(RGB_FADE_OUT);
}

void process_reactive_all() {
  flip_rgb_bit(LIGHT_ALL_LEDS);
}

void process_rgb_toggle() {
  flip_rgb_bit(RGB_WAS_ENABLED);
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
  if (timer_elapsed(rgb_fade_timer) > fade_speed) {
    for (int i = 0; i < RGBLED_NUM; i++) {
      rgbled* led = &rgbs[i];

      if (led->v >= 10) {
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

    if (led->v > 0) {
      rgblight_sethsv_at(led->h, led->s, led->v, led->index);
    }

    allLedsOff = led->v == 0;
  }

  if (allLedsOff && RGB_FLAGS & RGB_WAS_ENABLED) {
    rgblight_mode(orig_rgb_mode);
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
