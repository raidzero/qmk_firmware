#include QMK_KEYBOARD_H

#define MPP KC_MEDIA_PLAY_PAUSE
#define MPR KC_MEDIA_PREV_TRACK
#define MPN KC_MEDIA_NEXT_TRACK

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* 0: ANSI qwerty */
  LAYOUT(
    F(2),   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,  KC_NO,   KC_BSPC, \
    KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC, KC_BSLS,       \
    KC_CAPS,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,  KC_NO,   KC_ENT ,       \
    KC_LSFT,   KC_NO,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,           KC_RSFT, KC_NO, \
    KC_LCTL,   KC_LGUI, KC_LALT,                   KC_FN0,                    KC_RALT, KC_FN0,  KC_FN1,  KC_RCTL),

/* 1: Fn layer (FN0) */
  LAYOUT(
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NO, KC_DEL, \
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP,   KC_NO, KC_PSCR, KC_SLCK, KC_PAUS, KC_NO,       \
    KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO, KC_NO, KC_LEFT, KC_DOWN, KC_RIGHT, KC_NO, KC_NO, KC_NO, KC_NO,       \
    KC_TRNS, KC_NO, MPR, MPP, MPN, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,           KC_TRNS, KC_NO, \
    KC_TRNS, KC_TRNS, KC_TRNS,                   KC_NO,                   KC_TRNS, KC_NO, KC_NO, KC_TRNS),

/* 2: lighting layer (FN1) */
  LAYOUT(
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, \
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       \
    KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       \
    KC_TRNS, KC_NO, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, BL_BRTG, BL_DEC,  BL_TOGG, BL_INC,           KC_TRNS, KC_NO, \
    KC_TRNS, KC_TRNS, KC_TRNS,                   KC_NO,                   KC_TRNS, KC_NO, KC_NO, KC_TRNS),
};

const uint16_t PROGMEM fn_actions[] = {
  [0] = ACTION_LAYER_TAP_KEY(1, KC_SPACE),    // make space do double duty as FN (to layer 1)
  [1] = ACTION_LAYER_MOMENTARY(2),            // PN key to layer 2
  [2] = ACTION_FUNCTION(2),                   // ESC handling for grave and tilde
};


/* esc = esc, shift esc = tilde
this is called on default layer, FN layer will just produce a grave */
#define MODS_SHIFT_MASK   (MOD_BIT(KC_LSFT)|MOD_BIT(KC_RSFT))

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
  static uint8_t shift_pressed;

  switch(id) {
    case 2: // ESC key
      shift_pressed = get_mods() & MODS_SHIFT_MASK;

      if (record->event.pressed) { // key is going down
        if (!shift_pressed) {
          add_key(KC_ESC);
          send_keyboard_report();
        } else {
          add_key(KC_GRV);
          send_keyboard_report();
        }
      } else { // key is being released
         if (!shift_pressed) {
          del_key(KC_ESC);
          send_keyboard_report();
        } else {
          del_key(KC_GRV);
          send_keyboard_report();
        }
      }

      break;
  }
}
