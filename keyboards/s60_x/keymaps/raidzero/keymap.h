/* keymap beautifiers */
#define _______ KC_TRNS
#define x_____x KC_NO

/* almighty spaceFN */
#define SPCFN LT(2, KC_SPACE)

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

/* detect modifiers */
#define RSHIFT_HELD       (get_mods() & (MOD_BIT(KC_RSFT)))
#define LSHIFT_HELD       (get_mods() & (MOD_BIT(KC_LSFT)))
#define SHIFT_HELD        (get_mods() & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT)))

enum my_keycodes {
  RGB_RCT = SAFE_RANGE, // toggle RGB reactive mode
};

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  bool on;
} rgbled;