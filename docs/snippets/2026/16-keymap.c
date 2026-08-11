#include QMK_KEYBOARD_H
#include "keymap.h"

/**
 * ==============================
 * 内部変数・内部関数の定義
 * ==============================
 */

static uint32_t key_timer;
bool is_rgb_timeout = false;

void yawn(uint8_t hue, uint8_t sat, uint8_t val) {
  for (int16_t i = 0; i <= RGBLIGHT_LIMIT_VAL; i += RGBLIGHT_VAL_STEP) {
    rgblight_sethsv_noeeprom(hue, sat, i);
    wait_ms(30);
  }
  for (int16_t i = RGBLIGHT_LIMIT_VAL; i >= 0; i -= RGBLIGHT_VAL_STEP) {
    rgblight_sethsv_noeeprom(hue, sat, i);
    wait_ms(30);
  }

  rgblight_sethsv_noeeprom(hue, sat, 0);  // いったん消灯
  wait_ms(60);
  rgblight_sethsv_noeeprom(hue, sat, val);  // 最終的な明るさ
}

void check_rgb_timeout(void) {
  if (!is_rgb_timeout && (timer_elapsed32(key_timer) > RGBLIGHT_TIMEOUT)) {
    is_rgb_timeout = true;
    rgblight_mode_noeeprom(RGBLIGHT_MODE_TWINKLE);
    rgblight_suspend();
  }
}

void refresh_rgb(void) {
  key_timer = timer_read32();
  if (is_rgb_timeout) {
    is_rgb_timeout = false;
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_wakeup();
  }
}

/**
 * ==============================
 * QMKで用意されているAPI
 * ==============================
 */

void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom();
  yawn(HUE, SAT, VAL);
}

void housekeeping_task_user(void) {
#ifdef RGBLIGHT_TIMEOUT
  check_rgb_timeout();
#endif
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef RGBLIGHT_TIMEOUT
  if (record->event.pressed) {
    refresh_rgb();
  }
#endif
}

layer_state_t layer_state_set_user(layer_state_t state) {
  switch (get_highest_layer(state)) {
    case _MI:  // MIDI layer
      rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
      break;
    default:  // any other layers
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      break;
  }
  return state;
}

/**
 * ==============================
 * ファームウェア容量削減
 * ==============================
 */

// Magic機能は使わないので上書き
uint16_t keycode_config(uint16_t keycode) { return keycode; }
uint8_t mod_config(uint8_t mod) { return mod; }
