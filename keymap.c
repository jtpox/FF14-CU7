/*
Copyright 2021 CapsUnlocked

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H

enum LAYER_NAMES {
  MACROS,
  FF,
};

enum CUSTOM_KEYCODES {
  GREETING = SAFE_RANGE,
  GG,
  MOMMA,
};

enum {
  TD_RESET_OR_MACRO
};

void macro_or_reset(tap_dance_state_t *state, void *user_data) {
  if(state->count == 5) {
    reset_keyboard();
    return;
  }

  SEND_STRING("Yo momma so ");
  return;
}

tap_dance_action_t tap_dance_actions[] = {
  [TD_RESET_OR_MACRO] = ACTION_TAP_DANCE_FN(macro_or_reset)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [MACROS] = LAYOUT(
                DF(FF),
    GREETING,   GG,         TD(TD_RESET_OR_MACRO),
    LCTL(KC_C), LCTL(KC_V), LCTL(KC_X)
  ),
  [FF] = LAYOUT(
             DF(MACROS),
    KC_4,    KC_5,    KC_6,
    KC_1,    KC_2,    KC_3
  ),
};

void keyboard_post_init_kb(void) {
  rgblight_setrgb(RGB_OFF);
}

void keyboard_post_init_user(void) {
  rgblight_setrgb(RGB_OFF);
}

// Volume up/down on the encoder
bool encoder_update_user(uint8_t index, bool clockwise) {
  int current_layer = get_highest_layer(layer_state | default_layer_state);

  if(current_layer == FF) {
    if(clockwise) tap_code_delay(KC_TAB, 10);
    if(!clockwise) tap_code16_delay(LSFT(KC_TAB), 10);

    return false; // Disable default behavior of QMK's rotary encoder.
  }

  return true;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
  int current_state = biton32(state);

  if(current_state == FF) rgblight_setrgb(RGB_PURPLE);
  if(current_state == MACROS) rgblight_setrgb(RGB_OFF);

  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if(record->event.pressed) {
    if(keycode == GREETING) SEND_STRING("" SS_TAP(X_ENTER) "o7" SS_TAP(X_ENTER));
    if(keycode == GG) SEND_STRING("" SS_TAP(X_ENTER) "gg and thanks!" SS_TAP(X_ENTER));
  }

  return true;
}