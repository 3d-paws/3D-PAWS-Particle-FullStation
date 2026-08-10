#include <TF02ProI2C.h>

TF02ProI2C lidar(Wire, 0x10);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (lidar.begin()) {
    Serial.println("TF02-Pro found");
  } else {
    Serial.println("TF02-Pro not found");
  }

  // If you need to switch the sensor to I2C mode once:
  // lidar.setI2CMode();
  // delay(100);
  // lidar.saveSettings();
}

void loop() {
  uint16_t dist, strength;
  float tempC;

  if (lidar.readFrame(dist, strength, tempC, 150)) {
    Serial.print("Distance cm: ");
    Serial.print(dist);
    Serial.print("  Strength: ");
    Serial.print(strength);
    Serial.print("  Temp C: ");
    Serial.println(tempC);
  } else {
    Serial.println("Read failed");
  }

  delay(200);
}
