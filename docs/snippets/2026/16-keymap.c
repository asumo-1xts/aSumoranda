#include QMK_KEYBOARD_H
#include "keymap.h"

// Magic機能を使わないので、容量を削減するため上書きする
uint16_t keycode_config(uint16_t keycode) { return keycode; }
uint8_t mod_config(uint8_t mod) { return mod; }

/**
 * ==============================
 * 内部関数の宣言
 * ==============================
 */

static uint32_t key_timer;
bool is_rgb_timeout = false;
void yawn(uint8_t hue, uint8_t sat, uint8_t val);
void check_rgb_timeout(void);
void refresh_rgb(void);

/**
 * ==============================
 * QMKで用意されているAPI
 * ==============================
 */

void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom();
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
  yawn(HUE, SAT, VAL);
}

void housekeeping_task_user(void) {
#ifdef RGBLIGHT_TIMEOUT
  check_rgb_timeout();
#endif
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef RGBLIGHT_TIMEOUT
  if (record->event.pressed) refresh_rgb();
#endif
}

/**
 * ==============================
 * 内部関数の定義
 * ==============================
 */

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

void refresh_rgb(void) {
  key_timer = timer_read32();
  if (is_rgb_timeout) {
    is_rgb_timeout = false;
    rgblight_wakeup();
  }
}

void check_rgb_timeout(void) {
  if (!is_rgb_timeout && timer_elapsed32(key_timer) > RGBLIGHT_TIMEOUT) {
    rgblight_suspend();
    is_rgb_timeout = true;
  }
}
