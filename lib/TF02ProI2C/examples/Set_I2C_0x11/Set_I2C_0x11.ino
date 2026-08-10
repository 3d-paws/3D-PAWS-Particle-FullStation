/*
 *======================================================================================================================
 * Set_I2C_0x11 - Change TF02Pro from i2c address 0x10 to address 0x11
 * 
 * SEE https://en.benewake.com/uploadfiles/2024/04/20240426135442695.pdf
 * ======================================================================================================================
 */
#include <Wire.h>
#include <TF02ProI2C.h>

TF02ProI2C lidar10(Wire, 0x10);
TF02ProI2C lidar11(Wire, 0x11);

bool found10;
bool found11;

uint16_t dist, strength;
float tempC;

void ScanI2C() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    // Serial.println(address);

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);
  ScanI2C();
  delay(2000);

  found10= lidar10.begin();
  if (found10) {
    Serial.println("TF02-Pro 0x10 found");
    if (lidar10.readFrame(dist, strength, tempC, 150)) {
      Serial.print("Distance cm: ");
      Serial.print(dist);
      Serial.print("  Strength: ");
      Serial.print(strength);
      Serial.print("  Temp C: ");
      Serial.println(tempC);
      Serial.println();
      Serial.println("Setting I2C Address 0x11");

      if (lidar10.setAddress(0x11)) {
        Serial.println("Success - I2C Address is 0x11");
      }
      else {
        Serial.println("Failed Setting - I2C Address 0x11");
      }
    } else {
      Serial.println("Read failed");
    }
  }
  else {
    Serial.println("TF02-Pro 0x10 not found");
  }
  delay (2000);

  found11= lidar11.begin();
  if (found11) {
    Serial.println("TF02-Pro 0x11 found");
    if (lidar11.readFrame(dist, strength, tempC, 150)) {
      Serial.print("Distance cm: ");
      Serial.print(dist);
      Serial.print("  Strength: ");
      Serial.print(strength);
      Serial.print("  Temp C: ");
      Serial.println(tempC);
    } else {
      Serial.println("Read failed");
    }
  }
  else {
    Serial.println("TF02-Pro 0x10 not found");
  }
  delay (2000);
}

void loop() {
  uint16_t dist, strength;
  float tempC;

  if (found11) {
    Serial.print("TF02-Pro 0x11 ");
    if (lidar11.readFrame(dist, strength, tempC, 150)) {
      Serial.print("Distance cm: ");
      Serial.print(dist);
      Serial.print("  Strength: ");
      Serial.print(strength);
      Serial.print("  Temp C: ");
      Serial.println(tempC);
    } else {
      Serial.println("Read Failed");
    }

  }
  else {
    ScanI2C();
  }
  delay(5000);
}