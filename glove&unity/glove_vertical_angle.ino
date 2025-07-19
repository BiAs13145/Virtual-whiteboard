#include <Wire.h>

#include <DFRobot_QMC5883.h>

DFRobot_QMC5883 compass;

#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;               

int16_t ax, ay, az;
int16_t gx, gy, gz;
 double tx,ty,tz;
double angleX=0.00;
double angleY=0.00 ;
double angleZ=0.00;
 float targetangle ;
String  totalop0;
int su0;
float minX , maxX , minY , maxY , minZ , maxZ ;
struct v {
  float XAxis,YAxis,ZAxis;
}v;

bool firstRun=true;
// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO


void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(9600);

    // initialize device
 
    accelgyro.initialize();

    // verify connection


    // use the code below to change accel/gyro offset values
    

    // -76	-2359	1688	0	0	0

    accelgyro.setXAccelOffset(1600);
    accelgyro.setYAccelOffset(1207.5);
    accelgyro.setZAccelOffset(1000);
    accelgyro.setXGyroOffset(104);
    accelgyro.setYGyroOffset(23);
    accelgyro.setZGyroOffset(0);
//------------------------------------------------------------------------------
while (!compass.begin())
  {
   // Serial.println("Could not find a valid QMC5883 sensor, check wiring!");
    delay(500);
  }
   if(compass.isHMC()){
        //Serial.println("Initialize HMC5883");
        compass.setRange(HMC5883L_RANGE_1_3GA);
        compass.setMeasurementMode(HMC5883L_CONTINOUS);
        compass.setDataRate(HMC5883L_DATARATE_15HZ);
        compass.setSamples(HMC5883L_SAMPLES_8);
    }
   else if(compass.isQMC()){
        //Serial.println("Initialize QMC5883");
        compass.setRange(QMC5883_RANGE_2GA);
        compass.setMeasurementMode(QMC5883_CONTINOUS); 
        compass.setDataRate(QMC5883_DATARATE_50HZ);
        compass.setSamples(QMC5883_SAMPLES_8);

   }

  //---------------------------------------------------------- 

    
 pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A6, INPUT);
              
        }

 void initMinMax()
{
  minX = v.XAxis;
  maxX = v.XAxis;
  minY = v.YAxis;
  maxY = v.YAxis;
  minZ = v.ZAxis;
  maxZ = v.ZAxis;
}

void calibrat()
{
  if(v.XAxis < minX ) minX = v.XAxis;
  if(v.XAxis > maxX ) maxX = v.XAxis;
  if(v.YAxis < minY ) minY = v.YAxis;
  if(v.YAxis > maxY ) maxY = v.YAxis;
  if(v.ZAxis < minZ ) minZ = v.ZAxis;
  if(v.ZAxis > maxZ ) maxZ = v.ZAxis;
}

void Normalize(void)  //for mpu6050
{
  int range = 10;
  float Xsum = 0.0;
  float Ysum = 0.0;
  float Zsum = 0.0;

    while (range--){
      v.XAxis = ax;
      v.YAxis = ay;
      v.ZAxis = az;
      Xsum += v.XAxis;
      Ysum += v.YAxis;
      Zsum += v.ZAxis;
    }
    v.XAxis = Xsum/range;
    v.YAxis = Ysum/range;
    v.ZAxis = Zsum/range;
    if(firstRun){
      initMinMax();
      firstRun = false;
    }
    
    calibrat();
    v.XAxis= map(v.XAxis,minX,maxX,-360,360);
    v.YAxis= map(v.YAxis,minY,maxY,-360,360);
    v.ZAxis= map(v.ZAxis,minZ,maxZ,-360,360);
  }



void loop() {




  
    // read raw accel/gyro measurements from device
   accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.(&ax, &ay, &az);
    accelgyro.getAcceleration(&gx, &gy, &gz);
    
    Normalize();

    
    targetangle = atan(v.XAxis/sqrt(v.YAxis*v.YAxis+v.ZAxis*v.ZAxis))*(180/PI); 
    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif                    
    #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
//Serial.print("垂直");
        
//-----------------------------------------------------------------------------

Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);


  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0){
    heading += 2 * PI;
  }

  if (heading > 2 * PI){
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180/M_PI; 

 //Serial.print("水平:"); 
//Serial.print((targetangle/3.14159)*180); Serial.print(',');
//Serial.print(headingDegrees);
totalop0=(float)targetangle;

 //Serial.print(headingDegrees);Serial.print(',');
 
totalop0=totalop0+","+(float)headingDegrees; //first op string
 //------------------------------------------------------------

 String totalop1; // second stirng
 int v0= analogRead(A0); //finger angle
int v1= analogRead(A1);
int v2= analogRead(A2);
int v3= analogRead(A3);
int v6= analogRead(A6);
analogWrite(A0,v0);
analogWrite(A1,v1);
analogWrite(A2,v2);
analogWrite(A3,v3);
analogWrite(A6,v6);
if (v1>500) v1=1; else v1=0; //mapping to 0/1
if (v0>500) v0=1; else v0=0;
/*Serial.println(v0);Serial.print(',');
Serial.println(v1);//Serial.print(',');
/*Serial.print(v2);Serial.print(',');
Serial.print(v3);Serial.print(',');
Serial.println(v6);*/
totalop1="\0";
totalop1=totalop1+","+(int)v0+","+(int)v1;

totalop0=totalop0+totalop1; // output combine

Serial.println (totalop0);



        
// if (Serial.available() > 0) {
//     Serial.read();

//     Serial.print("stop!! 這什麼到底什麼閃現??");Serial.print("\n");
    
//       delay(100000e80);
//    }

   

    #endif


}
