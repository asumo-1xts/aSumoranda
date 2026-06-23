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

/**
 * @brief   可変抵抗をLive上で対数的にマッピングする関数
 * @param   raw 可変抵抗から読み取った値
 */
analog_t map2log(analog_t raw) {
  raw = constrain(raw, minimumValue, maximumValue);
  float x = (float)(raw - minimumValue) / (maximumValue - minimumValue);
  float p = 1.5f;  // y=x^pのp。大きくすると終端が急峻になる

  float x_calibrated = powf(x, p);

  return (analog_t)(x_calibrated * maxRawValue);
}
