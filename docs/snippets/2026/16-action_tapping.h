#pragma once

#ifndef TAPPING_TERM
#define TAPPING_TERM 200
#endif

#if !defined(QUICK_TAP_TERM) || QUICK_TAP_TERM > TAPPING_TERM
#define QUICK_TAP_TERM TAPPING_TERM
#endif

#ifndef TAPPING_TOGGLE
#define TAPPING_TOGGLE 5
#endif

#ifndef NO_ACTION_TAPPING
uint16_t get_record_keycode(keyrecord_t *record, bool update_layer_cache);
uint16_t get_event_keycode(keyevent_t event, bool update_layer_cache);
void action_tapping_process(keyrecord_t record);
#endif

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record);
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record);
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record);
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record);
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record);

#ifdef CHORDAL_HOLD
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                      uint16_t other_keycode, keyrecord_t *other_record);
bool get_chordal_hold_default(keyrecord_t *tap_hold_record,
                              keyrecord_t *other_record);
#endif

#ifdef FLOW_TAP_TERM
bool is_flow_tap_key(uint16_t keycode);
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record,
                           uint16_t prev_keycode);
#endif

#ifdef DYNAMIC_TAPPING_TERM_ENABLE
extern uint16_t g_tapping_term;
#endif

#if defined(TAPPING_TERM_PER_KEY) && !defined(NO_ACTION_TAPPING)
#define GET_TAPPING_TERM(keycode, record) get_tapping_term(keycode, record)
#elif defined(DYNAMIC_TAPPING_TERM_ENABLE) && !defined(NO_ACTION_TAPPING)
#define GET_TAPPING_TERM(keycode, record) g_tapping_term
#else
#define GET_TAPPING_TERM(keycode, record) (TAPPING_TERM)
#endif

#ifdef QUICK_TAP_TERM_PER_KEY
#define GET_QUICK_TAP_TERM(keycode, record) get_quick_tap_term(keycode, record)
#else
#define GET_QUICK_TAP_TERM(keycode, record) (QUICK_TAP_TERM)
#endif
