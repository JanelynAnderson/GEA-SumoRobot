#pragma once
#include <Bluepad32.h>
#include "MethodsThatShouldntExist.h"
namespace gea
{
  /**
   * This holds non-queue data (queue not yet implemented as not needed) that is transfered to
   * the DriveControl class through the semaphore
   */
  struct GamepadData
  {
    double rightJoystickX;
    double rightJoystickY;

    double leftJoystickX;
    double leftJoystickY;
    // TO BE ADDED TO
  };

  /**
   * Configuration for one Joystick: bounds, deadband, and reversals
   */
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

  struct GamepadCreateInfo
  {
    JoystickConfig rightJoystick;
    JoystickConfig leftJoystick;
  };

  class Gamepad
  {
  public:
  //constructor
    Gamepad(GamepadCreateInfo createInfo);
  //methods
    void GetRightJoystick(double& axisX, double& axisY);
    void GetLeftJoystick(double& axisX, double& axisY);
    void LinkGamepad(GamepadPtr myGamepad);
  private:
  //methods
    void GetNormalizedAxis(int32_t axisX,
                           int32_t axisY,
                           gea::JoystickConfig JSConfig,
                           double& normX,
                           double& normY);
  //vars
    GamepadPtr myGamepad;
    GamepadCreateInfo createInfo;
  };
}