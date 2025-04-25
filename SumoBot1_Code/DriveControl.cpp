#include "DriveControl.h"

DriveControl::DriveControl(DriveControlCreateInfo createInfo)
{
  this->createInfo = createInfo;
  
  //Initalize Right PWM Handler
  pinMode(createInfo.leftMotorPwmPin, OUTPUT);

  //Initalize Left PWM Handler
  pinMode(createInfo.rightMotorPwmPin, OUTPUT);

  //Initalize Right PWM Handler
  pinMode(createInfo.leftMotorDirPin, OUTPUT);

  //Initalize Left PWM Handler
  pinMode(createInfo.rightMotorDirPin, OUTPUT);
}

/**
 * runs all arithmtic calculations and saves them
 */
void DriveControl::Step()
{
  //gea::wStrLn( "\tx=" + std::to_string(data.rightJoystickX) + "\t\ty=" + std::to_string(data.rightJoystickY));
  angularVelocity = 1.0 * data.leftJoystickX; //mult = max angular velocity rad/s
  translationalVelocity = 1.0 * data.rightJoystickY; //mult = max linear velocity m/s

  DCContainer transDC = TranslationalVelocityToDutyCycle(translationalVelocity);
  DCContainer rotDC = RotationalVelocityToDutyCycle(angularVelocity);
  DCContainer totDC = transDC + rotDC;

  rightDutyCycle = gea::Clamp(totDC.DCRight,-1.0,1.0);
  
  leftDutyCycle = gea::Clamp(totDC.DCLeft, -1.0,1.0);
}
/**
 * uses calculations from step to push data to motors and such
 */
void DriveControl::Poststep()
{ 
  // Direction
  bool RightDirection = false;
  if(std::abs(rightDutyCycle) > 0.05)
  {
    RightDirection = std::signbit(rightDutyCycle);
    if(createInfo.isRightMotorReversed) RightDirection = !RightDirection;
  }
  else
  {

  }

  bool LeftDirection = false;
  if(std::abs(leftDutyCycle) > 0.05)
  {
    LeftDirection = std::signbit(leftDutyCycle);
    if(createInfo.isLeftMotorReversed) LeftDirection = !LeftDirection;
  }

  digitalWrite(createInfo.leftMotorDirPin, LeftDirection);
  digitalWrite(createInfo.rightMotorDirPin, RightDirection);
  
  // PWM
  analogWrite(createInfo.rightMotorPwmPin, static_cast<int>(std::abs(rightDutyCycle*255)));
  analogWrite(createInfo.leftMotorPwmPin, static_cast<int>(std::abs(leftDutyCycle*255)));

  //gea::printStrLn("Right Dir: " + std::to_string(RightDirection) + "\t\tRightPWM: " + std::to_string(static_cast<int>(std::abs(rightDutyCycle*255))));
  //gea::printStrLn("Left Dir: " + std::to_string(LeftDirection) + "\t\Left: " + std::to_string(static_cast<int>(std::abs(leftDutyCycle*255))));

}

/**
 * Transforms the requested translational velocity into the needed Duty cycle components
 */
DCContainer DriveControl::TranslationalVelocityToDutyCycle(double translationalVelocity)
{
  double DC = translationalVelocity*60/(createInfo.maxMotorRPM*2*PI*createInfo.wheelRadius);

  return DCContainer{DC,DC};
}

/**
 * Transforms the requested angular velocity into the needed Duty cycle components
 */
DCContainer DriveControl::RotationalVelocityToDutyCycle(double angularVelocity)
{
  double DC = angularVelocity*createInfo.turnRadius*60/(createInfo.maxMotorRPM*2*PI*createInfo.wheelRadius);

  return DCContainer{DC,-DC};

}

/**
 * Saves the controller data from the external semaphore to the Drive Control class
 */
void DriveControl::WriteControllerData(gea::GamepadData& _data_)
{
  this->data = _data_;
}

