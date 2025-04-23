#include <string>
#include "BluetoothClient.h"
//TODO: < 20kHz Switching rate
GamepadPtr BluetoothClient::myGamepads[BP32_MAX_GAMEPADS];

BluetoothClient::BluetoothClient(BluetoothClientCreateInfo createInfo) :
Controller{createInfo.ControllerCreateInfo}
{
  this->createInfo = createInfo;
}

/**
 * Used to set up the bluetooth connection in its respective thread
 */
void BluetoothClient::Setup()
{
  pinMode (LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  gea::printStrLn("intialising ESP32");

  // Setup the Bluepad32 callbacks
  /*
   * I'm not happy about this, I appologise. This is the only way to use non-static objects in the connect
   * and disconnect methods. This is needed because I want to pass create info through the constructor and
   * not statically address it. This brings me great sorrow using a lambda like this, but it is a nessisary
   * evil.
   */
  BP32.setup(
  [this](ControllerPtr controller) { OnConnectedGamepad(controller); },
  [this](ControllerPtr controller) { OnDisconnectedGamepad(controller); }
  );

  BP32.forgetBluetoothKeys();
}

/**
 * the main loop used in the thread of the Bluetooth Client
 */
void BluetoothClient::Loop()
{
  BP32.update();

  GamepadPtr myGamepad = myGamepads[0];
  if (myGamepad && myGamepad->isConnected()) {

    Controller.GetRightJoystick(data.rightJoystickX,data.rightJoystickY); //update right joystick values  
    Controller.GetLeftJoystick(data.leftJoystickX, data.leftJoystickY); //update left joystick values

    //gea::printStrLn( "\tx=" + std::to_string(data.rightJoystickX) + "\t\ty=" + std::to_string(data.rightJoystickY));
  }
}

/**
 * Runs when connecting a gamepad is detected by the bluepad32 library
 */
void BluetoothClient::OnConnectedGamepad(GamepadPtr gp)
{
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == nullptr) {
      myGamepads[i] = gp;
      foundEmptySlot = true;
      digitalWrite(LED_PIN, HIGH);
      Controller.LinkGamepad(gp);
      gea::printStrLn("Gamepad connected!");
      break;
    }
  }
  if (!foundEmptySlot) {
    gea::printStrLn( "CALLBACK: Gamepad connected, but could not found empty slot"); }
}

/**
 * Runs when disconnecting a gamepad is detected by the bluepad32 library
 */
void BluetoothClient::OnDisconnectedGamepad(GamepadPtr gp)
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
    gea::printStrLn( "CALLBACK: Gamepad disconnected, but not found in myGamepads"); }
}

/**
 * Saves the controller data from the bluetooth client to the external semaphore
 */
void BluetoothClient::ReadControllerData(gea::GamepadData& _data_)
{
  _data_ = this->data;
}