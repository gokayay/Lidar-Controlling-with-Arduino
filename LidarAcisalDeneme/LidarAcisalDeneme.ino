#include <RPLidar.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
RPLidar lidar;

// OLED display TWI address
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif




int maxMesafe = 0;
float aci = 0.0;


#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor.

void setup() {
  lidar.begin(Serial);
  pinMode(RPLIDAR_MOTOR, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  display.clearDisplay();
  // display a line of text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(27, 30);
  display.print("Hello, world!");

  display.display();
}

void loop() {
  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance; //distance value in mm unit
    float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
    bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement

    //perform data processing here...

    maxMesafe = distance;
    aci = angle * 255 / 360;


   
      if (aci > 0.0 && aci < 1.0 &&  quality != 0
          && quality < 50 && maxMesafe < 10000) {

        display.clearDisplay();
        display.setCursor(20, 30);
        display.print(maxMesafe);
        display.setCursor(20, 40);
        display.print(aci);
        display.setCursor(20, 50);
        display.print(quality);

        int oledMesafe = map(maxMesafe, 0, 1000, 0, 64);

        display.drawPixel(100, oledMesafe , WHITE);
        display.display();
      }
    


    
  } else {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor

    // try to detect RPLIDAR...
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      // detected...
      lidar.startScan();

      // start motor rotating at max allowed speed
      analogWrite(RPLIDAR_MOTOR, 255);
      delay(1000);
    }
  }
}
