#include QMK_KEYBOARD_H

/* keymap beautifiers */
#define _______ KC_TRNS
#define x_____x KC_NO

/* media keys */
#define MPP KC_MEDIA_PLAY_PAUSE
#define MPR KC_MEDIA_PREV_TRACK
#define MPN KC_MEDIA_NEXT_TRACK
#define VUP KC_AUDIO_VOL_UP
#define VDN KC_AUDIO_VOL_DOWN

/* RGB keys */
#define RGB_ST RGB_MODE_PLAIN
#define RGB_BR RGB_MODE_BREATHE
#define RGB_RB RGB_MODE_RAINBOW
#define RGB_SW RGB_MODE_SWIRL
#define RGB_SN RGB_MODE_SNAKE
#define RGB_KN RGB_MODE_KNIGHT
#define RGB_TE RGB_MODE_RGBTEST

/* detect modifiers */
#define RSHIFT_HELD       (get_mods() & (MOD_BIT(KC_RSFT)))
#define LSHIFT_HELD       (get_mods() & (MOD_BIT(KC_LSFT)))
#define SHIFT_HELD        (get_mods() & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT)))

enum my_keycodes {
  RGB_RCT = SAFE_RANGE, // toggle RGB reactive mode
  RGB_FAD,              // toggle RGB reactive fadeout
  RGB_ALL,              // toggle RGB lighting all LEDs
  RGB_WHT,              // drop all LEDs saturation to 0, value to max (white)
  ANI_STA,               // animation: static color
  ANI_BRE,              // animation: breathe
  ANI_SPC,              // animation: spectrum
  ANI_RNB,              // animation: rainbow
  ANI_SWI,              // animation: swirl
  ANI_PUL,              // animation: pulse
  ANI_SPI,              // increase animation speed
  ANI_SPD,              // decreae animation speed
  KC_LOCK,              // toggle locked keyboard mode
};

bool is_lighting_key(uint16_t keycode);

bool process_key_rz(uint16_t keycode, keyrecord_t* record);
