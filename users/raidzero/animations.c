#include "animations.h"
#include "underglow.h"
#include "keymap.h"
#include <print.h>

uint8_t mode = ANIMATION_MODE_STATIC;

uint16_t anim_speed = 40; // time to wait between steps of the animation

uint16_t currentHue = 0;

uint16_t step_timer = 0;

extern rgbled rgbs[RGBLED_NUM];
extern uint8_t RGB_FLAGS;
extern rgblight_config_t rgblight_config;

void init_animation() {
  mode = eeprom_read_byte(ANIM_EEPROM_BYTE);
}

void speed_increase() {
  if (anim_speed > 1) {
    anim_speed -= 1;
  }
}

void speed_decrease() {
  if (anim_speed < 10000) {
    anim_speed += 1;
  }
}

void animation_mode(uint8_t newMode) {
  if ((newMode <= ANIMATION_MODE_SPECTRUM && newMode >= ANIMATION_MODE_RAINBOW)
    || newMode == ANIMATION_MODE_STATIC || newMode == ANIMATION_MODE_SWIRL) {
    // if spectrum or rainbow or static, max out sat & val for all leds, in case they were down
    set_leds_saturation(255);
    set_leds_value(255);
  }

  mode = newMode;
  eeprom_update_byte(ANIM_EEPROM_BYTE, mode);
}

void scan_animation() {
  if (!(RGB_FLAGS & ANIM_SUSPEND)) {
    if (timer_elapsed(step_timer) > anim_speed) {
      step_timer = timer_read();

      switch(mode) {
        case ANIMATION_MODE_STATIC:
          animation_step_static();
          break;
        case ANIMATION_MODE_BREATHE:
          animation_step_breathe();
          break;
        case ANIMATION_MODE_SPECTRUM:
          animation_step_spectrum();
          break;
        case ANIMATION_MODE_RAINBOW:
          animation_step_rainbow();
          break;
        case ANIMATION_MODE_SWIRL:
          animation_step_swirl();
          break;
        case ANIMATION_MODE_PULSE:
          animation_step_pulse();
          break;
      }
    }
  }
}

// do nothing
void animation_step_static() {
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    led->h = rgblight_config.hue;
    led->s = rgblight_config.sat;
    led->v = rgblight_config.val;
  }
}

void animation_step_breathe() {
  float val = (exp(sin(timer_read()/2000.0*3.14159)) - 0.36787944)*108.0;

  for (int i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    led->v = (uint8_t) val;
    led->s = rgblight_config.sat;
    led->h = rgblight_config.hue;
  }
}

// fade through all the possible hues per LED
void animation_step_spectrum() {
  // iterate over all leds and increment the hue
  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    led->h = currentHue;
  }

  // increment hue once per step
  currentHue = (currentHue + 1) % 360;
}

// show every possible hue across the LEDs and rotate
void animation_step_rainbow() {
  uint16_t hue = 0;

  for (uint8_t i = 0; i < RGBLED_NUM; i++) {
    rgbled* led = &rgbs[i];

    // set hue per led
    hue = (360 / RGBLED_NUM * i + currentHue) % 360;
    led->h = hue;
  }

  // increment currentHue
  currentHue = (currentHue + 1) % 360;
}

int active_led = 0;

void animation_step_swirl() {
  // turn off all other leds
  for (int i = 0; i < RGBLED_NUM; i++) {
    if (i != active_led) {
      rgbled* led = &rgbs[i];
      led->v = 0;
    }
  }

  // if the last led has been activated, start over for the next iteration
  if (active_led++ >= RGBLED_NUM) {
    active_led = 0;
  }

  rgbled* led = &rgbs[active_led];
  led->v = 255;
}

uint8_t row1[] = RGB_ROW_1;
uint8_t row2[] = RGB_ROW_2;

// left and right directions
short right_index = RGB_ROW_SIZE / 2;
#if RGB_ROW_SIZE % 2 == 0
short left_index = RGB_ROW_SIZE / 2 - 1;
#else
short left_index = RGB_ROW_SIZE / 2;
#endif

void animation_step_pulse() {
  /*
  turn_off_all_leds();

  rgbled* led = &rgbs[8];
  led->v = 255;
  */
  for (int i = 0; i < RGB_ROW_SIZE; i++) {
    if (i != right_index && i != left_index) {
      rgbled* led1 = &rgbs[row1[i]];
      rgbled* led2 = &rgbs[row2[i]];
      led1->v = 0;
      led2->v = 0;
    }
  }

  // turn on active column leds:
  // moves to the right:
  rgbled* led1 = &rgbs[row1[right_index]];
  rgbled* led2 = &rgbs[row2[right_index++]];
  led1->v = 255;
  led2->v = 255;

  // moves to the left:
  rgbled* led3 = &rgbs[row1[left_index]];
  rgbled* led4 = &rgbs[row2[left_index--]];
  led3->v = 255;
  led4->v = 255;

  if (right_index > RGB_ROW_SIZE - 1) {
    right_index = RGB_ROW_SIZE / 2;
  }

  if (left_index < 0) {
    #if RGB_ROW_SIZE % 2 == 0
    left_index = RGB_ROW_SIZE / 2 - 1;
    #else
    left_index = RGB_ROW_SIZE / 2;
    #endif
  }
}
