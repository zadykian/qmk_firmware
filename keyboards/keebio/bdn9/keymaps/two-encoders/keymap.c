#include QMK_KEYBOARD_H

enum encoder_names {
    ENCODER_MIDDLE = 1,
    ENCODER_RIGHT  = 2,
};

enum tap_dance_action_type {
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
};

enum macro_pad_key_names {
    ANY_KEY,
    F04_KEY,
    F05_KEY,
    F06_KEY,
    F07_KEY,
    F08_KEY,
    F09_KEY,
};

typedef struct {
    uint16_t on_single_tap;
    uint16_t on_single_hold;
    uint16_t on_double_tap;
    uint16_t on_double_hold;
    uint16_t on_triple_tap;
    uint16_t on_triple_hold;
} tap_dance_tap_hold_t;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        TD(ANY_KEY) , KC_MUTE     , KC_NO       ,
        TD(F04_KEY) , TD(F05_KEY) , TD(F06_KEY) ,
        TD(F07_KEY) , TD(F08_KEY) , TD(F09_KEY)
    )
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == ENCODER_MIDDLE) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == ENCODER_RIGHT) {
        if (clockwise) {
            tap_code(KC_NO);
        } else {
            tap_code(KC_NO);
        }
    }
    return false;
}

enum tap_dance_action_type get_action_type(tap_dance_state_t *state) {
    bool is_tapped = state->interrupted || !state->pressed;
    switch (state->count) {
        case 1: return is_tapped ? TD_SINGLE_TAP : TD_SINGLE_HOLD;
        case 2: return is_tapped ? TD_DOUBLE_TAP : TD_DOUBLE_HOLD;
        case 3: return is_tapped ? TD_TRIPLE_TAP : TD_TRIPLE_HOLD;
        default: return TD_UNKNOWN;
    }
}

void tap_dance_on_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;
    switch (get_action_type(state)) {
        case TD_SINGLE_TAP  : register_code(tap_hold->on_single_tap)  ; break;
        case TD_SINGLE_HOLD : register_code(tap_hold->on_single_hold) ; break;
        case TD_DOUBLE_TAP  : register_code(tap_hold->on_double_tap)  ; break;
        case TD_DOUBLE_HOLD : register_code(tap_hold->on_double_hold) ; break;
        case TD_TRIPLE_TAP  : register_code(tap_hold->on_triple_tap)  ; break;
        case TD_TRIPLE_HOLD : register_code(tap_hold->on_triple_hold) ; break;
        default: break;
    }
}

void tap_dance_on_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;
    switch (get_action_type(state)) {
        case TD_SINGLE_TAP  : unregister_code(tap_hold->on_single_tap)  ; break;
        case TD_SINGLE_HOLD : unregister_code(tap_hold->on_single_hold) ; break;
        case TD_DOUBLE_TAP  : unregister_code(tap_hold->on_double_tap)  ; break;
        case TD_DOUBLE_HOLD : unregister_code(tap_hold->on_double_hold) ; break;
        case TD_TRIPLE_TAP  : unregister_code(tap_hold->on_triple_tap)  ; break;
        case TD_TRIPLE_HOLD : unregister_code(tap_hold->on_triple_hold) ; break;
        default: break;
    }
}

#define TAP_DANCING(st, sh, dt, dh, tt, th) \
    { .fn = {NULL, tap_dance_on_finished, tap_dance_on_reset, NULL}, .user_data = (void *)&((tap_dance_tap_hold_t){st, sh, dt, dh, tt, th}), }

tap_dance_action_t tap_dance_actions[] = {
    // Key    \   Action   |    1x Tap    |   1x Hold    |    2x Tap    |   2x Hold    |    3x Tap    |   3x Hold    |

    [ANY_KEY] = TAP_DANCING(   KC_MPLY    ,    KC_NO     ,   KC_MNXT    ,    KC_NO     ,   KC_MPRV    ,    KC_NO     ),

    [F04_KEY] = TAP_DANCING(  A(KC_F14)   ,  C(KC_F14)   ,  S(KC_F14)   , A(C(KC_F14)) , A(S(KC_F14)) , C(S(KC_F14)) ),
    [F05_KEY] = TAP_DANCING(  A(KC_F15)   ,  C(KC_F15)   ,  S(KC_F15)   , A(C(KC_F15)) , A(S(KC_F15)) , C(S(KC_F15)) ),
    [F06_KEY] = TAP_DANCING(  A(KC_F16)   ,  C(KC_F16)   ,  S(KC_F16)   , A(C(KC_F16)) , A(S(KC_F16)) , C(S(KC_F16)) ),

    [F07_KEY] = TAP_DANCING(  A(KC_F17)   ,  C(KC_F17)   ,  S(KC_F17)   , A(C(KC_F17)) , A(S(KC_F17)) , C(S(KC_F17)) ),
    [F08_KEY] = TAP_DANCING(  A(KC_F18)   ,  C(KC_F18)   ,  S(KC_F18)   , A(C(KC_F18)) , A(S(KC_F18)) , C(S(KC_F18)) ),
    [F09_KEY] = TAP_DANCING(  A(KC_F19)   ,  C(KC_F19)   ,  S(KC_F19)   , A(C(KC_F19)) , A(S(KC_F19)) , C(S(KC_F19)) ),
};
