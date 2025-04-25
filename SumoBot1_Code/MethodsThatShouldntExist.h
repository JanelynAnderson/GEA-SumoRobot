#pragma once
#include <algorithm>
#include <string>
#define LED_PIN 2

//#define Production

namespace gea
{
  /**
   * A custom clamp method because the standard library one is not able to be accessed in
   * c++11 which is the version the bluepad32 library is made in
   */
  template <class T>
  inline T Clamp(const T value, const T min, const T max)
  {
    return std::min(max, std::max(min, value));
  }


  /**
   * custom print method that takes in a regular c++ string and converts it to cstr buffer to print serial
   * if "Production" is not defined, the prints will take place, otherwise the method does nothing
   * prints do nothing because the serial out is very blocking
   */
  inline void printStrLn(std::string Text) {
    #ifndef Production 
      const char *cstr = Text.c_str();
      char buffer[Text.size()+1];
      strncpy(buffer, cstr, Text.size());
      Serial.println(buffer);
    #endif
  }
}