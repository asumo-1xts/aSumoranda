/**
 * @file      heartLand_daughter.cpp
 * @author    aSumo (1xtelescope@gmail.com)
 * @brief     a sketch for Mega2560 ProMini in USB-MIDI Controller "HeartLand"
 * @version   1.0
 * @date      2024-06-26
 * @copyright GPL-3.0
 * @details   Just send serial MIDI message from Serial1 to "mother"
 */

//! https://github.com/tttapa/Control-Surface.git
#include <Control_Surface.h>
//! 単純なシリアル通信のためのインスタンス
SerialMIDI_Interface<decltype(Serial1)> midi_ser{Serial1, MIDI_BAUD};

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ variable to change ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

uint8_t ch[3] = {2, 3, 4};  //!< MIDIチャンネルの管理

/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ variable to change ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

uint8_t util01 = 2;  //!< 余りその1
uint8_t util02 = 3;  //!< 余りその2

CCButton sw[] = {
    {27, {7, Channel::createChannel(ch[0])}},   // enc_sw01
    {30, {8, Channel::createChannel(ch[0])}},   // enc_sw02
    {33, {9, Channel::createChannel(ch[0])}},   // enc_sw03
    {36, {10, Channel::createChannel(ch[0])}},  // enc_sw04

    {4, {11, Channel::createChannel(ch[0])}},   // ary01
    {5, {12, Channel::createChannel(ch[0])}},   // ary02
    {6, {13, Channel::createChannel(ch[0])}},   // ary03
    {7, {14, Channel::createChannel(ch[0])}},   // ary04
    {8, {15, Channel::createChannel(ch[0])}},   // ary05
    {9, {16, Channel::createChannel(ch[0])}},   // ary06
    {10, {17, Channel::createChannel(ch[0])}},  // ary07
    {11, {18, Channel::createChannel(ch[0])}},  // ary08

    {22, {6, Channel::createChannel(ch[1])}},  // Sw01
    {23, {7, Channel::createChannel(ch[1])}},  // Sw02
    {24, {6, Channel::createChannel(ch[2])}},  // Sw03
    {25, {7, Channel::createChannel(ch[2])}},  // Sw04
};

CCPotentiometer pot[] = {
    {A0, {8, Channel::createChannel(ch[1])}},    // pot01_01
    {A1, {9, Channel::createChannel(ch[1])}},    // pot01_02
    {A2, {10, Channel::createChannel(ch[1])}},   // pot01_03
    {A3, {11, Channel::createChannel(ch[1])}},   // pot01_04
    {A4, {12, Channel::createChannel(ch[1])}},   // pot02_01
    {A5, {13, Channel::createChannel(ch[1])}},   // pot02_02
    {A6, {14, Channel::createChannel(ch[1])}},   // pot02_03
    {A7, {15, Channel::createChannel(ch[1])}},   // pot02_04
    {A8, {8, Channel::createChannel(ch[2])}},    // pot03_01
    {A9, {9, Channel::createChannel(ch[2])}},    // pot03_02
    {A10, {10, Channel::createChannel(ch[2])}},  // pot03_03
    {A11, {11, Channel::createChannel(ch[2])}},  // pot03_04
    {A12, {12, Channel::createChannel(ch[2])}},  // pot04_01
    {A13, {13, Channel::createChannel(ch[2])}},  // pot04_02
    {A14, {14, Channel::createChannel(ch[2])}},  // pot04_03
    {A15, {15, Channel::createChannel(ch[2])}},  // pot04_04
};

// The maximum value that can be measured (usually 16383 = 2¹⁴-1)
constexpr analog_t maxRawValue = 16383;
// The filtered value read when potentiometer is at the 0% position
constexpr analog_t minimumValue = maxRawValue / 64;
// The filtered value read when potentiometer is at the 100% position
constexpr analog_t maximumValue = maxRawValue - maxRawValue / 64;

/**
 * @brief   可変抵抗をLive上で正しく線形にマッピングする関数
 * @param   raw 可変抵抗から読み取った値
 */
analog_t map2lin(analog_t raw) {
  raw = constrain(raw, minimumValue, maximumValue);
  float x = (float)(raw - minimumValue) / (maximumValue - minimumValue);
  float x_calibrated;
  float p = 0.5f;  // 大きくすると終端が急峻になる

  if (x < 0.6f) {
    // 前半: 0からの立ち上がりを鋭く
    x_calibrated = 0.5f * powf(2.0f * x, p);
  } else {
    // 後半: 1.0への到達を鋭く
    x_calibrated = 1.0f - 0.5f * powf(2.0f * (1.0f - x), p);
  }

  return (analog_t)(x_calibrated * maxRawValue);
}

//! @brief setup関数
void setup() {
  pinMode(util01, INPUT_PULLUP);
  pinMode(util02, INPUT_PULLUP);
  for (auto &p : pot) {
    p.map(map2lin);
  }
  Control_Surface.begin();
}

//! @brief loop関数
void loop() { Control_Surface.loop(); }
