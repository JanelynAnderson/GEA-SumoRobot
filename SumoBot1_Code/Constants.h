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
      5,
      6,
      true,
      false
    };
  }
}