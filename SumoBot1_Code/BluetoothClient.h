#pragma once

#include <cmath>
#include <Bluepad32.h>
#include "Arduino.h"
#include "Gamepad.h"

//Code copied from https://www.youtube.com/watch?v=yfa0Za2N_Ww

struct BluetoothClientCreateInfo
{
  gea::GamepadCreateInfo ControllerCreateInfo;
};

class BluetoothClient
{
private:
  //methods
  void OnConnectedGamepad(GamepadPtr gp);
  void OnDisconnectedGamepad(GamepadPtr gp);

  //vars
  gea::GamepadData data;
  gea::Gamepad Controller;
  static GamepadPtr myGamepads[BP32_MAX_GAMEPADS];
  BluetoothClientCreateInfo createInfo;
public:
  //methods
  BluetoothClient(BluetoothClientCreateInfo createInfo);
  void Loop();
  void Setup();
  void ReadControllerData(gea::GamepadData& _data_);
  
};