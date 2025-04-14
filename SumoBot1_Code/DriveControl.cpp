#include "DriveControl.h"

DriveControl::DriveControl(DriveControlCreateInfo createInfo)
{
  this->createInfo = createInfo;
  
  //Initalize Right PWM Handler
  pinMode(createInfo.leftMotorPwmPin, OUTPUT);

  //Initalize Left PWM Handler
  pinMode(createInfo.rightMotorPwmPin, OUTPUT);
}

void DriveControl::step()
{
  DCContainer transDC = TranslationalVelocityToDutyCycle(translationalVelocity);
  DCContainer rotDC = RotationalVelocityToDutyCycle(angularVelocity);
  DCContainer totDC = transDC + rotDC;

  rightDutyCycle = Clamp(totDC.DCRight,-1.0,1.0);
  
  leftDutyCycle = Clamp(totDC.DCLeft, -1.0,1.0);
}

void DriveControl::poststep()
{
  double rightPercentOn = (((createInfo.isRightMotorReversed)? -1 : 1)*rightDutyCycle);
  double leftPercentOn = (((createInfo.isLeftMotorReversed)? -1 : 1)*leftDutyCycle);
  analogWrite(createInfo.rightMotorPwmPin, rightPercentOn*255);
  analogWrite(createInfo.leftMotorPwmPin, leftPercentOn*255);
}

DCContainer DriveControl::TranslationalVelocityToDutyCycle(double translationalVelocity)
{
  double DC = translationalVelocity*60/(createInfo.maxMotorRPM*2*PI*createInfo.wheelRadius);

  return DCContainer{DC,DC};
}

DCContainer DriveControl::RotationalVelocityToDutyCycle(double angularVelocity)
{
  double DC = angularVelocity*createInfo.turnRadius*60/(createInfo.maxMotorRPM*2*PI*createInfo.wheelRadius);

  return DCContainer{DC,-DC};

}

double DriveControl::Clamp(double val, double min, double max)
{
  if(val < min)
    return min;
  else if(val > max)
    return max;
  else
    return val;
}
