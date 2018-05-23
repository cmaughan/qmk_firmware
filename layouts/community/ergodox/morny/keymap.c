#include <stdarg.h>
#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "sendchar.h"
#include "virtser.h"
#include "led.h"
#include "action_util.h"
#include "timer.h"
#include "eeconfig.h"
#include "wait.h"

// Layers
#define BASE 0 // default layer
#define NUMP 1 // numpad
#define ARRW 2 // function, media, arrow keys
#define TXBOLT 3 // function, media, arrow keys

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

enum { LOCKS = 0 };

/* Custom keycodes */

enum {
  CT_SR
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // If it accepts an argument (i.e, is a function), it doesn't need KC_.
  // Otherwise, it needs KC_*
  /* layer 0 : default
   *
   * ,--------------------------------------------------.           ,--------------------------------------------------.
   * | `      |   1  |   2  |   3  |   4  |   5  |   6  |           |  7   |   8  |   9  |   0  |   -  |   =  | BCKSPC |
   * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
   * | TAB    |   Q  |   W  |   E  |   R  |   T  | HOME |           | PGUP |   Y  |   U  |   I  |   O  |   P  | DELETE |
   * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
   * | '      |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |  ENTER |
   * |--------+------+------+------+------+------|  END |           | PGDN |------+------+------+------+------+--------|
   * | (/LSFT |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |  UP  | )/RSFT |
   * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
   * | LCTRL  | LGUI | MENU | LEAD | LALT |                                       |[/RALT|]/RCTL| LEFT | DOWN |  RIGHT |
   * `------------------------------------'                                       `------------------------------------'
   *                                        ,-------------.       ,-------------.
   *                                        |PRTSCR| ESC  |       | VOL- | VOL+ |
   *                                 ,------|------|------|       |------+------+------.
   *                                 |      |SLASH/| LOCKS|       | MUTE |WHACK/|      |
   *                                 |  SPC | MO(4)|------|       |------|MO(4) | SPC  |
   *                                 |      |      | TXBLT|       | NUMP |      |      |
   *                                 `--------------------'       `--------------------'
   */
  [BASE] = LAYOUT_ergodox(
    // left hand
    KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,
    KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_HOME,
    KC_QUOT,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,
    KC_LSPO,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_END,
    KC_LCTL,    KC_LGUI,    KC_MENU,    KC_LEAD,    KC_LALT,
                                                                KC_PSCR,    KC_ESC,
                                                                            TD(LOCKS),
                                            KC_SPC,     LT(ARRW, KC_SLSH),  TG(TXBOLT),
    // right hand
        KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,
        KC_PGUP,    KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_DEL,
                    KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_ENTER,
        KC_PGDN,    KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_UP,      KC_RSPC,
                    MT(MOD_RALT, KC_LBRC),  MT(MOD_RCTL, KC_RBRC),  KC_LEFT,    KC_DOWN,    KC_RGHT,
        KC_VOLD,    KC_VOLU,
        KC_MUTE,
        TG(NUMP),   LT(ARRW, KC_BSLS),   KC_SPC
),

/* layer 1: numberpad
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |   (  |   )  |   /  |  *   |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |   7  |   8  |   9  |  -   |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      |   4  |   5  |   6  |  +   |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |   1  |   2  |   3  |  =   |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 * |        |      |      |      |      |                                       |0/RALT|./RCTL|   ,  | ENTER|        |
 * `------------------------------------'                                       `------------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[NUMP] = LAYOUT_ergodox(
  // left hand
  _______,    _______,    _______,    _______,    _______,    _______,    _______,
  _______,    _______,    _______,    _______,    _______,    _______,    _______,
  _______,    _______,    _______,    _______,    _______,    _______,
  _______,    _______,    _______,    _______,    _______,    _______,    _______,
  _______,    _______,    _______,    _______,    _______,
                                      _______,    _______,
                                                  _______,
                      _______,        _______,    _______,
  // right hand
      _______,    _______,    S(KC_9),    S(KC_0),    KC_PSLS,    KC_PAST,    _______,
      _______,    _______,    KC_KP_7,    KC_KP_8,    KC_KP_9,    KC_PMNS,    _______,
                  _______,    KC_KP_4,    KC_KP_5,    KC_KP_6,    KC_PPLS,    _______,
      _______,    _______,    KC_KP_1,    KC_KP_2,    KC_KP_3,    KC_PEQL,    _______,
                              MT(MOD_RALT, KC_KP_0),  MT(MOD_RCTL, KC_KP_DOT),KC_PCMM,    KC_PENT,    _______,
      _______,    _______,
      _______,
      _______,    _______,    _______
),

/* layer 2 : functions and arrows
 * This layer is at the top so that the functions still work no matter what layers are active.
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | ESCAPE |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  | F10  | F11  | F12  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |  UP  |      |      |      |           |      |      |      |      |      |      | INSERT |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      | LEFT | DOWN |RIGHT |      |------|           |------| LEFT | DOWN |  UP  | RIGHT|      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |M_PREV|M_STOP|M_PLPS|M_NEXT|      |      |           |      |      |      |      |      | PGUP |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 * |        |      |      |      |      |                                       |      |      | HOME | PGDN |    END |
 * `------------------------------------'                                       `------------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | SYSRQ| PAUSE|       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[ARRW] = LAYOUT_ergodox(
  // left hand
  KC_ESC,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,
  _______,    _______,    _______,    KC_UP,      _______,    _______,    _______,
  _______,    _______,    KC_LEFT,    KC_DOWN,    KC_RGHT,    _______,
  _______,    KC_MPRV,    KC_MSTP,    KC_MPLY,    KC_MNXT,    _______,    _______,
  _______,    _______,    _______,    _______,    _______,
                                                              KC_SYSREQ,  KC_PAUSE,
                                                                          _______,
                                              _______,        _______,    _______,
  // right hand
      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,
      _______,    _______,    _______,    _______,    _______,    _______,    KC_INS,
                  KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,    _______,    _______,
      _______,    _______,    _______,    _______,    _______,    KC_PGUP,    _______,
                              _______,    _______,    KC_HOME,    KC_PGDN,    KC_END,
      _______,    _______,
      _______,
      _______,    _______,    _______
),
// TxBolt Codes
#define Sl 0b00000001
#define Tl 0b00000010
#define Kl 0b00000100
#define Pl 0b00001000
#define Wl 0b00010000
#define Hl 0b00100000
#define Rl 0b01000001
#define Al 0b01000010
#define Ol 0b01000100
#define X  0b01001000
#define Er 0b01010000
#define Ur 0b01100000
#define Fr 0b10000001
#define Rr 0b10000010
#define Pr 0b10000100
#define Br 0b10001000
#define Lr 0b10010000
#define Gr 0b10100000
#define Tr 0b11000001
#define Sr 0b11000010
#define Dr 0b11000100
#define Zr 0b11001000
#define NM 0b11010000
#define GRPMASK 0b11000000
#define GRP0 0b00000000
#define GRP1 0b01000000
#define GRP2 0b10000000
#define GRP3 0b11000000
/* Keymap 3: TxBolt (Serial)
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | BKSPC  |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   #  |   #  |   #  |   #  |   #  |      |           |      |   #  |   #  |   #  |   #  |   #  |   #    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   S  |   T  |   P  |   H  |   *  |------|           |------|   *  |   F  |   P  |   L  |   T  |   D    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   S  |   K  |   W  |   R  |   *  |      |           |      |   *  |   R  |   B  |   G  |   S  |   Z    |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |   A  |   O  |------|       |------|   E  |   U  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
 // TxBolt over Serial
 [TXBOLT] = KEYMAP(
        KC_BSPC, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   M(NM),   M(NM),   M(NM),   M(NM),   M(NM),  KC_NO,
        KC_NO,   M(Sl),   M(Tl),   M(Pl),   M(Hl),   M(X),
        KC_NO,   M(Sl),   M(Kl),   M(Wl),   M(Rl),   M(X),   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                            KC_NO,   KC_NO,
                                                     KC_NO,
                                   M(Al),   M(Ol),   KC_TRNS,
   // right hand
      KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_NO,  M(NM),   M(NM),   M(NM),   M(NM),   M(NM),   M(NM),
                M(X),    M(Fr),   M(Pr),   M(Lr),   M(Tr),   M(Dr),
      KC_NO,    M(X),    M(Rr),   M(Br),   M(Gr),   M(Sr),   M(Zr),
                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_NO,   KC_NO,
      KC_NO,
      KC_TRNS,   M(Er),   M(Ur)
),
};

const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE
(
  UCIS_SYM("poop", 0x1f4a9)
);

// Tap Dancing
void dance_lock(qk_tap_dance_state_t *state, void *user_data)
{
  if (state->count == 1) { // Press once for NUMLOCK
    register_code(KC_NLCK);
    unregister_code(KC_NLCK);
  }
  else if (state->count == 2) { // Press twice for CAPSLOCK
    register_code(KC_CAPS);
    unregister_code(KC_CAPS);
  }
  else if (state->count == 3) { //Press thrice for SCROLLLOCK
    register_code(KC_SLCK);
    unregister_code(KC_SLCK);
  }
}

qk_tap_dance_action_t tap_dance_actions[] =
{
    [LOCKS] = ACTION_TAP_DANCE_FN(dance_lock)
};

uint8_t chord[4] = { 0,0,0,0 };
uint8_t pressed_count = 0;

void send_chord(void)
{
  for (uint8_t i = 0; i < 4; i++)
  {
    if (chord[i])
      virtser_send(chord[i]);
  }
  virtser_send(0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  // We need to track keypresses in all modes, in case the user
  // changes mode whilst pressing other keys.
  if (record->event.pressed)
    pressed_count++;
  else
    pressed_count--;
  return true;
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function

  if (record->event.pressed) {
    uint8_t grp = (id & GRPMASK) >> 6;
    chord[grp] |= id;
  }
  else {
    if (pressed_count == 0) {
      send_chord();
      chord[0] = chord[1] = chord[2] = chord[3] = 0;
    }
  }
  return MACRO_NONE;
};
// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  //set_unicode_input_mode(UC_LNX); // Linux
  //set_unicode_input_mode(UC_OSX); // Mac OSX
  set_unicode_input_mode(UC_WIN); // Windows (with registry key, see wiki)
  //set_unicode_input_mode(UC_WINC); // Windows (with WinCompose, see wiki)
};

LEADER_EXTERNS();

// Runs constantly in the background, in a loop.
void matrix_scan_user(void)
{

  uint8_t layer = biton32(layer_state);

  ergodox_board_led_off();
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();
  switch (layer)
  {
  case NUMP:
    ergodox_right_led_1_on();
    break;
  case ARRW:
    ergodox_right_led_2_on();
    break;
  case TXBOLT:
    ergodox_right_led_3_on();
    break;
  default:
    // none
    break;
  }
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

    SEQ_THREE_KEYS(KC_F, KC_K, KC_N)
    {
      //uprintf("CMD:reflash\n");
      wait_ms(1000);
      reset_keyboard();
    }
  }
};


