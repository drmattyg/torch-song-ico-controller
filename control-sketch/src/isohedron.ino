#include <Adafruit_NeoPixel.h>  //For the LED Strip
#include <CurieIMU.h>
 /* We are using Serial1 (connected to XBee) instead of BLE
#include <CurieBLE.h>
BLEPeripheral     blePeripheral;
BLEService        torchSongCtl("180D");
BLECharacteristic torchSongFace("2A37",BLERead|BLENotify,sizeof(int));
  */
#define DEBUG false

#define NEOPIXEL_PIN 6
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ400);


void setup()
{
  Serial1.begin(9600);
  //This starts about 5 seconds after power is connected
  //the arduino board should be held steady and face up (PCB flat with I/O pins up)
  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(2);

  if (DEBUG) Serial1.print("Starting Gyroscope calibration...");
  CurieIMU.autoCalibrateGyroOffset();
  if (DEBUG) Serial1.println(" Done");
  if (DEBUG) Serial1.print("Starting Acceleration calibration...");
  CurieIMU.autoCalibrateXAccelOffset(0);
  CurieIMU.autoCalibrateYAccelOffset(0);
  CurieIMU.autoCalibrateZAccelOffset(1);
  if (DEBUG) Serial1.println(" Done");  

  if (DEBUG) Serial1.println("Enabling Gyroscope/Acceleration offset compensation");
  CurieIMU.setGyroOffsetEnabled(true);
  CurieIMU.setAccelOffsetEnabled(true);  

  strip.begin();
  strip.show(); // Initialize all NeoPixels to 'off'
}

// these are the previous icosahedron face normal vectors
int32_t face_up_normal[20][3] = {
  { 0 , 0 , 3464 },         //0
  { 1825 , -1414 , 2581 },  //1 
  { 311 , 2288 , 2581 },    //2 
  { -2137 , -874 , 2581 },  //3 
  { 2330 , 2288 , 1154 },   //4 
  { 3265 , 0 , 1154 },      //5 
  { -1632 , -2828 , 1154 }, //6 
  { -3146 , 874 , 1154 },   //7 
  { -1632 , 2828 , 1154 },  //8
  { 816 , -3162 , 1154 },   //9 
  { 1632 , -2828 , -1154 }, //10 
  { 1632 , 2828 , -1154 },  //11 
  { -3265 , 0 , -1154 },    //12 
  { -2330 , -2288 , -1154 },//13 
  { -816 , 3162 , -1154 },  //14 
  { 3146 , -874 , -1154 },  //15 
  { -311 , -2288 , -2581 }, //16 
  { -1825 , 1414 , -2581 }, //17 
  { 2137 , 874 , -2581 },   //18   
  { 0 , 0 , -3464 }         //19 
};

//new normals taken from experimenting
//I tried the { z y -x } rotation of above but it still didn't work so well
int32_t normal[20][6] = { 
  //first 3 numbers are the vector for each face normal, next 3 are the closest vertex
  { 17298, -300, -1588, 0,1,5 },    //0 burning man
  { 6021, -9238, -13449, 1,2,7 },   //1 sum
  { 13766, 9188, 4157, 0,4,5 },     //2 p: 
  { 13390, -9462, 4547, 1,5,6 },    //3 poop
  {  6119, 9066, -13586, 0,2,3 },   //4 hand
  { 14058, -738, -12289, 0,1,2},    //5 alpha
  { 8729, -5705, 14330, 5,6,10 },   //6 theta
  { 7356, 5917, 13882, 4,5,10 },    //7 30
  { 8362, 14836, -2870, 0,3,4 },    //8 waves  
  { 6157, -15254, -2667, 1,6,7 },   //9 phi
  { -11674, -9289, -5419, 7,8,11 }, //10 fx
  { -11257, 9598, -5901, 8,9,3 },   //11 sharp
  { -10668, 294, 11314, 9,10,11 },  //12 blank
  { -4495, -15086, 2047, 6,7,11 },  //13 note
  { -4063, 15401, 1709, 3,9,4 },    //14 fancy
  { -5392, -5521, -15105, 2,8,7 },  //15 pi
  { -2595, -9221, 12733, 6,10,11 }, //16 delta
  { -2181, 9440, 12302, 4,9,10 },   //17 omega
  { -3500, 5347, -15073, 8,3,2 },   //18 integral  
  { -15409, 231, 475, 8,9,11 }      //19 heart
};

int32_t vertex[12][8] = {  
  //first 3 numbers is the vector for each vertex, next 5 are the closest faces
  { 14879 , 6304 , -7848, 8,2,0,5,4 },     //0  
  { 14351 , -8778 , -6650, 0,3,9,1,5 },    //1   
  { 4000 , 455 , -17382, 1,15,18,4,5 },    //2   
  { -1088 , 13648 , -8828, 14,11,8,4,18},  //3   
  { 5650 , 13638 ,  7500, 17,7,2,8,14},    //4   
  { 15948 , -764 , 8034, 3,6,7,2,0},       //5   
  { 5125 , -14487 , 7129, 16,13,7,3,6},    //6   
  { -1804 , -14064 , -8727, 15,10,1,9,13}, //7   
  { -11505 , 603 , -10287, 19,11,18,10,15},//8   
  { -10691 , 9232 , 3882, 19,12,17,14,11}, //9   
  { 5000 , 5000 , 22700, 16,6,7,17,12},    //10  
  { -11152 , -8809 , 4444, 19,10,13,16,12},//11  
};

//Accelerometer and gyroscope globals
int16_t aX,  aY,  aZ,  gX,  gY,  gZ;

//true if the _vertex_ is beasides _face_
bool isfacevertex(int face, int vertex) {
   if ( normal[face][3] == vertex ) return true;
   if ( normal[face][4] == vertex ) return true;
   if ( normal[face][5] == vertex ) return true;
   return false;
}

// Compute unnormalized dot product with a face.
int32_t dotprod(int k)
{
  return (int32_t)aX*normal[k][0]+(int32_t)aY*normal[k][1]+(int32_t)aZ*normal[k][2];
}
// Compute unnormalized dot product with a vertex.
int32_t dotprodv(int k)
{
  return (int32_t)aX*vertex[k][0]+(int32_t)aY*vertex[k][1]+(int32_t)aZ*vertex[k][2];
}
// Find the most up face
int faceup()
{
  int32_t maxval(dotprod(0));
  int32_t tmpval(0);
  int face(0);
  for (int k = 1; k < 20; k++) {
    tmpval = dotprod(k);
    if ( tmpval > maxval) {
      face = k;
      maxval = tmpval;
    }
  }
  return face;
}
// Find the most up vertex
int vertexup()
{
  int32_t maxval(dotprodv(0));
  int32_t tmpval(0);
  int vertex(0);
  for (int k = 1; k < 12; k++) {
    tmpval = dotprodv(k);
    if ( tmpval > maxval) {
      vertex = k;
      maxval = tmpval;
    }
  }
  return vertex;
}
// Find if the iso was picked up
// 20000 is the motion threshold for detecting if it was picked up
// *we may need to adjust this
bool pickedup()
{
  static int32_t last_gyro_mag(0);
  int32_t gyro_mag = gX*gX+gY*gY+gZ*gZ;
  int32_t gyro_diff = last_gyro_mag - gyro_mag; 
  bool result = true;
  if (( gyro_diff > -20000 ) && (gyro_diff < 20000 )) {
    if (DEBUG) Serial1.print("Not moving diff=");
    if (DEBUG) Serial1.println( gyro_diff );
    result = false;     
  } else {
    if (DEBUG) Serial1.print("Moving diff=");
    if (DEBUG) Serial1.println( gyro_diff );    
  }
  last_gyro_mag = gyro_mag;
  return result;
}
// The following are copied from the neopixel sample code and
// have varios lighting effects:

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(int j, uint8_t wait) {
  //for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  //}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//Various state variables
int upface(-1);
uint16_t iters=100;
int upvertex(0);
bool ispickedup(0);
bool imbored(false);

//Main loop
void loop()
{
  int face; 
  iters++; 
  delay(50);

  /* Other sample effectes
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50); // Red
  theaterChase(strip.Color(0, 0, 127), 50); // Blue
  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
  */

  //Read the gyro and accelerometer  
  CurieIMU.getMotion6(&aX, &aY, &aZ, &gX, &gY, &gZ);
  
  //Determine up face, if it is different from previous
  //we were sending the data, but i think the gyro is accurate enouph
  //we don't need to that logic, and thus will allow the user to select
  //the same face twice.  
  face = faceup();
  if (face != upface) {
    upface = face;
  }
  if (ispickedup != pickedup()) {
    ispickedup = !ispickedup;
    if (ispickedup) {
      //If the iso is picked up wipe all the led color to white
      colorWipe(0xffffff, 50);
      delay(500);
    } else {
      //If the iso is placed down transmit the upface
      Serial1.println(upface);
    }
    //If any change in state reset the iter counter (which triggers bored routine)
    iters=100;
    imbored=false;
  }

  //uber debug
  if (DEBUG) {
    Serial1.print("a/g: ");
    Serial1.print(aX);
    Serial1.print(" ");
    Serial1.print(aY);
    Serial1.print(" ");
    Serial1.print(aZ);
    Serial1.print(" ");
    Serial1.print(gX);
    Serial1.print(" ");
    Serial1.print(gY);
    Serial1.print(" ");
    Serial1.print(gZ);
    Serial1.print(" ");   
    Serial1.println( ispickedup );   
  }

  //Now do the reqular routine based on if it is picked up or not
  if (ispickedup) {
    //Sets the up vertex to white while the iso is being handled
    //we might want to do some anti-aliasing so the light does not jump from
    //vertex to vertex and the fades from one to the other (might be cool)
    for (uint16_t i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    upvertex = vertexup();
    strip.setPixelColor(upvertex, strip.Color(255, 255, 255));
    strip.show();
  } else {
    //Sets the 3 leds around the face to blue and pulses all the other
    //leds red while the iso is stationary. also triggers the 
    //theaterChaseRainbow effect after 2000 iters (about 100 seconds)
    //and theaterChaseRaibow lasts 256*3 iters (about 38 seconds)
    for (uint16_t i=0; i < strip.numPixels(); i++) {
      if (iters % 2000 == 0) {imbored = true;iters=0;}
      if (imbored) {theaterChaseRainbow(iters, 50);}
      if (iters > 256) imbored = false;
      
      int fade = abs( iters%200 - 100 );
      if (isfacevertex(face,i)) strip.setPixelColor(i, strip.Color(0, 0, 255));
      else strip.setPixelColor(i, strip.Color(0, fade, 0));
    }
    strip.show();
  }  
  /* We are using Serial1 (connected to XBee) instead of BLE
  if (central) {
    Serial.println("Shoob");
    torchSongFace.setValue((unsigned char*)&upface,sizeof(int));
  }
  */
}