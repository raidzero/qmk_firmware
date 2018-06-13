/* Copyright 2018 raidzero
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

#define MPP KC_MEDIA_PLAY_PAUSE
#define MPR KC_MEDIA_PREV_TRACK
#define MPN KC_MEDIA_NEXT_TRACK
#define VUP KC_AUDIO_VOL_UP
#define VDN KC_AUDIO_VOL_DOWN

/* custom keycodes to trigger macros */
enum my_keycodes {
  DM1 = SAFE_RANGE,     // macro to move all desktops to right monitor and disable the others
  DM3,                  // macro to re-enable all monitors
};

/* tapdance friendly names */
enum {
  TD_SHOW_MUSIC = 0,
  TD_RIGHT_MONITOR = 1,
  TD_ALL_MONITORS = 2,
};

/* keymap tap dance shortcuts */
#define TDP TD(TD_SHOW_MUSIC)
#define TDN TD(TD_RIGHT_MONITOR)
#define TDR TD(TD_ALL_MONITORS)

/* tap dance defintions */
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_SHOW_MUSIC] = ACTION_TAP_DANCE_DOUBLE(MPP, LGUI(KC_6)),
  [TD_RIGHT_MONITOR] = ACTION_TAP_DANCE_DOUBLE(MPN, DM1),
  [TD_ALL_MONITORS] = ACTION_TAP_DANCE_DOUBLE(MPR, DM3)
};

/* keymap */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* default layer*/ LAYOUT( VUP, VDN, TDR, TDP, TDN )
};

/* macro handling */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case DM1:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI(SS_LSFT("d")));
        _delay_ms(200);
        SEND_STRING("l" SS_TAP(X_ENTER));
      }
      return false;
    case DM3:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI(SS_LSFT("d")));
        _delay_ms(200);
        SEND_STRING("jkl" SS_TAP(X_ENTER));
      }
      return false;
  }

  return true;
}
