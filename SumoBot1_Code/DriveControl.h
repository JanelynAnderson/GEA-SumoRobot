#pragma once
#include <algorithm>
#include <Arduino.h>
#define PI  3.14

struct DriveControlCreateInfo
{
  double wheelRadius;
  double maxMotorRPM;
  double turnRadius;
  int leftMotorPwmPin;
  int rightMotorPwmPin;
  bool isLeftMotorReversed;
  bool isRightMotorReversed;
};

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
  // real time inputs
  double angularVelocity = 0;
  double translationalVelocity = 0;
  // real time outputs
  double rightDutyCycle = 0;
  double leftDutyCycle = 0;

  DriveControl(DriveControlCreateInfo createInfo);

  double Clamp(double val, double min, double max);
  // step the model
  void step();
  void poststep();
private:
  //RightPWMHandler Object
  
  //LeftPWMHandler Object

  DriveControlCreateInfo createInfo;
  DCContainer TranslationalVelocityToDutyCycle(double translationalVelocity);
  DCContainer RotationalVelocityToDutyCycle(double angularVelocity);
};