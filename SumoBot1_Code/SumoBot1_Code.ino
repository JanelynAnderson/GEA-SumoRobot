#include "DriveControl.h"
#include "BluetoothClient.h"
#include "Constants.h"

BluetoothClient m_BluetoothClient;
DriveControl m_DriveControl{Constants::DriveSystem::DriveSystemCreateInfo};

void setup()
{

}

void loop()
{
  // put your main code here, to run repeatedly:
  m_BluetoothClient.step();
  m_DriveControl.step();
  m_DriveControl.poststep();
}
