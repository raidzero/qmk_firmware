
#include QMK_KEYBOARD_H

/* keymap beautifiers */
#define _______ KC_TRNS
#define x_____x KC_NO

/* almighty spaceFN */
#define SPCFN LT(2, KC_SPACE)
#define CAPS MT(MOD_LGUI, KC_CAPS)

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


/* detect modifiers */
#define RSHIFT_HELD       (get_mods() & (MOD_BIT(KC_RSFT)))
#define LSHIFT_HELD       (get_mods() & (MOD_BIT(KC_LSFT)))
#define SHIFT_HELD        (get_mods() & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT)))

enum my_keycodes {
  RGB_RCT = SAFE_RANGE, // toggle RGB reactive mode
};

typedef struct {
  int h;
  uint8_t s;
  uint8_t v;
  uint8_t index;
  bool on;
} rgbled;

/* custom functions */
void light_leds_random_color(uint8_t numberOfLeds); // light up number of random LEDS in random colors
void light_led_random_color(uint8_t ledIndex); // light up an led at ledIndex in a random color
void light_all_leds(int hue);

void fadeLeds(void);
void set_leds(void);
