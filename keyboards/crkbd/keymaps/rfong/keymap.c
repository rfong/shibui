/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>
Copyright 2020 @manna-harbour
Copyright 2021 @rfong

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

/* TODO
 * - split transfer keylog data to left half
 * - finish making pet jump
 * - allow OLED to sleep
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

/* BEGIN TAPDANCE SETUP */

// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    TDL1,  // Momentary = layer 1, doubletap = layer 5
    TDL2,  // Momentary = layer 2, doubletap = layer 6
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
td_state_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(qk_tap_dance_state_t *state, void *user_data, layer_state_t hold_layer, layer_state_t dt_layer);
void ql_finished_left(qk_tap_dance_state_t *state, void *user_data);
void ql_finished_right(qk_tap_dance_state_t *state, void *user_data);
void ql_reset(qk_tap_dance_state_t *state, void *user_data, layer_state_t hold_layer);
void ql_reset_left(qk_tap_dance_state_t *state, void *user_data);
void ql_reset_right(qk_tap_dance_state_t *state, void *user_data);

/* END TAPDANCE SETUP */

/* Activate mouse button layer with PS2 mouse - crkbd:manna-harbour */
#if defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE

static uint16_t mh_auto_buttons_timer;
extern int tp_buttons; // mousekey button state set in action.c and used in ps2_mouse.c

// If mouse has moved recently, activate mousebutton layer
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


/* BEGIN KEYMAP */

/* Custom macros */
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

// seems to be broken in iterm
#define MC_SFAL   RSA_T(KC_SPC)         // Shift-alt when held, space tap

enum custom_keycodes {
    MC_VSAV = SAFE_RANGE,
    MC_VSPL,
    MC_VQUI,
    MC_VTAB,
    MC_JSCL,
    SA_COPY,
};

/* Keymap */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* BASE (ALPHAS) */
  [0] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
         KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
         KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_QUOT,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
         KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                   LTHM3,TD(TDL1),    LTHM1,     RTHM1,TD(TDL2),   RTHM3 
                              //`--------------------------'  `--------------------------'
  ),

  /* NUMROW & SYMBOLS */
  [1] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
         KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
       KC_ESC,  KC_ENT, KC_PGUP, KC_PGDN, MC_SFAL,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, KC_MINS,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_PDOT, KC_SLSH,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                 KC_TRNS, _______, KC_TRNS,    KC_TRNS,   MO(3), KC_TRNS
                              //`--------------------------'  `--------------------------'
  ),

  /* VIM */
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

  /* SPECIALS & MOUSE */
  [3] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
        RESET, RGB_HUI, KC_VOLD, KC_VOLU, KC_MUTE,                      XXXXXXX, KC_BTN1, KC_BTN2, XXXXXXX,  MC_SLP,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, KC_MPRV, KC_MNXT, KC_MPLY,                      KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      KC_BRID, KC_BRIU, KC_LSFT, KC_LALT, XXXXXXX,                      MC_WNDL, MC_WNDF, MC_WNDR, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                 KC_TRNS, _______, KC_TRNS,     KC_TRNS, _______, KC_TRNS
                              //`--------------------------'  `--------------------------'
  ),

  /* MOMENTARY MOUSE BUTTONS FOR TRACKPOINT */
  [4] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_LCTL, KC_LGUI, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_LSFT, KC_LALT, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                 KC_BTN2, KC_BTN3, KC_BTN1,    KC_BTN1, KC_BTN3, KC_BTN2
                              //`--------------------------'  `--------------------------'
  ),


  /* MATH LAYER (TODO) */
  [5] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,    KC_5, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                 KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS
                              //`--------------------------'  `--------------------------'
  ),

  /* DEDICATED NAV LAYER (TODO) */
  [6] = LAYOUT_split_3x5_3(
  //,--------------------------------------------.                    ,--------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, XXXXXXX,
  //|--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_ENT, KC_PGUP, KC_PGDN, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+---------|  |-------+--------+--------+--------+--------+--------|
                                 KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS
                              //`--------------------------'  `--------------------------'
  )
};

#ifndef OLED_ENABLE
/* Special keycode processing */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {  // on keypress
        // process custom keycodes
        switch(keycode) {
            case MC_VSAV: SEND_STRING(":w"SS_TAP(X_ENT)); break;
            case MC_VQUI: SEND_STRING(":q"SS_TAP(X_ENT)); break;
            case MC_VSPL: SEND_STRING(":sp "); break;
            case MC_VTAB: SEND_STRING("s/^  /"); break;
            case MC_JSCL: SEND_STRING(" => {}"); break;
            case SA_COPY: SEND_STRING(SS_LCTL("ac")); break;  // example str
        }
    }
    return true;
}
#endif // OLED_ENABLE

/* END KEYMAP */

/* keyboard pet jump status variables */
bool isJumping = false;
bool showedJump = true;

/* This section from crkbd:default
 * Luna qmk pet from HellSingCoder
 * Send debug info to OLED */
#ifdef OLED_ENABLE

static const char PROGMEM layer_names[7][5] = {
  "Base",
  "Num",
  "Sym",
  "Fn",
  "     ",
  "Math",
  "Nav",
};

// recall that layer_state flips bits for each layer that is activated;
// multiple layers may be active
void oled_render_layer_state(void) {
    oled_set_cursor(0,0);
    oled_write_P(PSTR("Layer"), false);
    if (IS_LAYER_ON(MH_AUTO_BUTTONS_LAYER)) {
        oled_write_ln_P(PSTR("Mouse"), false);
        return;
    }
    // Loop through layers in reverse. Write & quit on the first active one.
    for(int i = 7; i >= 0; i--) {
        if (IS_LAYER_ON(i)) {
            oled_write_ln_P(layer_names[i], false);
            oled_write_ln_P(PSTR("     "), false);
            return;
        }
    }
    oled_write_ln_P(PSTR("???"), false);
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
  bool on_right = (record->event.key.row < 4);
  snprintf(keylog_str, sizeof(keylog_str), "%c%dx%d %c(%2d)   ",
           on_right ? 'R' : 'L',
           record->event.key.row % 4, record->event.key.col,
           name, keycode);
}

/*
void oled_render_keylog(void) {
    oled_set_cursor(0,5);
    oled_write(keylog_str, false);
}
*/

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {  // on keypress
        // show keycode on OLED
        set_keylog(keycode, record);
        switch(keycode) {
            case KC_ENT: 
						case KC_TAB:
						case KC_ESC:
                /*Keyboard Pet jump*/
                isJumping = true;
                showedJump = false;
								return false; break;
        }
    }
    return true;
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

/* Keyboard Pet settings */
#define MIN_WALK_SPEED 10
#define MIN_RUN_SPEED 50
#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
#define ANIM_SIZE 96 // number of bytes per image

/* Keyboard Pet status variables */
led_t led_usb_state;
uint8_t current_wpm = 0;
uint8_t current_frame = 0;
uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;

/* Keyboard Pet logic */
static void render_keyboard_pet(int KEYBOARD_PET_X, int KEYBOARD_PET_Y) {

    /* Sit */
    static const char PROGMEM sit[2][ANIM_SIZE] = {
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c,
            0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28,
            0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        },
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c,
            0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28,
            0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        }
    };

    /* Walk */
    static const char PROGMEM walk[2][ANIM_SIZE] = {
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80,
            0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03,
            0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        },
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00,
            0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
            0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e,
            0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        }
    };

    /* Run */
    static const char PROGMEM run[2][ANIM_SIZE] = {
        {
            0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
            0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01,
            0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00
        },
        {
            0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
            0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37,
            0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        }
    };

    /* Bark */
    static const char PROGMEM bark[2][ANIM_SIZE] = {
        {
            0x00, 0xc0, 0x20, 0x10, 0xd0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40,
            0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02,
            0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        },
        {
            0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40,
            0x40, 0x2c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x20, 0x4a, 0x09, 0x10,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02,
            0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        }
    };

    /* animation */
    void animate_keyboard_pet(void) {

        /* jump */
        if (isJumping || !showedJump) {
            oled_set_cursor(KEYBOARD_PET_X,KEYBOARD_PET_Y +2);
            oled_write_P(PSTR("     "), false);
            oled_set_cursor(KEYBOARD_PET_X,KEYBOARD_PET_Y -1);
            showedJump = true;
        } else {
            oled_set_cursor(KEYBOARD_PET_X,KEYBOARD_PET_Y -1);
            oled_write_P(PSTR("     "), false);
            oled_set_cursor(KEYBOARD_PET_X,KEYBOARD_PET_Y);
        }

        /* switch frame */
        current_frame = (current_frame + 1) % 2;

        /* current animation status */
        if(led_usb_state.caps_lock) {
            oled_write_raw_P(bark[abs(1 - current_frame)], ANIM_SIZE);
        } else if(current_wpm <= MIN_WALK_SPEED) {
            oled_write_raw_P(sit[abs(1 - current_frame)], ANIM_SIZE);
        } else if(current_wpm <= MIN_RUN_SPEED) {
            oled_write_raw_P(walk[abs(1 - current_frame)], ANIM_SIZE);
        } else {
            oled_write_raw_P(run[abs(1 - current_frame)], ANIM_SIZE);
        }
    }

    /* animation timer */
    if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        if(timer_elapsed32(anim_sleep) < OLED_TIMEOUT) {
            animate_keyboard_pet();
        }
    }

    if (current_wpm > 0) {
        anim_sleep = timer_read32();
    } else if(timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
        //oled_off();
    }

}

static void print_status_narrow(void) {

    /* Print current OS */
    oled_set_cursor(2,0);
    if (keymap_config.swap_lctl_lgui) {
        oled_write_P(PSTR("\x9E"), false);
    } else {
        oled_write_P(PSTR("\x9F"), false);
    }

    /* Print current layer */
    oled_render_layer_state();

    /* caps lock */
    oled_set_cursor(0,3);
    if (led_usb_state.caps_lock) {
      oled_write_P(PSTR("CAPS"), false);
    } else {
      oled_write_P(PSTR("    "), false);
    }

    //oled_render_keylog();

   /* Joystick debugging */
#      ifdef JOYSTICK_ENABLE
        if (joystick_debug) {
            oled_set_cursor(0,10);
            oled_write_P(PSTR("X:"), false);
            uint16_t val = analogReadPin(JOYSTICK_X_PIN);
            char val_str[3];
            itoa(val, val_str, 10);
            oled_write(val_str, false);
            oled_set_cursor(0,11);
            oled_write_P(PSTR("Y:"), false);
            val = analogReadPin(JOYSTICK_Y_PIN);
            itoa(val, val_str, 10);
            oled_write(val_str, false);
        } else {
            oled_set_cursor(0,10);
            oled_write_P(PSTR("     "), false);
            oled_set_cursor(0,11);
            oled_write_P(PSTR("     "), false);
        }
#       endif

    /* Keyboard Pet Render */
    render_keyboard_pet(0,13);
}

/* vertical screens orientation fix */
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

//bool oled_task_user(void) {
void oled_task_user(void) {
    /* Keyboard Pet Variables */
    current_wpm = get_current_wpm();
    led_usb_state = host_keyboard_led_state();
    print_status_narrow();
}

#endif // OLED_ENABLE

/* BEGIN TAPDANCE FUNCTIONALITY */

// Determine the current tap dance state
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) return TD_DOUBLE_TAP;
    else return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Functions that control what our tap dance key does
void ql_finished(qk_tap_dance_state_t *state, void *user_data, layer_state_t hold_layer, layer_state_t dt_layer) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_on(hold_layer);
            break;
        case TD_DOUBLE_TAP:  // Toggle
            // Check to see if the layer is already set
            if (layer_state_is(dt_layer)) {
                // If already set, then switch it off
                layer_off(dt_layer);
            } else {
                // If not already set, then move to this layer.
                // (Note: if you need nested layers, use layer_on instead)
                layer_move(dt_layer);
            }
            break;
        default:
            break;
    }
}
void ql_finished_left(qk_tap_dance_state_t *state, void *user_data) {
    ql_finished(state, user_data, 1, 5);
}
void ql_finished_right(qk_tap_dance_state_t *state, void *user_data) {
    ql_finished(state, user_data, 2, 6);
}

void ql_reset(qk_tap_dance_state_t *state, void *user_data, layer_state_t hold_layer) {
    // If the key was held down and now is released then switch off the layer
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(hold_layer);
    }
    ql_tap_state.state = TD_NONE;
}
void ql_reset_left(qk_tap_dance_state_t *state, void *user_data) {
    ql_reset(state, user_data, 1);
}
void ql_reset_right(qk_tap_dance_state_t *state, void *user_data) {
    ql_reset(state, user_data, 2);
}

// Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [TDL1] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, ql_finished_left, ql_reset_left, 275),
    [TDL2] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, ql_finished_right, ql_reset_right, 275)
};

/* END TAPDANCE FUNCTIONALITY */
