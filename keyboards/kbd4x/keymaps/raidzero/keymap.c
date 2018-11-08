/* Copyright 2018 sevenseacat
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include "underglow.h"
#include "animations.h"
#include "raidzero.h"

#define _______ KC_TRNS
#define xxxxxxx KC_NO

/*
  43 blues
  33 w brown
  3 b brown
*/

enum my_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _OSWM,
  _NAVIGATION
};

#define LOWER   TT(_LOWER)              // tap toggle lower
#define RAISE   TT(_RAISE)              // tap toggle raise
#define SPCFN   LT(_NAVIGATION, KC_SPC) // arrow keys
#define OSLYR   MO(_OSWM)               // OS/WM layer
#define RSFTT   RSFT_T(KC_ENT)          // shift on hold, enter on tap
#define TABW    LGUI_T(KC_TAB)          // win on hold, tab on tap

// bracket mods - shifted ones do not work, unused
#define LC_PO   LCTL_T(S(KC_9))         // left ctrl on hold, open paren on tap
#define RC_PC   RCTL_T(KC_RPRN)         // right ctrl on hold, close paren on tap
#define LW_BO   LGUI_T(KC_LBRC)         // left win on hold, open bracket on tap
#define RW_BC   RGUI_T(KC_RBRC)         // right win on hold, close bracket on tap
#define LA_BO   LALT_T(KC_LCBR)         // left alt on hold, open brace on tap
#define RA_BC   RALT_T(KC_RCBR)         // right alt on hold, close brace on tap

// i3 macros
// workspaces
#define I31     LGUI(KC_1)
#define I32     LGUI(KC_2)
#define I33     LGUI(KC_3)
#define I34     LGUI(KC_4)
#define I35     LGUI(KC_5)
#define I36     LGUI(KC_6)
#define I37     LGUI(KC_7)
#define I38     LGUI(KC_8)
#define I39     LGUI(KC_9)
#define I30     LGUI(KC_0)
// other
#define I3X     SGUI(KC_Q)              // quit active app
#define I3LM    LGUI(KC_LBRC)           // workspace to the left
#define I3RM    LGUI(KC_RBRC)           // workspace to the right
#define I3PWR   LGUI(KC_SCROLLLOCK)     // power menu
#define DISP    SGUI(KC_D)              // monitor selection mode

// macos helpers
#define MSW     LGUI(KC_GRV)            // switch window
#define PASTE   SGUI(LALT(KC_V))        // paste w/o formatting

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  KBD4X = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Tab/W|   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | WM/OS| Ctrl | Alt  | GUI  |Lower | Space / FN  |Raise | Left | Down |  Up  | Right|
 * `-----------------------------------------------------------------------------------'
 */
 [_QWERTY] = LAYOUT_planck_mit(
    KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    TABW,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RSFTT,
    OSLYR,   KC_LCTL, KC_LGUI, KC_LALT, LOWER,   SPCFN,   RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |      |      |      |      |      |      |   _  |   +  |   {  |   }  |  |   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |    | | Home | End  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_mit(
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,  KC_ASTR,  KC_LPRN, KC_RPRN, KC_BSPC,
    KC_DEL,  _______, _______, _______, _______, _______, _______, KC_UNDS,  KC_PLUS,  KC_LCBR, KC_RCBR, KC_PIPE,
    _______, _______, _______, _______, _______, _______, _______, _______,  _______,  KC_HOME, KC_END,  _______,
    _______, _______, _______, _______, _______, _______, _______, KC_MNXT,  KC_VOLD,  KC_VOLU, KC_MPLY
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |      |      |      |      |      |   *  |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |   /  |   +  |   .  |Pg Up |Pg Dn |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_mit(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_DEL,  _______, _______, _______, _______, _______, KC_ASTR, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
    _______, _______, _______, _______, _______, _______, KC_SLSH, KC_PLUS, KC_DOT,  KC_PGUP, KC_PGDN, _______,
    _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY
),

/* navigation layer / F keys
 * ,-----------------------------------------------------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |      |      |      |  UP  |      |      |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |  F5  |  F6  |  F7  |  F8  |      |      | LEFT | DOWN`| RIGHT|      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F9  | F10  | F11  | F12  |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_NAVIGATION] = LAYOUT_planck_mit(
    xxxxxxx, KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______, _______, xxxxxxx, KC_UP,   xxxxxxx, xxxxxxx, xxxxxxx,
    xxxxxxx, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______, _______, KC_LEFT, KC_DOWN, KC_RIGHT,xxxxxxx, xxxxxxx,
    xxxxxxx, KC_F9,   KC_F10,  KC_F11,  KC_F12,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,
    xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx
),

/* i3wm/macos
 * ,-----------------------------------------------------------------------------------.
 * |  MSW | ws1  | ws2  | ws3  | ws4  | ws5  | ws6  | ws7  | ws8  | ws9  | ws0  | ____ |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      | DISP |      |      |      |      |      | wslm | wsrm | power|
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |______|      | I3X  |      |PASTE |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_OSWM] = LAYOUT_planck_mit(
    MSW,     I31,     I32,     I33,     I34,     I35,     I36,     I37,     I38,     I39,     I30,     _______,
    _______, _______, _______, DISP,    _______, _______, _______, _______, _______, I3LM,    I3RM,    I3PWR,
    _______, _______, I3X,     _______, PASTE,   _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)
};

// shift + shift + raise + k = bootloader
// shift + shift + raise + j = eeprom reset
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return process_key_rz(keycode, record);
}

void matrix_init_user(void) {
  init_leds();
  init_animation();
}

void matrix_scan_user(void) {\
  scan_leds();
}

void led_set_user(uint8_t usb_led) {
}
