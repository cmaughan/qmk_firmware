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

// My ergodox setup; portions taken from algernon's excellent modifications.
// Intended as an easy way to transition to the keyboard for a touch typist, used to things in certain
// places.  But will likely evolve over time to suit me
#define BASE 0 // default layer
#define SYMB 1 // Symbols/numpad
#define TXBOLT 2 // function, media, arrow keys

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

/* Fn keys */
enum {
  F_SFT,
  F_ALT,
  F_CTRL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = 
{
  // If it accepts an argument (i.e, is a function), it doesn't need KC_.
  // Otherwise, it needs KC_*
  /* layer 0 : default
   *
   * ,--------------------------------------------------.           ,--------------------------------------------------.
   * |        |   1  |   2  |   3  |   4  |   5  |   6  |           |  7   |   8  |   9  |   0  |   -  |   =  | BCKSPC |
   * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
   * | TAB    |   Q  |   W  |   E  |   R  |   T  | HOME |           | PGUP |   Y  |   U  |   I  |   O  |   P  | DELETE |
   * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
   * |        |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |  ENTER |
   * |--------+------+------+------+------+------|  END |           | PGDN |------+------+------+------+------+--------|
   * | (/LSFT |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  | )/RSFT |
   * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
   * | LCTRL  | LGUI | ALT  |      | SYMB\ |                                      |SYMB/ | ALT  | CTRL |RIGHT |     |
   * `------------------------------------'                                       `------------------------------------'
   *                                        ,-------------.       ,-------------.
   *                                        | LEAD |TXBOLT|       |      |      |
   *                                 ,------|------|------|       |------+------+------.
   *                                 |      |      |      |       |      |      |      |
   *                                 | DEL  |      |------|       |------|      | SPC  |
   *                                 |      |      | ESC  |       |(SYMB)|      |      |
   *                                 `--------------------'       `--------------------'
   */
  [BASE] = LAYOUT_ergodox(
    // left hand
    _______,     KC_1,       KC_2,       KC_3,       KC_4,                KC_5,       KC_6,
    KC_TAB,      KC_Q,       KC_W,       KC_E,       KC_R,                KC_T,       KC_HOME,
    _______,     KC_A,       KC_S,       KC_D,       KC_F,                KC_G,
    KC_LSPO,     KC_Z,       KC_X,       KC_C,       KC_V,                KC_B,       KC_END,
    KC_LCTL,     KC_LGUI,    F(F_ALT),   _______,    LT(SYMB, KC_BSLS),
                                                                                        KC_LEAD,    TG(TXBOLT),
                                                                                                    _______,
                                                                                KC_DEL, _______,    KC_ESC,
    // right hand
        KC_7,       KC_8,                KC_9,                   KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,
        KC_PGUP,    KC_Y,                KC_U,                   KC_I,       KC_O,       KC_P,       KC_DEL,
                    KC_H,                KC_J,                   KC_K,       KC_L,       KC_SCLN,    KC_ENTER,
        KC_PGDN,    KC_N,                KC_M,                   KC_COMM,    KC_DOT,     KC_SLSH,    KC_RSPC,
                    LT(SYMB, KC_SLSH),   MT(MOD_RCTL, KC_RBRC),  F(F_ALT),   F(F_CTRL),  KC_RGHT,
        _______,    _______,
        _______,
        TG(SYMB),   _______,   KC_SPC
),

/* Keymap 1: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |Version  |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |   !  |   @  |   {  |   }  |   |  |      |           |      |   Up |   7  |   8  |   9  |   *  |   F12  |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   #  |   $  |   (  |   )  |   `  |------|           |------| Down |   4  |   5  |   6  |   +  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   %  |   ^  |   [  |   ]  |   ~  |      |           |      |   &  |   1  |   2  |   3  |   \  |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |    . |   0  |   =  |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = LAYOUT_ergodox(
       // left hand
       _______,   KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,_______,
       _______,KC_EXLM,KC_AT,  KC_LCBR,KC_RCBR,KC_PIPE,_______,
       _______,KC_HASH,KC_DLR, KC_LPRN,KC_RPRN,KC_GRV,
       _______,KC_PERC,KC_CIRC,KC_LBRC,KC_RBRC,KC_TILD,_______,
       _______,_______,_______,_______,_______,
                                       _______,_______,
                                               _______,
                               _______,_______,_______,
       // right hand
       _______, KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
       _______, KC_UP,   KC_7,   KC_8,    KC_9,    KC_ASTR, KC_F12,
                KC_DOWN, KC_4,   KC_5,    KC_6,    KC_PLUS, _______,
       _______, KC_AMPR, KC_1,   KC_2,    KC_3,    KC_BSLS, _______,
                         _______,KC_DOT,  KC_0,    KC_EQL,  _______,
       _______, _______,
       _______,
       _______, _______, _______
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
        KC_NO,   KC_NO,   KC_NO,   _______,   _______,
                                              _______,  _______,
                                                        _______,
                                      M(Al),   M(Ol),   _______,
   // right hand
      KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      KC_NO,    M(NM),   M(NM),   M(NM),   M(NM),   M(NM),   M(NM),
                M(X),    M(Fr),   M(Pr),   M(Lr),   M(Tr),   M(Dr),
      KC_NO,    M(X),    M(Rr),   M(Br),   M(Gr),   M(Sr),   M(Zr),
                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
      _______,   _______,
      _______,
      _______,   M(Er),   M(Ur)
)
};

const uint16_t PROGMEM fn_actions[] = 
{
   [F_SFT]  = ACTION_MODS_ONESHOT (MOD_LSFT)
  ,[F_ALT]  = ACTION_MODS_ONESHOT (MOD_LALT)
  ,[F_CTRL] = ACTION_MODS_ONESHOT (MOD_LCTL)
};

const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE
(
 UCIS_SYM("poop", 0x1f4a9),
 UCIS_SYM("rofl", 0x1f923),
 UCIS_SYM("kiss", 0x1f619),
 UCIS_SYM("snowman", 0x2603),
 UCIS_SYM("coffee", 0x2615),
 UCIS_SYM("heart", 0x2764),
 UCIS_SYM("bolt", 0x26a1),
 UCIS_SYM("pi", 0x03c0),
 UCIS_SYM("mouse", 0x1f401),
 UCIS_SYM("micro", 0x00b5),
 UCIS_SYM("tm", 0x2122),
 UCIS_SYM("child", 0x1f476),
 UCIS_SYM("family", 0x1F46A)
);

/*
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
*/

qk_tap_dance_action_t tap_dance_actions[] =
{
//    [LOCKS] = ACTION_TAP_DANCE_FN(dance_lock)
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
  // CM: This is for the STENO mode to detect when the chord has finished
  if (record->event.pressed)
    pressed_count++;
  else
    pressed_count--;
 
  // This allows escape to reset the one shot held keys
  if (keycode == KC_ESC && record->event.pressed) 
  {
    bool queue = true;
    if ((get_oneshot_mods ()) && !has_oneshot_mods_timed_out ())
    {
      clear_oneshot_mods ();
      queue = false;
    }
    return queue;
  }
  return true;
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function

  if (record->event.pressed) 
  {
    uint8_t grp = (id & GRPMASK) >> 6;
    chord[grp] |= id;
  }
  else 
  {
    // Chord complete
    if (pressed_count == 0) 
    {
      send_chord();
      chord[0] = chord[1] = chord[2] = chord[3] = 0;
    }
  }
  return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) 
{
  // Startup light show sequence
  ergodox_led_all_on();
  for (int i = LED_BRIGHTNESS_HI; i > LED_BRIGHTNESS_LO; i--) 
  {
    ergodox_led_all_set (i);
    wait_ms (5);
  }
  wait_ms(1000);
  for (int i = LED_BRIGHTNESS_LO; i > 0; i--)
  {
    ergodox_led_all_set (i);
    wait_ms (10);
  }
  ergodox_led_all_off();

  //if (!eeconfig_is_enabled())
  //  eeconfig_init();
  //dl = eeconfig_read_default_layer ();

  // CM?
  set_unicode_input_mode(UC_WIN); // Windows (with registry key, see wiki)
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
  case SYMB:
    ergodox_right_led_1_on();
    ergodox_right_led_1_set (LED_BRIGHTNESS_HI);
    break;
  case TXBOLT:
    ergodox_right_led_3_on();
    ergodox_right_led_3_set (LED_BRIGHTNESS_LO);
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
    
    SEQ_ONE_KEY (KC_V) 
    {
      SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ (" QMK_VERSION "/" LAYOUT_ergodox_VERSION ")");
    }
  }
};


