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

/* reactive rgb mode bit field */
#define RGB_REACTIVE_ENABLED  (1 << 0)
#define RGB_WAS_ENABLED       (1 << 1)
#define LIGHT_RANDOM_LEDS     (1 << 2)
#define LIGHT_ALL_LEDS        (1 << 3)
#define RGB_FADE_OUT          (1 << 4)

void flip_rgb_bit_on(uint8_t bit);
void flip_rgb_bit_off(uint8_t bit);
void flip_rgb_bit(uint8_t bit);

enum my_keycodes {
  RGB_RCT = SAFE_RANGE, // toggle RGB reactive mode
  RGB_FAD,              // toggle RGB reactive fadeout
  RGB_ALL,              // toggle RGB lighting all LEDs
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

void turn_off_all_leds(void);
void fadeLeds(void);
void set_leds(void);
