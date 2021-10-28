/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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
#include <stdio.h>
#include <keymap.h>

// Debug
#if defined CONSOLE_ENABLE
#include <print.h>
#endif

// Mouse stuff
#if defined PS2_MOUSE_ENABLE
#include "ps2_mouse.h"
#endif

#if defined MOUSEKEY_ENABLE
#include "mousekey.h"
#endif

#if defined MH_AUTO_BUTTONS
//#include MH_USER_NAME_H // for MH_AUTO_BUTTONS_LAYER
#endif

#if defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE

static uint16_t mh_auto_buttons_timer;
extern int tp_buttons; // mousekey button state set in action.c and used in ps2_mouse.c

void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
  if (mh_auto_buttons_timer) {
    mh_auto_buttons_timer = timer_read();
  } else {
    if (!tp_buttons) {
      layer_on(MH_AUTO_BUTTONS_LAYER);
      mh_auto_buttons_timer = timer_read();
  #if defined CONSOLE_ENABLE
      print("mh_auto_buttons: on\n");
  #endif
    }
  }
}

void matrix_scan_user(void) {
  if (mh_auto_buttons_timer && (timer_elapsed(mh_auto_buttons_timer) > MH_AUTO_BUTTONS_TIMEOUT)) {
    if (!tp_buttons) {
      layer_off(MH_AUTO_BUTTONS_LAYER);
      mh_auto_buttons_timer = 0;
  #if defined CONSOLE_ENABLE
      print("mh_auto_buttons: off\n");
  #endif
    }
  }
}

#endif // defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && #defined MOUSEKEY_ENABLE


// Keymap macros
#define LTHM3 MT(MOD_LCTL,KC_TAB)
#define LTHM1 MT(MOD_LGUI,KC_ENT)
#define RTHM1 MT(MOD_LSFT|MOD_RSFT,KC_SPC)
#define RTHM3 MT(MOD_LALT|MOD_RALT,KC_ESC)

#define MC_LANG   LCTL(LALT(KC_SPC))    // Switch keyboard language
#define MC_WNDF   RCTL(RALT(KC_DOWN))   // Spectacle: fullsize window
#define MC_WNDL   RCTL(RALT(KC_LEFT))   // Spectacle: left half
#define MC_WNDR   RCTL(RALT(KC_RGHT))   // Spectacle: right half
#define MC_SLP    LGUI(LCTL(KC_Q))      // Put display to sleep
#define MC_SCR0   LGUI(LSFT(KC_3))      // Take screenshot
#define MC_SCR1   LGUI(LSFT(KC_4))      // Take partial screenshot

#define MC_SFAL   MT(MOD_LSFT|MOD_LALT|MOD_RSFT|MOD_RALT,KC_NO) // Shift-alt

enum custom_keycodes {
    MC_VSAV = SAFE_RANGE,
    MC_VSPL,
    MC_VQUI,
    MC_VTAB,
    MC_JSCL,
    SA_COPY,
};

// Keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
         KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
         KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                          KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                   LTHM3,   MO(1),    LTHM1,     RTHM1,   MO(2),   RTHM3 
                              //`--------------------------'  `--------------------------'
  ),

  [1] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
         KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
       KC_ESC,  KC_ENT, KC_PGUP, KC_PGDN, KC_SPC,                       KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, KC_MINS,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_PDOT, KC_SLSH,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                 KC_TRNS, _______, KC_TRNS,    KC_TRNS,   MO(3), KC_TRNS
                              //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
      MC_VQUI, MC_VSAV, XXXXXXX, XXXXXXX, MC_LANG,                       KC_ENT, KC_SCLN, KC_COLN, KC_MINS, KC_EQL,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      KC_LGUI, KC_LALT, KC_LCTL,  KC_TAB, KC_CAPS,                      KC_QUOT,  KC_GRV, KC_RSFT, KC_LBRC, KC_RBRC,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      MC_VSPL, MC_VTAB, MC_SCR1, MC_SCR0, MC_JSCL,                      KC_BSPC, KC_BSLS, KC_PIPE, KC_LPRN, KC_RPRN,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                 KC_TRNS,  MO(3), KC_TRNS,     KC_TRNS, _______, KC_TRNS
                              //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
        RESET, RGB_HUI, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  MC_SLP,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAI, XXXXXXX, MC_SFAL,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_LSFT, KC_LALT, XXXXXXX,                      MC_WNDL, MC_WNDF, MC_WNDR, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                 KC_TRNS, _______, KC_TRNS,     KC_TRNS, _______, KC_TRNS
                              //`--------------------------'  `--------------------------'
  ),

  /* MOUSE BUTTON LAYER */
  [4] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                 KC_BTN2, KC_BTN3, KC_BTN1,    KC_BTN1, KC_BTN3, KC_BTN2
                              //`--------------------------'  `--------------------------'
  )
};

// OLED debugging
#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

#define L_BASE 0
#define L_1 2
#define L_2 4
#define L_3 8

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("Base"), false);
            break;
        case L_1:
            oled_write_ln_P(PSTR("Num"), false);
            break;
        case L_2:
            oled_write_ln_P(PSTR("Nav/Sym"), false);
            break;
        case L_3:
        case L_3|L_1:
        case L_3|L_2:
        case L_3|L_2|L_1:
            oled_write_ln_P(PSTR("Special"), false);
            break;
    }
}


char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
    }
}
#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {  // on keypress
        // show keycode on OLED
        #ifdef OLED_ENABLE
        set_keylog(keycode, record);
        #endif // OLED_ENABLE

        // process custom keycodes
        switch(keycode) {
            case MC_VSAV: SEND_STRING(":w"SS_TAP(X_ENT)); break;
            case MC_VQUI: SEND_STRING(":q"SS_TAP(X_ENT)); break;
            case MC_VSPL: SEND_STRING(":sp"); break;
            case MC_VTAB: SEND_STRING("s/^  /"); break;
            case MC_JSCL: SEND_STRING(" => {}"); break;
            case SA_COPY: SEND_STRING(SS_LCTL("ac")); break;  // example str
        }
    }
    return true;
}

