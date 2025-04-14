#include <string>
#include "BluetoothClient.h"

GamepadPtr BluetoothClient::myGamepads[BP32_MAX_GAMEPADS];


BluetoothClient::BluetoothClient(BluetoothClientCreateInfo createInfo)
{
  this->createInfo = createInfo;
}

void BluetoothClient::Setup()
{
  Serial.begin(9600);
  pinMode (LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("intialising ESP32");

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
  BP32.forgetBluetoothKeys();
}

void BluetoothClient::Loop()
{
  BP32.update();

  GamepadPtr myGamepad = myGamepads[0];
  if (myGamepad && myGamepad->isConnected()) {

    getRightJoystick(data.rightJoystickX,data.rightJoystickY); //update right joystick values
    getLeftJoystick(data.leftJoystickX, data.leftJoystickY); //update left joystick values

    gea::printStrLn( "\tx=" + std::to_string(data.rightJoystickX) + "\t\ty=" + std::to_string(data.rightJoystickY));
  }
}

void BluetoothClient::onConnectedGamepad(GamepadPtr gp)
{
  bool foundEmptySlot = false;
  digitalWrite(LED_PIN, HIGH);
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == nullptr) {
      myGamepads[i] = gp;
      foundEmptySlot = true;
      Serial.println("Gamepad connected!");
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
        "CALLBACK: Gamepad connected, but could not found empty slot");
  }
}

void BluetoothClient::onDisconnectedGamepad(GamepadPtr gp)
{
  bool foundGamepad = false;
  digitalWrite(LED_PIN, LOW);
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == gp) {
      Serial.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
      myGamepads[i] = nullptr;
      foundGamepad = true;
      break;
    }
  }

  if (!foundGamepad) {
    Serial.println(
        "CALLBACK: Gamepad disconnected, but not found in myGamepads");
  }
}

void BluetoothClient::getNormalizedAxis(int32_t axisX, int32_t axisY, JoystickConfig JSConfig, double& normX, double& normY)
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
    normY = static_cast<double>(-axisY) / JSConfig.upperBoundY;
  } 
  else if(axisY < 0)
  {
    normY = static_cast<double>(-axisY) / JSConfig.lowerBoundY;
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

void BluetoothClient::getRightJoystick(double& axisX, double& axisY)
{
  GamepadPtr myGamepad = myGamepads[0];
  getNormalizedAxis(myGamepad->axisRX(), myGamepad->axisRY(), createInfo.RightJoystick, axisX, axisY);
}

void BluetoothClient::getLeftJoystick(double& axisX, double& axisY)
{
  GamepadPtr myGamepad = myGamepads[0];
  getNormalizedAxis(myGamepad->axisX(), myGamepad->axisY(), createInfo.LeftJoystick, axisX, axisY);
}