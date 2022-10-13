

#include <Adafruit_SensorLab.h>
Adafruit_SensorLab lab;

Adafruit_Sensor *mag;
sensors_event_t mag_event;

sensors_event_t event;
float min_x, max_x, mid_x;
float min_y, max_y, mid_y;
float min_z, max_z, mid_z;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  
  Serial.println(F("Sensor Lab - Magnetometer Calibration!"));
  lab.begin();
  
  Serial.println("Looking for a magnetometer");
  mag = lab.getMagnetometer();
  if (! mag) {
    Serial.println(F("Could not find a magnetometer, check wiring!"));
    while(1) delay(10);
  }
  mag->printSensorDetails();
  delay(100);

  mag->getEvent(&event);
  min_x = max_x = event.magnetic.x;
  min_y = max_y = event.magnetic.y;
  min_z = max_z = event.magnetic.z;
  delay(10);
}



void loop() {
  mag->getEvent(&event);
  float x = event.magnetic.x;
  float y = event.magnetic.y;
  float z = event.magnetic.z;
  
  Serial.print("Mag: (");
  Serial.print(x); Serial.print(", ");
  Serial.print(y); Serial.print(", ");
  Serial.print(z); Serial.print(")");

  min_x = min(min_x, x);
  min_y = min(min_y, y);
  min_z = min(min_z, z);

  max_x = max(max_x, x);
  max_y = max(max_y, y);
  max_z = max(max_z, z);

  mid_x = (max_x + min_x) / 2;
  mid_y = (max_y + min_y) / 2;
  mid_z = (max_z + min_z) / 2;
  Serial.print(" Hard offset: (");
  Serial.print(mid_x); Serial.print(", ");
  Serial.print(mid_y); Serial.print(", ");
  Serial.print(mid_z); Serial.print(")");  

  Serial.print(" Field: (");
  Serial.print((max_x - min_x)/2); Serial.print(", ");
  Serial.print((max_y - min_y)/2); Serial.print(", ");
  Serial.print((max_z - min_z)/2); Serial.println(")");    
  delay(10); 


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


 delay(100);

  
}
