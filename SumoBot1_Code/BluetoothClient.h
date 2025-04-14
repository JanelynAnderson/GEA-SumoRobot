#pragma once

#include <cmath>
#include <Bluepad32.h>
#include "Arduino.h"
#include "MethodsThatShouldntExist.h"

//Code copied from https://www.youtube.com/watch?v=yfa0Za2N_Ww

struct JoystickConfig
{
  /** Some integer*/
  int upperBoundY;
  /** Some integer*/
  int lowerBoundY;
  /** Some integer*/
  int upperBoundX;
  /** Some integer*/
  int lowerBoundX;
  /** double between 0 and 1*/
  double deadband;
  bool reverseX;
  bool reverseY;
};

struct BluetoothClientCreateInfo
{
  JoystickConfig LeftJoystick;
  JoystickConfig RightJoystick;
};

struct GamepadData
{
  double rightJoystickX;
  double rightJoystickY;

  double leftJoystickX;
  double leftJoystickY;
  // TO BE ADDED TO
};

class BluetoothClient
{
private:
  //vars
  static GamepadPtr myGamepads[BP32_MAX_GAMEPADS];
  BluetoothClientCreateInfo createInfo;

  //methods
  void getNormalizedAxis(int32_t axisX,
                                          int32_t axisY,
                                          JoystickConfig JSConfig,
                                          double& normX,
                                          double& normY);
  static void onConnectedGamepad(GamepadPtr gp);
  static void onDisconnectedGamepad(GamepadPtr gp);
public:
  //vars
  GamepadData data;
  //methods
  BluetoothClient(BluetoothClientCreateInfo createInfo);
  void getRightJoystick(double& axisX, double& axisY);
  void getLeftJoystick(double& axisX, double& axisY);
  void Loop();
  void Setup();
};