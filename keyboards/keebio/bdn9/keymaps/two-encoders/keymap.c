#include QMK_KEYBOARD_H

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_MPLY , KC_NO   , KC_MUTE ,
        KC_NO   , KC_NO   , KC_NO   ,
        KC_NO   , KC_NO   , KC_NO
    )
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _MIDDLE) {
        if (clockwise) {
            tap_code(KC_NO);
        } else {
            tap_code(KC_NO);
        }
    }
    else if (index == _RIGHT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return false;
}
