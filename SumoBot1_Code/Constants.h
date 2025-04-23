#pragma once

#include "DriveControl.h"
#include "BluetoothClient.h"

#define PI  3.14

//650rpm 6v

// 26 direction left bool
// 18 direction left pwm
// 19 direction right pwm
// 23 direction right bool

namespace Constants
{
  namespace DriveSystem
  {
    DriveControlCreateInfo DriveSystemCreateInfo
    {
      //Physical Properties
      0.035,   // wheelRadius (meters) // 70mm OD
      650.0,   // maxMotorRPM (RPM)
      4.0,     // turnRadius  (meters)
      //Pins
      18,      // leftMotorPwmPin
      19,      // rightMotorPwmPin
      26,      // leftMotorDirPin
      23,      // rightMotorDirPin
      //Reversal
      false,    // isLeftMotorReversed
      true    // isRightMotorReversed
    };
  }
  namespace Bluetooth
  {
    gea::JoystickConfig rightJoystick
    {
      511, // Y axis max
      511, // Y axis min
      511, // X axis max
      511, // X axis min
      .05,  // deadband [0, 1]
      false, // invert the X axis
      true // invert the y axis
    };
    gea::JoystickConfig leftJoystick
    {
      511, // Y axis max
      511, // Y axis min
      511, // X axis max 
      511, // X axis min 
      .05,  // deadband [0, 1]
      false, // invert the X axis
      true // invert the y axis
    };
    gea::GamepadCreateInfo ControllerCreateInfo
    {
      rightJoystick,
      leftJoystick
    };
    BluetoothClientCreateInfo bluetoothCreateInfo
    {
      ControllerCreateInfo
    };
  }
}