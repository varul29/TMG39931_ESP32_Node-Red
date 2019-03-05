#include "BluetoothSerial.h"
#include<Wire.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
#define Addr 0x39
volatile float g,r,b,cD,cc;

void setup() {
  Wire.begin(21,22);
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
//  tmgLight();
}

void loop() {

  //volatile float g,r,b,cD,cc; 
  tmgLight();
  
  if (Serial.available()) {
    delay(100); 
    SerialBT.println("Hello");
    SerialBT.println(String(g,2));
    SerialBT.println(String(r,2));
    SerialBT.println(String(b,2));
    SerialBT.println(String(cD,2));
    SerialBT.println(String(cc,0));
    }
  
  delay(20);
}

void tmgLight(){

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select Enable register
  Wire.write(0x80);
  // Power ON, ALS enable, Proximity enable, Wait enable
  Wire.write(0x0F);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select ADC integration time register
  Wire.write(0x81);
  // ATIME : 712ms, Max count = 65535 cycles
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select Wait time register
  Wire.write(0x83);
  // WTIME : 2.78ms
  Wire.write(0xFF);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x8F);
  // AGAIN is 1x
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);

  unsigned int data[9];
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x94);
  // Stop I2C transmission
  Wire.endTransmission();
  
  // Request 9 bytes of data
  Wire.requestFrom(Addr, 9);
   
  // Read the 9 bytes of data
  // cData channel LSB, cData channel MSB, Red channel LSB, Red channel MSB
  // Green channel LSB, Green channel MSB, Blue channel LSB, Blue channel MSB, proximity
  if(Wire.available() == 9) 
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
    data[6] = Wire.read();
    data[7] = Wire.read();
    data[8] = Wire.read();
   }
   
  // Convert the data
  float cData = data[1] * 256.0 + data[0];
  float red = data[3] * 256.0 + data[2];
  float green = data[5] * 256.0 + data[4];
  float blue = data[7] * 256.0 + data[6];

  unsigned int c = data[8];
  
  // Output data to serial monitor       
  Serial.print("Green Color Luminance : ");
  Serial.println(green);
  Serial.print("Red Color Luminance : ");
  Serial.println(red)  ;
  Serial.print("Blue Color Luminance : ");
  Serial.println(blue) ;
  Serial.print("InfraRed Luminance : ");
  Serial.println(cData) ;
  Serial.print("Proximity of the device : ");
  Serial.println(c);
  delay(500);

  g = green;
  r = red;
  b = blue;
  cD = cData;
  c = cc;
}
