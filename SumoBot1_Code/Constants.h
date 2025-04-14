#pragma once

#include "DriveControl.h"
#include "BluetoothClient.h"

#define PI  3.14

namespace Constants
{
  namespace DriveSystem
  {
    DriveControlCreateInfo DriveSystemCreateInfo
    {
      1.0, // Wheel raduius
      2.0,
      4.0,
      33,
      22,
      true,
      false
    };
  }
  namespace Bluetooth
  {
    JoystickConfig rightJoystick
    {
      511, // Y axis max
      511, // Y axis min
      511, // X axis max
      511, // X axis min
      .05,  // deadband [0, 1]
      false, // invert the X axis
      true // invert the y axis
    };
    JoystickConfig leftJoystick
    {
      511, // Y axis max
      511, // Y axis min
      511, // X axis max 
      511, // X axis min 
      .05,  // deadband [0, 1]
      false, // invert the X axis
      true // invert the y axis
    };

    BluetoothClientCreateInfo bluetoothCreateInfo
    {
      rightJoystick,
      leftJoystick
    };
  }
}