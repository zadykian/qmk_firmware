#include QMK_KEYBOARD_H

enum encoder_name {
    ENCODER_RIGHT  = 1,
    ENCODER_MIDDLE = 2,
};

enum key_name {
    KEY_ANY,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
};

enum tap_dance_action_name {
    ACTION_UNKNOWN,
    ACTION_SINGLE_TAP,
    ACTION_SINGLE_HOLD,
    ACTION_DOUBLE_TAP,
    ACTION_DOUBLE_HOLD,
    ACTION_TRIPLE_TAP,
    ACTION_TRIPLE_HOLD,
};

typedef struct {
    uint16_t on_single_tap;
    uint16_t on_single_hold;
    uint16_t on_double_tap;
    uint16_t on_double_hold;
    uint16_t on_triple_tap;
    uint16_t on_triple_hold;
} tap_dance_action_codes_t;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        TD(KEY_ANY) , KC_MUTE     , KC_KP_ASTERISK ,
        TD(KEY_F13) , TD(KEY_F14) , TD(KEY_F15)    ,
        TD(KEY_F16) , TD(KEY_F17) , TD(KEY_F18)
    )
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == ENCODER_MIDDLE) {
        if (clockwise) {
            tap_code(KC_VOLU);
        }
        else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == ENCODER_RIGHT) {
        if (clockwise) {
            tap_code(KC_KP_PLUS);
        }
        else {
            tap_code(KC_KP_MINUS);
        }
    }
    return false;
}

enum tap_dance_action_name get_action_type(tap_dance_state_t *state) {
    bool is_tapped = state->interrupted || !state->pressed;
    switch (state->count) {
        case 1: return is_tapped ? ACTION_SINGLE_TAP : ACTION_SINGLE_HOLD;
        case 2: return is_tapped ? ACTION_DOUBLE_TAP : ACTION_DOUBLE_HOLD;
        case 3: return is_tapped ? ACTION_TRIPLE_TAP : ACTION_TRIPLE_HOLD;
        default: return ACTION_UNKNOWN;
    }
}

void tap_dance_on_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_action_codes_t *action_codes = (tap_dance_action_codes_t *)user_data;
    switch (get_action_type(state)) {
        case ACTION_SINGLE_TAP  : tap_code16(action_codes->on_single_tap)  ; break;
        case ACTION_SINGLE_HOLD : tap_code16(action_codes->on_single_hold) ; break;
        case ACTION_DOUBLE_TAP  : tap_code16(action_codes->on_double_tap)  ; break;
        case ACTION_DOUBLE_HOLD : tap_code16(action_codes->on_double_hold) ; break;
        case ACTION_TRIPLE_TAP  : tap_code16(action_codes->on_triple_tap)  ; break;
        case ACTION_TRIPLE_HOLD : tap_code16(action_codes->on_triple_hold) ; break;
        default: break;
    }
}

#define TAP_DANCING(st, sh, dt, dh, tt, th) \
    { \
        .fn = { NULL, tap_dance_on_finished, NULL, NULL }, \
        .user_data = (void *)&((tap_dance_action_codes_t){ st, sh, dt, dh, tt, th }), \
    }

#define TAP_DANCING_TAPS(on_single_tap, on_double_tap, on_triple_tap) \
    TAP_DANCING(on_single_tap, KC_NO, on_double_tap, KC_NO, on_triple_tap, KC_NO )

tap_dance_action_t tap_dance_actions[] = {
    [KEY_ANY] = TAP_DANCING_TAPS(KC_MPLY, KC_MNXT, KC_MPRV),
    [KEY_F13] = TAP_DANCING_TAPS(KC_F13, KC_F19, KC_NO),
    [KEY_F14] = TAP_DANCING_TAPS(KC_F14, KC_F20, KC_NO),
    [KEY_F15] = TAP_DANCING_TAPS(KC_F15, KC_F21, KC_NO),
    [KEY_F16] = TAP_DANCING_TAPS(KC_F16, KC_F22, KC_NO),
    [KEY_F17] = TAP_DANCING_TAPS(KC_F17, KC_F23, KC_NO),
    [KEY_F18] = TAP_DANCING_TAPS(KC_F18, KC_F24, KC_NO),
};
