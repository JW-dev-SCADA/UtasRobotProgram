
#include <Wire.h>
#include "SparkFun_Ublox_Arduino_Library_Series_6_7.h"

//compass part
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS3MDL lis3mdl;

//GPS

SFE_UBLOX_GPS myGPS;


#define NUMBER_OF_SENSORS 2

#define MUX_ADDR 0x70 //7-bit unshifted default I2C Address

//Enables a specific port number
void enableMuxPort(byte portNumber)
{
  if (portNumber > 7) portNumber = 7;

  Wire.beginTransmission(MUX_ADDR);
  //Read the current mux settings
  Wire.requestFrom(MUX_ADDR, 1);
  if (!Wire.available()) return; //Error
  byte settings = Wire.read();

  //Set the wanted bit to enable the port
  settings |= (1 << portNumber);

  Wire.write(settings);
  Wire.endTransmission();
}

//Disables a specific port number
void disableMuxPort(byte portNumber)
{
  if (portNumber > 7) portNumber = 7;

  Wire.beginTransmission(MUX_ADDR);
  //Read the current mux settings
  Wire.requestFrom(MUX_ADDR, 1);
  if (!Wire.available()) return; //Error
  byte settings = Wire.read();

  //Clear the wanted bit to disable the port
  settings &= ~(1 << portNumber);

  Wire.write(settings);
  Wire.endTransmission();
}



void setup()
{
  Serial.begin(115200);
  Serial.println("Project B Starts!");
  Wire.begin();

  //Initialize all the sensors

  enableMuxPort(0); //Tell mux to connect to port X
  myGPS.begin(); //Init the sensor connected to this port
  disableMuxPort(0);


  enableMuxPort(1); //Tell mux to connect to port X
  lis3mdl.begin_I2C();
  delay(10);
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_80_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  lis3mdl.configInterrupt(false, false, true, // enable z axis
                          true, // polarity
                          false, // don't latch
                          true); // enabled!
  disableMuxPort(1);

  Serial.println("All Sensors are online");
}

void loop()
{
  char comchar;  
  //for (byte x = 0 ; x < NUMBER_OF_SENSORS ; x++)
  //{

    if(Serial.available()>0){
      comchar = Serial.read();
      if (comchar =='0')
        {
        byte b = 0;
    enableMuxPort(b); //Tell mux to connect to this port, and this port only

    if (myGPS.begin() == true)
    {

      long latitude = myGPS.getLatitude();
      Serial.print(F("Lat: "));
      Serial.print(latitude);
      long longitude = myGPS.getLongitude();
      Serial.print(F(" Long: "));
      Serial.print(longitude);
     long heading = myGPS.getHeading();
     Serial.print(F(" Heading: "));
     Serial.print(heading*10^5);
     Serial.print(F(" (degrees * 10^-5)"));
      Serial.println(); // Print new line every time.
    }


    disableMuxPort(b); //Tell mux to disconnect from this port
        }
      if(comchar =='1'){
        byte b = 1;
         enableMuxPort(b);
          lis3mdl.begin_I2C(); 
          lis3mdl.read(); 
          sensors_event_t event; 
          lis3mdl.getEvent(&event);
          Serial.print("\tX: "); Serial.print(event.magnetic.x);
          Serial.print(" \tY: "); Serial.print(event.magnetic.y); 
          Serial.print(" \tZ: "); Serial.print(event.magnetic.z); 
          Serial.println();
          float x = event.magnetic.x;
          float y = event.magnetic.y;
          x=x-(-33.64);
          y=y-(-4.55);
            
float heading=0;
if (y != 0)
        {
            if (y < 0)
            {
                if ( x > 0)
                    heading = 57.2958 * atan(x / -y)+180; // Quadrant 1
                else
                    heading = 57.2958 * atan(-x /y)+180 ; // Quadrant 2
            }
            else
            {
                if (x < 0)
                    heading = 57.2958 * atan(-x / y) ; // Quadrant 3
                else
                    heading = 360- 57.2958 * atan(x / y); // Quadrant 4
            }
        }
        else
        {
            // atan of an infinite number is 90 or 270 degrees depending on X value
            if (x > 0)
                heading = 270;
            else
                heading = 90;
        }

   Serial.println(heading);



          
    disableMuxPort(b);
        }
      }

  delay(1); //Wait for next reading
}
