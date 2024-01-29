#include QMK_KEYBOARD_H

enum encoder_name {
    ENCODER_RIGHT  = 1,
    ENCODER_MIDDLE = 2,
};

enum key_name {
    KEY_ANY,
    KEY_F04,
    KEY_F05,
    KEY_F06,
    KEY_F07,
    KEY_F08,
    KEY_F09,
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
        TD(KEY_ANY) , KC_MUTE     , C(KC_F13)   ,
        TD(KEY_F04) , TD(KEY_F05) , TD(KEY_F06) ,
        TD(KEY_F07) , TD(KEY_F08) , TD(KEY_F09)
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
            tap_code16(S(KC_F13));
        }
        else {
            tap_code16(A(KC_F13));
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
        case ACTION_SINGLE_TAP  : register_code16(action_codes->on_single_tap)  ; break;
        case ACTION_SINGLE_HOLD : register_code16(action_codes->on_single_hold) ; break;
        case ACTION_DOUBLE_TAP  : register_code16(action_codes->on_double_tap)  ; break;
        case ACTION_DOUBLE_HOLD : register_code16(action_codes->on_double_hold) ; break;
        case ACTION_TRIPLE_TAP  : register_code16(action_codes->on_triple_tap)  ; break;
        case ACTION_TRIPLE_HOLD : register_code16(action_codes->on_triple_hold) ; break;
        default: break;
    }
}

void tap_dance_on_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_action_codes_t *action_codes = (tap_dance_action_codes_t *)user_data;
    switch (get_action_type(state)) {
        case ACTION_SINGLE_TAP  : unregister_code16(action_codes->on_single_tap)  ; break;
        case ACTION_SINGLE_HOLD : unregister_code16(action_codes->on_single_hold) ; break;
        case ACTION_DOUBLE_TAP  : unregister_code16(action_codes->on_double_tap)  ; break;
        case ACTION_DOUBLE_HOLD : unregister_code16(action_codes->on_double_hold) ; break;
        case ACTION_TRIPLE_TAP  : unregister_code16(action_codes->on_triple_tap)  ; break;
        case ACTION_TRIPLE_HOLD : unregister_code16(action_codes->on_triple_hold) ; break;
        default: break;
    }
}

#define TAP_DANCING(st, sh, dt, dh, tt, th) \
    { \
        .fn = { NULL, tap_dance_on_finished, tap_dance_on_reset, NULL }, \
        .user_data = (void *)&((tap_dance_action_codes_t){ st, sh, dt, dh, tt, th }), \
    }

#define TAP_DANCING_FN(key) TAP_DANCING( A(key) , G(A(key)) , C(key) , G(C(key)) , S(key) , G(S(key)) )

tap_dance_action_t tap_dance_actions[] = {
    [KEY_ANY] = TAP_DANCING(KC_MPLY, KC_NO, KC_MNXT, KC_NO, KC_MPRV, KC_NO),

    [KEY_F04] = TAP_DANCING_FN(KC_F14),
    [KEY_F05] = TAP_DANCING_FN(KC_F15),
    [KEY_F06] = TAP_DANCING_FN(KC_F16),

    [KEY_F07] = TAP_DANCING_FN(KC_F17),
    [KEY_F08] = TAP_DANCING_FN(KC_F18),
    [KEY_F09] = TAP_DANCING_FN(KC_F19),
};
