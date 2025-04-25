#include "Gamepad.h"

gea::Gamepad::Gamepad(GamepadCreateInfo createInfo)
{
  this->createInfo = createInfo;
}

/**
 * Raw Controller data is passed in and normalized with its bounds, a deadband is applied, and the normalized cords are
 * returned by reference
 * the bounds are accessed through the JoystickConfig passed in
 */
void gea::Gamepad::GetNormalizedAxis(int32_t axisX, int32_t axisY, gea::JoystickConfig JSConfig, double& normX, double& normY)
{
  // IMPORTANT THAT SCALING HAPPENES BEFORE DEADBAND
  // X Normalization
  if(axisX > 0)
  {
    normX = static_cast<double>(axisX) / JSConfig.upperBoundX;
  } 
  else if(axisX < 0)
  {
    normX = static_cast<double>(axisX) / JSConfig.lowerBoundX;
  }
  else
    normX = 0.0;
  normX = gea::Clamp(normX, -1.0, 1.0); // clamp output to [-1, 1]
  if(JSConfig.reverseX) normX = -normX; // reverse if nessisary

  // Y Normalization
  if(axisY > 0)
  {
    normY = static_cast<double>(axisY) / JSConfig.upperBoundY;
  } 
  else if(axisY < 0)
  {
    normY = static_cast<double>(axisY) / JSConfig.lowerBoundY;
  }
  else
    normY = 0;
  normY = gea::Clamp(normY, -1.0, 1.0); // clamp output to [-1, 1]
  if(JSConfig.reverseY) normY = -normY; // reverse if nessisary

  //Add a Deadband
  if(std::hypot(normX, normY) < JSConfig.deadband)
  {
    normX = 0;
    normY = 0;
  }
}

/**
 * Returns x and y by reference that are between -1 and 1 inclusive for the Right Joystick
 */
void gea::Gamepad::GetRightJoystick(double& axisX, double& axisY)
{
  GetNormalizedAxis(myGamepad->axisRX(), myGamepad->axisRY(), createInfo.rightJoystick, axisX, axisY);
}

/**
 * Returns x and y by reference that are between -1 and 1 inclusive for the Left Joystick
 */
void gea::Gamepad::GetLeftJoystick(double& axisX, double& axisY)
{
  GetNormalizedAxis(myGamepad->axisX(), myGamepad->axisY(), createInfo.leftJoystick, axisX, axisY);
}

/**
 * Saves the passed ptr and uses it to get gamepad data for other methods, should be changed when connecting controller
 */
void gea::Gamepad::LinkGamepad(GamepadPtr myGamepad)
{
  this->myGamepad = myGamepad;
}