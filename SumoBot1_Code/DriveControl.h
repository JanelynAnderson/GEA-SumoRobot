#pragma once
#include <algorithm>
#include <Arduino.h>
#include "BluetoothClient.h"
#include "MethodsThatShouldntExist.h"
#define PI  3.14

struct DriveControlCreateInfo
{
  double wheelRadius;
  double maxMotorRPM;
  double turnRadius;

  int leftMotorPwmPin;
  int rightMotorPwmPin;

  int leftMotorDirPin;
  int rightMotorDirPin;

  bool isLeftMotorReversed;
  bool isRightMotorReversed;
};

/**
 * Holds a left and rihgt DC and preforms operations on them
 */
struct DCContainer
{
  double DCRight;
  double DCLeft;

  DCContainer operator+(DCContainer const& obj)
    {
        DCContainer newDCContainer;
        newDCContainer.DCRight = obj.DCRight + DCRight;
        newDCContainer.DCLeft = obj.DCLeft + DCLeft;
        return newDCContainer;
    }

};

class DriveControl
{
public:
  DriveControl(DriveControlCreateInfo createInfo);
//methods
  void Step();
  void Poststep();
  void WriteControllerData(gea::GamepadData& _data_);
//vars
  // real time inputs
  double angularVelocity = 0;
  double translationalVelocity = 0;
  // real time outputs
  double rightDutyCycle = 0;
  double leftDutyCycle = 0;
private:
//methods
  DCContainer TranslationalVelocityToDutyCycle(double translationalVelocity);
  DCContainer RotationalVelocityToDutyCycle(double angularVelocity);
//vars
  DriveControlCreateInfo createInfo;
  gea::GamepadData data;
};