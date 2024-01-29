#include QMK_KEYBOARD_H

enum encoder_names {
  LEFT,
  RIGHT,
  MIDDLE,
};

enum tap_dance_names {
    MEDIA_CONTROL,
    F14_MACRO,
    F15_MACRO,
    F16_MACRO,
    F17_MACRO,
    F18_MACRO,
    F19_MACRO,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        TD(MEDIA_CONTROL) , KC_MUTE       , KC_NO         ,
        TD(F14_MACRO)     , TD(F15_MACRO) , TD(F16_MACRO) ,
        TD(F17_MACRO)     , TD(F18_MACRO) , TD(F19_MACRO) ,
    )
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == MIDDLE) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == RIGHT) {
        if (clockwise) {
            tap_code(KC_NO);
        } else {
            tap_code(KC_NO);
        }
    }
    return false;
}

void media_control_handler(tap_dance_state_t *state, void *user_data) {
    switch (state -> count) {

        // Single Tap
        case 1:
            tap_code(KC_MEDIA_PLAY_PAUSE);
            return;

        // Double Tap
        case 2:
            tap_code(KC_MEDIA_NEXT_TRACK);
            return;

        // Triple Tap
        case 3:
            tap_code(KC_MEDIA_PREV_TRACK);
            return;

        default:
            return;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [MEDIA_CONTROL] = ACTION_TAP_DANCE_FN(media_control_handler),

    [F14_MACRO_KEY] = ACTION_TAP_DANCE_DOUBLE(KC_F14, RCTL(KC_F14)),
    [F15_MACRO_KEY] = ACTION_TAP_DANCE_DOUBLE(KC_F14, RCTL(KC_F15)),
    [F16_MACRO_KEY] = ACTION_TAP_DANCE_DOUBLE(KC_F14, RCTL(KC_F16)),

    [F17_MACRO_KEY] = ACTION_TAP_DANCE_DOUBLE(KC_F14, RCTL(KC_F17)),
    [F18_MACRO_KEY] = ACTION_TAP_DANCE_DOUBLE(KC_F14, RCTL(KC_F18)),
    [F19_MACRO_KEY] = ACTION_TAP_DANCE_DOUBLE(KC_F14, RCTL(KC_F19)),
};
