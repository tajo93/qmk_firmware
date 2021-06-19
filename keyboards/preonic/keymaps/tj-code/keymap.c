/* Copyright 2015-2017 Jack Humbert
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
#include "muse.h"

#define ESCNAV LT(_NAV, KC_ESC)

enum preonic_layers {
    _QWERTY,
    _WORKMAN,
    _LOWER,
    _RAISE,
    _NAV,
    _FN,
    _MENU,
    _MOUSE,
    _ADJUST
};

enum preonic_keycodes {
    LAYOUT = SAFE_RANGE,
    LOWER,
    RAISE,
    QWERTY,
    WORKMAN,
    WIN_TAB,
    WIGGLER
};

enum wiggle_directions {
    LEFT,
    RIGHT
};

int char_count    = 0;
int last_word_len = 0;
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
bool wiggler = false;
uint16_t wiggle_direction = LEFT;
uint16_t wiggle_timer = 0;

enum combos {
    COMBO_DELETE_WORD
};

const uint16_t PROGMEM combo_delete_word[] = {KC_D, KC_W, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    [COMBO_DELETE_WORD] = COMBO_ACTION(combo_delete_word)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Qwerty
     * ,-----------------------------------------------------------------------------------.
     * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Del  |
     * |------+------+------+------+------+-------------+------+------+------+------+------|
     * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
     * |------+------+------+------+------+------|------+------+------+------+------+------|
     * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Shift |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | CTtrl| Gui  | Alt  | DN   |Lower |    Space    |Raise | CMD  | Alt  | Menu |Enter |
     * `-----------------------------------------------------------------------------------'
     */
    [_QWERTY] = LAYOUT_preonic_grid(
        KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_DEL,
        LT(_NAV, KC_ESC), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        KC_LSPO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSPC,
        KC_LCTL, KC_LGUI, KC_LALT, MO(_FN), LOWER, KC_SPC, KC_SPC, RAISE, KC_LEAD, KC_RALT, MO(_MENU), RCTL_T(KC_ENT)),

    /* Workman
     * ,-----------------------------------------------------------------------------------.
     * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Tab  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Del  |
     * |------+------+------+------+------+-------------+------+------+------+------+------|
     * | Esc  |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |  "   |
     * |------+------+------+------+------+------|------+------+------+------+------+------|
     * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  |Enter |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
     * `-----------------------------------------------------------------------------------'
     */
    [_WORKMAN] = LAYOUT_preonic_grid(
        KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
        KC_TAB, KC_Q, KC_D, KC_R, KC_W, KC_B, KC_J, KC_F, KC_U, KC_P, KC_SCLN, KC_DEL,
        LT(_NAV, KC_ESC), KC_A, KC_S, KC_H, KC_T, KC_G, KC_Y, KC_N, KC_E, KC_O, KC_I, KC_QUOT,
        KC_LSPO, KC_Z, KC_X, KC_M, KC_C, KC_V, KC_K, KC_L, KC_COMM, KC_DOT, KC_SLSH, KC_RSPC,
        KC_LCTL, KC_LGUI, KC_LALT, MO(_FN), LOWER, KC_SPC, KC_SPC, RAISE, KC_LEAD, KC_RALT, MO(_MENU), RCTL_T(KC_ENT)),

    /* Lower
     * ,-----------------------------------------------------------------------------------.
     * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
     * |------+------+------+------+------+-------------+------+------+------+------+------|
     * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Del  |
     * |------+------+------+------+------+-------------+------+------+------+------+------|
     * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  |   |
     * |------+------+------+------+------+------|------+------+------+------+------+------|
     * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | | Home | End  |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
     * `-----------------------------------------------------------------------------------'
     */
    [_LOWER] = LAYOUT_preonic_grid(
        KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
        KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
        KC_DEL, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
        _______, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, S(KC_NUHS), S(KC_NUBS), KC_LBRC, KC_RBRC, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY),

    /* Raise
     * ,-----------------------------------------------------------------------------------.
     * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Del  |
     * |------+------+------+------+------+-------------+------+------+------+------+------|
     * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |  \   |
     * |------+------+------+------+------+------|------+------+------+------+------+------|
     * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO # |ISO / | Pg Up| Pg Dn|      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
     * `-----------------------------------------------------------------------------------'
     */
    [_RAISE] = LAYOUT_preonic_grid(
        KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
        KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_DEL,
        KC_DEL, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,
        _______, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_NUHS, KC_NUBS, LCA(KC_MINUS), LCA(KC_UNDERSCORE), _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_NAV] = LAYOUT_preonic_grid(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, KC_PGDN, KC_UP,   KC_PGUP, KC_HOME, _______, _______, _______, KC_WH_D, KC_MS_U, KC_WH_U, _______,
        _______, KC_LEFT, KC_DOWN, KC_RIGHT, KC_END, _______, _______, KC_BTN1, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN2,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN3, _______, _______,
        _______, _______, WIN_TAB, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_FN] = LAYOUT_preonic_grid(
        _______, _______, _______, _______, _______, KC_SLCK, KC_NLCK, KC_P7,   KC_P8,   KC_P9,   KC_PPLS, _______,
        _______, KC_MUTE, KC_MPRV, KC_MNXT, _______, _______, _______, KC_P4,   KC_P5,   KC_P6,   KC_PMNS, _______,
        _______, KC_VOLU, KC_MSTP, KC_MPLY, _______, _______, _______, KC_P1,   KC_P2,   KC_P3,   KC_PAST, _______,
        KC_CAPS, KC_VOLD, _______, _______, _______, _______, _______, KC_P0,   KC_PCMM, KC_PDOT, KC_PSLS, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_PENT),

    [_MENU] = LAYOUT_preonic_grid(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, DM_REC1, DM_PLY1, _______, _______, _______, _______, _______, _______, _______, WIGGLER, _______,
        DM_RSTP, DM_REC2, DM_PLY2, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_ADJUST] = LAYOUT_preonic_grid(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, RESET,   DEBUG, EEP_RST, _______, _______, _______, _______, RGB_TOG, RGB_RMOD, RGB_MOD, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, RGB_HUI, RGB_SAI, RGB_VAI, _______,
        _______, QWERTY, WORKMAN, _______, _______, _______, _______, _______, RGB_HUD, RGB_SAD, RGB_VAD, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______)

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (keycode >= 4 && keycode <= 39) {
            char_count++;
        } else {
            last_word_len = char_count;
            char_count    = 0;
        }
    }
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            break;
        case WORKMAN:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_WORKMAN);
            }
            break;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            break;
        case WIN_TAB:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
        case WIGGLER:
            if (record->event.pressed) {
                wiggler = !wiggler;
                if (!wiggler) {
                    unregister_code(KC_MS_LEFT);
                    unregister_code(KC_MS_RIGHT);
                } else {
                    wiggle_timer = timer_read();
                }
            }
    }
    return true;
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case COMBO_DELETE_WORD:
            if (pressed) {
                last_word_len += char_count - 1;
                while (last_word_len > 0) {
                    tap_code(KC_BSPACE);
                    last_word_len--;
                }
            }
            break;
    }
}

bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 50;

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (muse_mode) {
        if (IS_LAYER_ON(_RAISE)) {
            if (clockwise) {
                muse_offset++;
            } else {
                muse_offset--;
            }
        } else {
            if (clockwise) {
                muse_tempo += 1;
            } else {
                muse_tempo -= 1;
            }
        }
    } else {
        if (clockwise) {
            register_code(KC_PGDN);
            unregister_code(KC_PGDN);
        } else {
            register_code(KC_PGUP);
            unregister_code(KC_PGUP);
        }
    }
    return true;
}

void dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if (active) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            break;
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
}

LEADER_EXTERNS();

void matrix_scan_user(void) {
    LEADER_DICTIONARY() {
        leading = false;
        leader_end();

        SEQ_ONE_KEY(KC_W) { SEND_STRING(SS_DOWN(X_RCTRL) SS_TAP(X_RIGHT) SS_UP(X_RCTRL)); }

        SEQ_ONE_KEY(KC_B) { SEND_STRING(SS_DOWN(X_RCTRL) SS_TAP(X_LEFT) SS_UP(X_RCTRL)); }

        SEQ_ONE_KEY(KC_O) {
            if (get_mods() & MOD_MASK_SHIFT) {
                SEND_STRING(SS_TAP(X_HOME) SS_UP(X_LSFT) SS_UP(X_RSFT) SS_TAP(X_ENTER) SS_TAP(X_UP));
            } else {
                SEND_STRING(SS_TAP(X_END) SS_TAP(X_ENTER));
            }
        }

        // vim dd
        SEQ_TWO_KEYS(KC_D, KC_D) { SEND_STRING(SS_TAP(X_HOME) SS_DOWN(X_LSFT) SS_TAP(X_END) SS_UP(X_LSFT) SS_TAP(X_DEL)); }
        // vim gg
        SEQ_TWO_KEYS(KC_G, KC_G) { SEND_STRING(SS_DOWN(X_LCTL) SS_TAP(X_HOME) SS_UP(X_LCTL)); }
        // open camunda imported dir for businesskei in clipboard
        SEQ_TWO_KEYS(KC_C, KC_O) { SEND_STRING(SS_LGUI(SS_TAP(X_R)) "\\\\p081\\ddmppl\\prod\\_camundaImported\\" SS_RCTL(SS_TAP(X_V)) SS_TAP(X_ENTER)); }
    }
    // WIN TAB TIMER
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1000) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
        }
    }
    // WIGGLE TIMER
    if (wiggler) {
        if (timer_elapsed(wiggle_timer) > 1000) {
            wiggle_timer = 0;
            wiggle_timer = timer_read();
            if (wiggle_direction == LEFT) {
                unregister_code(KC_MS_RIGHT);
                register_code(KC_MS_LEFT);
                wiggle_direction = RIGHT;
            } else {
                unregister_code(KC_MS_LEFT);
                register_code(KC_MS_RIGHT);
                wiggle_direction = LEFT;
            }
        }
    }
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
        case RAISE:
        case LOWER:
            return false;
        default:
            return true;
    }
}

const char *inttostr(int n) {
    char *result;
    if (n >= 0)
        result = malloc(floor(log10(n)) + 2);
    else
        result = malloc(floor(log10(n)) + 3);
    sprintf(result, "%d", n);
    return result;
}
