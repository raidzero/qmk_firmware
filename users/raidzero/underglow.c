#include "underglow.h"
#include "animations.h"

extern rgblight_config_t rgblight_config;

// this controls all the reactive rgb bools
uint8_t RGB_FLAGS =  RGB_WAS_ENABLED | LIGHT_ALL_LEDS | RGB_FADE_OUT;

// a place to keep references to all the RGB LEDs
rgbled rgbs[RGBLED_NUM];

// timers for controlling RGB fading
uint16_t rgb_fade_out_timer = 0;
uint16_t rgb_fade_in_timer = 0;

uint8_t fade_speed = 10; // 20ms steps for fading LEDs in & out

uint8_t orig_rgb_mode = 0;

void init_leds() {
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    led->h = 0; // start red
    led->s = 255; // start with full saturation
    led->v = 255; // start with full brightness
  }
}

void process_leds(keyrecord_t* record) {
  if (RGB_FLAGS & RGB_REACTIVE_ENABLED) {
    if (record->event.pressed) {
      if (RGB_FLAGS & LIGHT_ALL_LEDS) {
        light_all_leds(rand() % 360);
      } else {
        light_all_leds(999);
      }

      if (RGB_FLAGS & RGB_FADE_OUT) {
        flip_rgb_bit_on(RGB_FADING_OUT);
      }
    }
  }
}

void scan_leds(void) {
  scan_animation(); // this will update the rgb leds array
  set_leds();

  if (RGB_FLAGS & RGB_FADING_OUT) {
    fade_leds_out();
  } else if (RGB_FLAGS & RGB_FADING_IN) {
    fade_leds_in();
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

void process_rgb_white() {
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    led->s = 0;
    led->v = 255;
  }

  rgblight_config.sat = 0;
  rgblight_config.val = 255;
}

void light_led_in_color(uint8_t index, int hue) {
  rgbled* led = &rgbs[index];

  led->v = 255;
  led->s = 255;
  led->h = hue;
}

// light all leds in the given hue
void light_all_leds(int hue) {
  rgblight_enable();
  rgblight_mode(0);

  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    if (hue <= 360) {
      light_led_in_color(i, hue);
    } else {
      light_led_in_color(i, rand() % 360);
    }
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
void fade_leds_out(void) {
  uint8_t allLedsOff = 0;

  if (timer_elapsed(rgb_fade_out_timer) > fade_speed) {
    for (int i = 0; i < RGBLED_NUM; i++) {
      rgbled* led = &rgbs[i];

      if (led->v >= 10) {
        led->v -= 10;
      } else {
        led->v = 0;
      }

      allLedsOff = led->v == 0;
    }

    if (allLedsOff) {
      // all leds are finished fading or otherwise off
      // flip the fadein bit so scan_leds() knows to call
      // fade_leds_in()
      if (RGB_FLAGS & RGB_WAS_ENABLED) {
        // allow animations to continue
        flip_rgb_bit_off(ANIM_SUSPEND);
        flip_rgb_bit_off(RGB_FADING_OUT);
        flip_rgb_bit_on(RGB_FADING_IN);
      }
    } else {
      // LEDs are fading. suspend animations
      flip_rgb_bit_on(ANIM_SUSPEND);
      flip_rgb_bit_on(RGB_FADING_OUT);
    }

    // update timer
    rgb_fade_out_timer = timer_read();
  }
}

// increase all leds value by 10 every 10 ms
void fade_leds_in(void) {
  uint8_t allLedsOn = 0;

  if (timer_elapsed(rgb_fade_in_timer) > fade_speed) {
    for (int i = 0; i < RGBLED_NUM; i++) {
      rgbled* led = &rgbs[i];

      if (led->v <= 245) {
        led->v += 10;
      } else {
        led->v = 255;
      }

      allLedsOn = led->v == 255;
    }

    if (allLedsOn) {
      // all leds are finished fading in or otherwise on
      // allow animations to continue, if RGB was on
      if (RGB_FLAGS & RGB_WAS_ENABLED) {
        flip_rgb_bit_off(ANIM_SUSPEND);
        flip_rgb_bit_off(RGB_FADING_IN);
      }
    } else {
      // LEDs are fading
      flip_rgb_bit_on(RGB_FADING_IN);
    }

    // update timer
    rgb_fade_in_timer = timer_read();
  }
}

// updates the actual LEDs with contents of the rgbs array
void set_leds(void) {

  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    if (led->v > 0) {
      rgblight_sethsv_at(led->h, led->s, led->v, i);
    }
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

bool is_rgb_bit_on(uint8_t bit) {
  return RGB_FLAGS & bit;
}

void set_leds_saturation(uint8_t sat) {
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];
    led->s = sat;
  }

  rgblight_config.sat = sat;
}

void set_leds_value(uint8_t val) {
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];
    led->v = val;
  }

  rgblight_config.val = val;
}

void change_leds_saturation(enum ValueMode mode) {
  uint8_t sat = rgblight_config.sat;

  if (mode == INCREASE) {
    if (sat <= 255 - RGBLIGHT_VAL_STEP) {
      sat += RGBLIGHT_SAT_STEP;
    } else {
      sat = 255;
    }
  } else {
    if (sat >= RGBLIGHT_SAT_STEP) {
      sat -= RGBLIGHT_SAT_STEP;
    } else {
      sat = 0;
    }
  }
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];
    led->s = sat;
  }

  rgblight_config.sat = sat;
}

void change_leds_value(enum ValueMode mode) {
  uint8_t val = rgblight_config.val;

  if (mode == INCREASE) {
    if (val <= 255 - RGBLIGHT_VAL_STEP) {
      val += RGBLIGHT_VAL_STEP;
    } else {
      val = 255;
    }
  } else {
    if (val >= RGBLIGHT_VAL_STEP) {
      val -= RGBLIGHT_VAL_STEP;
    } else {
      val = 0;
    }
  }
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];
    led->v = val;
  }

  rgblight_config.val = val;
}

void change_leds_hue(enum ValueMode mode) {
  int hue = rgblight_config.hue;

  if (mode == INCREASE) {
    hue = hue + RGBLIGHT_HUE_STEP % 360;
  } else {
    if (hue >= RGBLIGHT_HUE_STEP) {
      hue -= RGBLIGHT_HUE_STEP;
    } else {
      hue = 360 - RGBLIGHT_HUE_STEP;
    }
  }

  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];
    led->h = hue;
  }

  rgblight_config.hue = hue;
}
