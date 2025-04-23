#include "DriveControl.h"
#include "BluetoothClient.h"
#include "Constants.h"

BluetoothClient m_BluetoothClient{Constants::Bluetooth::bluetoothCreateInfo};
DriveControl m_DriveControl{Constants::DriveSystem::DriveSystemCreateInfo};

TaskHandle_t BluetoothThread;
TaskHandle_t DriveThread;

gea::GamepadData data;
SemaphoreHandle_t Gamepad_Mutex;

void setup()
{
    Serial.begin(115200);
    analogWriteResolution(8); //8 bit, [0,255]

    Gamepad_Mutex = xSemaphoreCreateMutex();
    if (Gamepad_Mutex == NULL) {
        gea::printStrLn("Mutex creation failed\n");
        return;
    }
  // Create Task1 pinned to core 0
  xTaskCreatePinnedToCore(
    BluetoothThreadFunc, // Function to implement the task
    "Bluetooth",         // Name of the task
    10000,               // Stack size
    NULL,                // Parameter
    1,                   // Task priority
    &BluetoothThread,    // Task handle
    0);                  // Core 0

  // Create Task2 pinned to core 1
  xTaskCreatePinnedToCore(
    DriveCodeThreadFunc,
    "Drive",
    10000,
    NULL,
    1,
    &DriveThread,
    1);          // Core 1
}

void loop() {}

 void BluetoothThreadFunc(void * parameter) {
  m_BluetoothClient.Setup();
   for(;;) {
      m_BluetoothClient.Loop();

      if (xSemaphoreTake(Gamepad_Mutex, portMAX_DELAY) == pdTRUE) {
          m_BluetoothClient.ReadControllerData(data);
          xSemaphoreGive(Gamepad_Mutex);
      }

      vTaskDelay(10 / portTICK_PERIOD_MS);
   }
 }

 void DriveCodeThreadFunc(void * parameter) {
   for(;;) {

      if (xSemaphoreTake(Gamepad_Mutex, portMAX_DELAY) == pdTRUE) {
        m_DriveControl.WriteControllerData(data);
        xSemaphoreGive(Gamepad_Mutex);
      }

      m_DriveControl.Step();
      m_DriveControl.Poststep();
      vTaskDelay(10 / portTICK_PERIOD_MS);
   }
 }