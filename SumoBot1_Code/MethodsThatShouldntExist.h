#pragma once
#include <algorithm>
#include <string>
#define LED_PIN 2

namespace gea
{
  template <class T>
  inline T Clamp(const T value, const T min, const T max)
  {
    return std::min(max, std::max(min, value));
  }

  inline void printStrLn(std::string Text) {
    const char *cstr = Text.c_str();
    char buffer[Text.size()+1];
    strncpy(buffer, cstr, Text.size());
    Serial.println(buffer);
  }
}
