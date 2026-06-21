/**
 * ==============================
 * RGBライトのカラー設定: HSV_CORAL
 * ==============================
 */

const uint8_t HUE = 11;
const uint8_t SAT = 176;
const uint8_t VAL = 127;
const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM = {30};

/**
 * ==============================
 * キーマップの定義
 * ==============================
 */

#define _BS 0
#define _FN 1
#define _MI 2
#define TTTT KC_TRNS
#define NNNN KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BS] = LAYOUT(
        MO(_FN),KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,
        KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12, KC_PSCR,NNNN   ,NNNN,
        KC_ESC, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,
        KC_7,   KC_8,   KC_9,   KC_0,   KC_MINS,KC_EQL, KC_GRV, KC_BSPC,KC_HOME,
                KC_TAB, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,
        KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_LBRC,KC_RBRC,KC_ENT, KC_END,
                KC_LCTL,KC_A,   KC_S,   KC_D,   KC_F,   KC_G,
        KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT,KC_BSLS,        MS_WHLU,
                KC_LSFT,KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,
        KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,KC_RSFT,        KC_UP,  MS_WHLD,
                MS_BTN1,KC_LWIN,KC_LALT,KC_LNG1,KC_SPC,
                KC_SPC, KC_LNG2,KC_RWIN,MO(_FN),KC_LEFT,        KC_DOWN,KC_RGHT
    ),
    [_FN] = LAYOUT(
        TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
        TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
        TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
        TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   KC_DEL, TTTT,
                TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
        TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
                TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
        TTTT,   TTTT,   TTTT,   UG_VALU,TTTT,   TTTT,   TTTT,           MS_BTN3,
                KC_DEL, TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
        TTTT,   TG(_MI),TTTT,   TTTT,   TTTT,   KC_CAPS,        MS_UP,  MS_BTN4,
                MS_BTN2,TTTT,   TTTT,   TTTT,   TTTT,
                TTTT,   TTTT,   TTTT,   TTTT,   MS_LEFT,        MS_DOWN,MS_RGHT
    ),
    [_MI] = LAYOUT(
        TTTT,   MI_CH1, MI_CH2, MI_CH3, MI_CH4, MI_CH5, MI_CH6,
        MI_CH7, MI_CH8, MI_CH9, MI_CH10,MI_CH11,MI_CH12,TTTT,   TTTT,   TTTT,
        TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
        TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   MI_OCTU,
                TTTT,   TTTT,   MI_Cs,  MI_Ds,  TTTT,   MI_Fs,
        MI_Gs,  MI_As,  TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   MI_OCTD,
                TTTT,   MI_C,   MI_D,   MI_E,   MI_F,   MI_G,
        MI_A,   MI_B,   MI_C,   TTTT,   TTTT,   TTTT,   TTTT,           MI_VELU,
                TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
        TTTT,   TTTT,   TTTT,   TTTT,   TTTT,   TTTT,           MI_MODU,MI_VELD,
                TTTT,   TTTT,   TTTT,   TTTT,   TTTT,
                TTTT,   TTTT,   TTTT,   TTTT,   MI_BNDD,        MI_MODD,MI_BNDU
    )
};
