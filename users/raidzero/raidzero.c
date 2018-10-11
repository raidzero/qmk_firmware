#include "raidzero.h"
#include "underglow.h"
#include "animations.h"

bool is_lighting_key(uint16_t keycode) {
  return
    /* QMK RGB code */
    (keycode >= RGB_HUI && keycode <= RGB_VAD)
    /* QMK backlight code */
    || (keycode >= BL_ON && keycode <= BL_BRTG)
    /* one of my rgb mode codes */
    || (keycode >= ANI_STA && keycode <= ANI_SPD);
}

bool process_key_rz(uint16_t keycode, keyrecord_t* record) {

  // if reactive, do not react when lighting keys are used
  if (is_rgb_bit_on(RGB_REACTIVE_ENABLED)) {
    if(!is_lighting_key(keycode)) {
      process_leds(record);
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
    case RGB_WHT:
      if (record->event.pressed) {
        animation_mode(ANIMATION_MODE_STATIC);
        process_rgb_white();
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
    case ANI_SWI:
      if (record->event.pressed) {
        animation_mode(ANIMATION_MODE_SWIRL);
        return false;
      }
      break;
    case RGB_VAI:
      if (record->event.pressed) {
        change_leds_value(INCREASE);
        return false;
      }
      break;
     case RGB_VAD:
      if (record->event.pressed) {
        change_leds_value(DECREASE);
        return true;
      }
      break;
    case RGB_SAI:
      if (record->event.pressed) {
        change_leds_saturation(INCREASE);
        return true;
      }
      break;
     case RGB_SAD:
      if (record->event.pressed) {
        change_leds_saturation(DECREASE);
        return true;
      }
      break;
    case RGB_HUI:
      if (record->event.pressed) {
        change_leds_hue(INCREASE);
        return true;
      }
      break;
     case RGB_HUD:
      if (record->event.pressed) {
        change_leds_hue(DECREASE);
        return true;
      }
      break;
    }

 return true;
}
