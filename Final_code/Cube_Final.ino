#define myLayer 2
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include <SparkFun_SCD30_Arduino_Library.h> // Include the SCD30 library
#define PIN 6
//////////////////////////////////

volatile uint16_t mymatrix[16][8];  // color of each LED in the matrix
int brightness = 80;  // set brightness
///Simulate LED light strip into cube
Adafruit_NeoMatrix cube = Adafruit_NeoMatrix(8, 8, 8, 1, PIN,
                          NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                          NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                          NEO_RGB            + NEO_KHZ800);



#define White 43
#define Black 0
#define Red 1
#define Red2 44
#define Blue 45
#define Orange 4
#define Yellow 9
#define Green 15
#define Aqua 20
#define Blue 29
#define Purple 34
uint32_t mycolor;
const byte color [46][3] PROGMEM = {  // the color pallette table
  {0, 0, 0}, //Black
  {7, 0, 0}, {7, 1, 0}, {6, 1, 0}, {6, 2, 0}, {5, 2, 0}, {5, 3, 0}, {4, 3, 0}, {4, 4, 0}, {3, 4, 0}, {3, 5, 0}, {2, 5, 0}, {2, 6, 0}, {1, 6, 0}, {1, 7, 0}, // Red to Green
  {0, 7, 0}, {0, 7, 1}, {0, 6, 1}, {0, 6, 2}, {0, 5, 2}, {0, 5, 3}, {0, 4, 3}, {0, 4, 4}, {0, 3, 4}, {0, 3, 5}, {0, 2, 5}, {0, 2, 6}, {0, 1, 6}, {0, 1, 7}, // Green to Blue
  {0, 0, 7}, {1, 0, 7}, {1, 0, 6}, {2, 0, 6}, {2, 0, 5}, {3, 0, 5}, {3, 0, 4}, {4, 0, 4}, {4, 0, 3}, {5, 0, 3}, {5, 0, 2}, {6, 0, 2}, {6, 0, 1}, {7, 0, 1}, // Blue to Red
  {7, 7, 7},{7, 2, 2},{3, 5, 7},  // White
};

byte myloc[3] = {2, 2, 2};
const byte cubetPath [50][3] PROGMEM = {  // cubet path
  {0, 0, 3}, {0, 1, 3}, {0, 2, 3}, {0, 3, 3}, {1, 3, 3}, {2, 3, 3}, {3, 3, 3}, {3, 2, 3}, {3, 1, 3}, {3, 0, 3}, {2, 0, 3}, {1, 0, 3}, {0, 0, 3},
  {0, 2, 0}, {0, 2, 1}, {0, 2, 2}, {0, 2, 3}, {1, 2, 3}, {2, 2, 3}, {3, 2, 3}, {3, 2, 2}, {3, 2, 1}, {3, 2, 0}, {2, 2, 0}, {1, 2, 0}, {0, 2, 0},
  {0, 0, 2}, {1, 0, 2}, {2, 0, 2}, {3, 0, 2}, {3, 1, 2}, {3, 2, 2}, {3, 3, 2}, {2, 3, 2}, {1, 3, 2}, {0, 3, 2}, {0, 2, 2}, {0, 1, 2}, {0, 0, 2},
  {2, 0, 1}, {1, 0, 1}, {0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3, 1}, {3, 1, 1}, {3, 2, 1}, {2, 0, 1}
};
unsigned long start;

//////////////
SCD30 airSensor;
bool co2good;
int numspeed = 40;
int numstay= 400;

/////////////////////
int redPin = 10;
int greenPin = 11;
int bluePin = 12;
int buttonPin = 13;
int buttonState = 0;
int lastButtonState = 0;
int option = 0; // Variable to track the current option
#define COMMON_ANODE
int radiusChange = 0; // Variables used to control radius changes
////////////////////
//Skip remaining animation after button press
bool istemp=false;
bool ishum=false;
bool isco2=false;
////////////////


void setup() {
  Wire.begin();

  Serial.begin(9600);
  airSensor.begin(Wire); // Initialize the SCD30 sensor
    if (airSensor.begin() == false) {
    Serial.println("SCD30 not detected, please check the wiring!");
    while (1); // If the sensor is not detected, halt the program
  }
  //////////////////////////
  cube.begin();
  cube.setTextWrap(false);
  cube.setBrightness(brightness);
  initBall();
  /////////////////////////button pin
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  pinMode(buttonPin, INPUT_PULLUP); // Set the button as an input
  attachInterrupt(digitalPinToInterrupt(buttonPin),button111,CHANGE);
////////////////////////////
  
  delay(2000);
}


void loop() {
  //Switch between four animation modes according to button status
  switch(option){
    case 0:
  looptype1();
    //return;
    break;
    case 1:
  looptype2();
    //return;
    break;
    case 2:
    looptype3();
    //return;
    break;
    case 3:
    looptype4();
    //return;
    break;
  }

  

}





//LED coordinates and color settings
void LED(int x, int y, int z, unsigned int mycolor){
  if (z%2) cube.drawPixel((z+1)*8-x-1, y, GetColor(mycolor));
  else cube.drawPixel(x+(z*8),y, GetColor(mycolor));
}
void LED1(int z, int y, int x, unsigned int mycolor){
  if (z%2) cube.drawPixel((z+1)*8-x-1, y, GetColor(mycolor));
  else cube.drawPixel(x+(z*8),y, GetColor(mycolor));
}
void LED2(int z, int y, int x, byte red, byte green, byte blue){
  if (z%2) cube.drawPixel((z+1)*8-x-1, y, GetColor2(red,green,blue));
  else cube.drawPixel(x+(z*8),y, GetColor2(red,green,blue));
  //cube.show();
}
uint32_t GetColor(int thecolor) {
  byte myred = 36*pgm_read_byte_near(&color[thecolor][0]);
  byte mygreen = 36* pgm_read_byte_near(&color[thecolor][1]);
  byte myblue = 36*pgm_read_byte_near(&color[thecolor][2]);
  return cube.Color(myred,mygreen, myblue);
}

uint32_t GetColor2(byte red, byte green, byte blue) {
  byte myred = 36*pgm_read_byte_near(red);
  byte mygreen = 36* pgm_read_byte_near(green);
  byte myblue = 36*pgm_read_byte_near(blue);
  return cube.Color(myred,mygreen, myblue);
}

//clear all leds
void clearCube(){
cube.fill(Black);
}

void pause() {  // pause between animations
  clearCube();
  delay(500);
}

void showCube(int mytime) {
  delay(20 * mytime);
  clearCube();
}


//co2 animation
//////////////////////////////
void wave(int co2) {
  int wavespeed;
  if(co2>1500){
    mycolor =1;
    wavespeed=30;
  }else{
    mycolor =15;
    wavespeed=80;
  }
  int sinewavearray[8], addr, sinemult[8], colselect, rr = 0, gg = 0, bb = 15, addrt;
  int sinewavearrayOLD[8], select, subZ = -7, subT = 7, multi = 0;
  sinewavearray[0] = 0;
  sinemult[0] = 1;
  sinewavearray[1] = 1;
  sinemult[1] = 1;
  sinewavearray[2] = 2;
  sinemult[2] = 1;
  sinewavearray[3] = 3;
  sinemult[3] = 1;
  sinewavearray[4] = 4;
  sinemult[4] = 1;
  sinewavearray[5] = 5;
  sinemult[5] = 1;
  sinewavearray[6] = 6;
  sinemult[6] = 1;
  sinewavearray[7] = 7;
  sinemult[7] = 1;

  start = millis();

  while (millis() - start < 30000) {
    if( option!= 0 && !isco2){
      
      clearCube();
      return;
    }else if(option!= 3 && isco2){
      clearCube();
      //isco2=false;
      return;
    }
    for (addr = 0; addr < 8; addr++) {
      if (sinewavearray[addr] == 7) {
        sinemult[addr] = -1;
      }
      if (sinewavearray[addr] == 0) {
        sinemult[addr] = 1;
      }
      sinewavearray[addr] = sinewavearray[addr] + sinemult[addr];
    }
    if (sinewavearray[0] == 7) {
      select = random(3);
      if (select == 0) {
        rr = random(1, 16);
        gg = random(1, 16);
        bb = 0;
      }
      if (select == 1) {
        rr = random(1, 16);
        gg = 0;
        bb = random(1, 16);
      }
      if (select == 2) {
        rr = 0;
        gg = random(1, 16);
        bb = random(1, 16);
      }


    }

/////////////////////////////////////////////////
    for (addr = 0; addr < 8; addr++) {
      LED1(sinewavearrayOLD[addr], addr, 0, Black);
      LED1(sinewavearrayOLD[addr], 0, addr,Black);
      LED1(sinewavearrayOLD[addr], subT - addr, 7, Black);
      LED1(sinewavearrayOLD[addr], 7, subT - addr, Black);
      LED1(sinewavearray[addr], addr, 0, mycolor);
      LED1(sinewavearray[addr], 0, addr, mycolor);
      LED1(sinewavearray[addr], subT - addr, 7, mycolor);
      LED1(sinewavearray[addr], 7, subT - addr, mycolor);
      //cube.show();
    }

    for (addr = 1; addr < 7; addr++) {
      LED1(sinewavearrayOLD[addr + multi * 1], addr, 1, Black);
      LED1(sinewavearrayOLD[addr + multi * 1], 1, addr, Black);
      LED1(sinewavearrayOLD[addr + multi * 1], subT - addr, 6, Black);
      LED1(sinewavearrayOLD[addr + multi * 1], 6, subT - addr, Black);
      LED1(sinewavearray[addr + multi * 1], addr, 1, mycolor);
      LED1(sinewavearray[addr + multi * 1], 1, addr, mycolor);
      LED1(sinewavearray[addr + multi * 1], subT - addr, 6, mycolor);
      LED1(sinewavearray[addr + multi * 1], 6, subT - addr, mycolor);
      //cube.show();
    }

    for (addr = 2; addr < 6; addr++) {
      LED1(sinewavearrayOLD[addr + multi * 2], addr, 2, Black);
      LED1(sinewavearrayOLD[addr + multi * 2], 2, addr, Black);
      LED1(sinewavearrayOLD[addr + multi * 2], subT - addr, 5,Black);
      LED1(sinewavearrayOLD[addr + multi * 2], 5, subT - addr, Black);
      LED1(sinewavearray[addr + multi * 2], addr, 2, mycolor);
      LED1(sinewavearray[addr + multi * 2], 2, addr, mycolor);
      LED1(sinewavearray[addr + multi * 2], subT - addr, 5, mycolor);
      LED1(sinewavearray[addr + multi * 2], 5, subT - addr, mycolor);
      //cube.show();
    }
    for (addr = 3; addr < 5; addr++) {
      LED1(sinewavearrayOLD[addr + multi * 3], addr, 3, Black);
      LED1(sinewavearrayOLD[addr + multi * 3], 3, addr, Black);
      LED1(sinewavearrayOLD[addr + multi * 3], subT - addr, 4, Black);
      LED1(sinewavearrayOLD[addr + multi * 3], 4, subT - addr, Black);
      LED1(sinewavearray[addr + multi * 3], addr, 3, mycolor);
      LED1(sinewavearray[addr + multi * 3], 3, addr, mycolor);
      LED1(sinewavearray[addr + multi * 3], subT - addr, 4, mycolor);
      LED1(sinewavearray[addr + multi * 3], 4, subT - addr, mycolor);
      //cube.show();
    }
    /////////////////////////////////////////////////////
    for (addr = 0; addr < 8; addr++)
      sinewavearrayOLD[addr] = sinewavearray[addr];

    cube.show();
    //Skip remaining animation
    if( option!= 0 && !isco2){
      
      clearCube();
      return;
    }else if(option!= 3 && isco2){
      clearCube();
      //isco2=false;
      return;
    }
    
    delay(wavespeed);
  }
//Skip remaining animation
if( option!= 0 && !isco2){
      
      clearCube();
      return;
    }else if(option!= 3 && isco2){
      clearCube();
      //isco2=false;
      return;
    }

}


///////Numbers and letters animation

void num0(int mycolor){
  int z,y,x;
  for(int y=0;y<8;y++){
    for(x=2;x<6;x++){
      LED1(0, y, x, mycolor);
      LED1(7, y, x, mycolor);
    }
    for(z=1;z<7;z++){
      LED1(z, y, 1, mycolor);
      LED1(z, y, 6, mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
    //cube.show();
    
  }
  
  delay(numstay);
}
//////////////////////////////
void num1(int mycolor){
  for(int y=0;y<8;y++){
    for(int x1=1;x1<7;x1++){
      LED1(0, y, x1, mycolor);
    }
    for(int z1=1;z1<8;z1++){
      LED1(z1, y, 3, mycolor);
      LED1(z1, y, 4, mycolor);
    }
    LED1(5, y, 1, mycolor);
    LED1(6, y, 2, mycolor);
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
  }
  delay(numstay);
}
///////////////////////////

void num2(int mycolor){
  int z2[4]={1,2,3,4};
  int x2[4]={2,3,4,5};
 for(int y=0;y<8;y++){
    for(int x1=2;x1<7;x1++){
      LED1(0, y, x1, mycolor);
    }
    for(int i=0;i<4;i++){
      LED1(z2[i],y,x2[i],mycolor);
    }
    for(int z3=5;z3<7;z3++){
      LED1(z3,y,2,mycolor);
      LED1(z3,y,6,mycolor);
    }
    for(int x2=3;x2<6;x2++){
      LED1(7,y,x2,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }

  }
  delay(numstay);
}

////////////////////////////

void num3(int mycolor){
  for(int y=0;y<8;y++){
    for(int x1=3;x1<6;x1++){
      LED1(0,y,x1,mycolor);
      LED1(7,y,x1,mycolor);
    }
    for(int z1=1;z1<3;z1++){
      LED1(z1,y,6,mycolor);
    }
    for(int z2=5;z2<7;z2++){
      LED1(z2,y,6,mycolor);
    }
    for(int z3=3;z3<5;z3++){
      LED1(z3,y,5,mycolor);
    }
    LED1(1,y,2,mycolor);
    LED1(6,y,2,mycolor);
    LED1(3,y,4,mycolor);
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
  
  }
  delay(numstay);
}

//////////////////////////////////////////////

void num4(int mycolor){
  for(int y=0;y<8;y++){
    for(int z1=0;z1<8;z1++){
      LED1(z1,y,4,mycolor);
    }
    for(int z2=3;z2<8;z2++){
      LED1(z2,y,1,mycolor);
    }
    for(int x1=1;x1<7;x1++){
      LED1(3,y,x1,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }

  }
  delay(numstay);
}

///////////////////////////////////

void num5(int mycolor){
  for(int y=0;y<8;y++){
    for(int x1=2;x1<7;x1++){
      LED1(7,y,x1,mycolor);
    }
    for(int x2=2;x2<6;x2++){
      LED1(4,y,x2,mycolor);
      LED1(0,y,x2,mycolor);
    }
    for(int z1=5;z1<7;z1++){
      LED1(z1,y,2,mycolor);
    }
    for(int z2=1;z2<4;z2++){
      LED1(z2,y,6,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
    
  }
  delay(numstay);

}
////////////////////

void num6(int mycolor){
 for(int y=0;y<8;y++){
    for(int x1=2;x1<7;x1++){
      LED1(0,y,x1,mycolor);
      LED1(4,y,x1,mycolor);
      LED1(7,y,x1,mycolor);
    }
    for(int z1=1;z1<4;z1++){
      LED1(z1,y,2,mycolor);
    }
    for(int z2=1;z2<7;z2++){
      LED1(z2,y,2,mycolor);
    }
    for(int z2=0;z2<5;z2++){
      LED1(z2,y,6,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
    //cube.show();
  }
  delay(numstay);
}

//////////////
void num7(int mycolor){
  int z1[7]={0,0,0,0,2,4,6};
  int z2[7]={0,0,0,2,4,6,8};
  for(int y=0;y<8;y++){
    for(int x1=2;x1<7;x1++){
      LED1(7,y,x1,mycolor);
    }
    for(int x2=3;x2<7;x2++){
      for(int z=z1[x2];z<z2[x2];z++){
        LED1(z,y,x2,mycolor);
      }
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
    //cube.show();
  }
  delay(numstay);
}
/////////////////
void num8(int mycolor){
  int z[6]={6,6,5,5,3,3};
  int x[6]={6,1,5,2,5,2,};
  for(int y=0;y<8;y++){
    for(int x1=2;x1<6;x1++){
      LED1(0,y,x1,mycolor);
      LED1(7,y,x1,mycolor);
    }
    for(int z1=1;z1<3;z1++){
      LED1(z1,y,1,mycolor);
      LED1(z1,y,6,mycolor);
    }
    for(int x2=3;x2<5;x2++){
      LED1(4,y,x2,mycolor);
    }
    for(int i=0;i<6;i++){
      LED1(z[i],y,x[i],mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
  }
  //cube.show();
  delay(numstay);
}
/////////////
void num9(int mycolor){
  for(int y=0;y<8;y++){
    for(int x1=3;x1<6;x1++){
      LED1(0,y,x1,mycolor);
      LED1(3,y,x1,mycolor);
      LED1(7,y,x1,mycolor);
    }
    for(int z1=1;z1<7;z1++){
      LED1(z1,y,6,mycolor);
    }
    for(int z2=4;z2<7;z2++){
      LED1(z2,y,2,mycolor);
    }
    LED1(1,y,2,mycolor);
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
  }
  //cube.show();
  delay(numstay);
}
/////////////
void T(int mycolor){
  for(int y=0;y<8;y++){
    for(int x1=1;x1<8;x1++){
      LED1(7,y,x1,mycolor);
    }
    for(int z1=0;z1<7;z1++){
      LED1(z1,y,4,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
    
  }
  delay(numstay);
}
///////////////////
void U(int mycolor){
  for(int y=0;y<8;y++){
    for(int x1=2;x1<6;x1++){
      LED1(0,y,x1,mycolor);
    }
    for(int z1=1;z1<8;z1++){
      LED1(z1,y,1,mycolor);
      LED1(z1,y,6,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
  }
  delay(numstay);
}
///////////////////////
void M(int mycolor){
  for(int y=0;y<8;y++){
    for(int z1=0;z1<8;z1++){
      LED1(z1,y,1,mycolor);
      LED1(z1,y,7,mycolor);
    }
    for(int z2=3;z2<5;z2++){
      LED1(z2,y,4,mycolor);
    }
    LED1(6,y,2,mycolor);
    LED1(6,y,6,mycolor);
    LED1(5,y,3,mycolor);
    LED1(5,y,5,mycolor);

    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
    
  }
  delay(numstay);
}
///////////////////////////////
void P(int mycolor){
  for(int y=0;y<8;y++){
    for(int z1=0;z1<8;z1++){
      LED1(z1,y,1,mycolor);
    }
    for(int x1=1;x1<5;x1++){
      LED1(2,y,x1,mycolor);
      LED1(7,y,x1,mycolor);
    }
    LED1(6,y,5,mycolor);
    LED1(3,y,5,mycolor);
    LED1(4,y,6,mycolor);
    LED1(5,y,6,mycolor);
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
  }
  delay(numstay);
}
///////////////////////////
void H(int mycolor){
  for(int y=0;y<8;y++){
    for(int z1=0;z1<8;z1++){
      LED1(z1,y,2,mycolor);
      LED1(z1,y,6,mycolor);
    }
    for(int x1=2;x1<7;x1++){
      LED1(4,y,x1,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
  
  }
  delay(numstay);
}
/////////////////////////
void C(int mycolor){
  for(int y=0;y<8;y++){
    for(int x1=3;x1<6;x1++){
      LED1(0,y,x1,mycolor);
      LED1(7,y,x1,mycolor);
    }
    for(int x1=2;x1<6;x1++){
      LED1(x1,y,1,mycolor);
    }
    LED1(1,y,2,mycolor);
    LED1(1,y,6,mycolor);
    LED1(6,y,2,mycolor);
    LED1(6,y,6,mycolor);
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
    
  }
  delay(numstay);
}
///////////////////////
void colon(int mycolor){
  for(int y=0;y<8;y++){
    for(int z1=5;z1<7;z1++){
      for(int x1=3;x1<5;x1++){
        LED1(z1,y,x1,mycolor);
      }
    }
    for(int z1=1;z1<3;z1++){
      for(int x1=3;x1<5;x1++){
        LED1(z1,y,x1,mycolor);
      }
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }
   
  }
  delay(numstay);
}
//////////////////////////////
void percentage(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=5;z1<7;z1++){
      for(int y1=1;y1<3;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    for(int z1=1;z1<3;z1++){
      for(int y1=5;y1<7;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    for(int z1=2;z1<4;z1++){
      LED1(z1,3,x,mycolor);
    }
    for(int z1=4;z1<6;z1++){
      LED1(z1,4,x,mycolor);
    }
    LED1(0,1,x,mycolor);
    LED1(1,2,x,mycolor);
    LED1(6,5,x,mycolor);
    LED1(7,6,x,mycolor);

    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  
  }
  delay(numstay);
}
////////////////////
void exclamation(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=3;z1<8;z1++){
      LED1(z1,3,x,mycolor);
      LED1(z1,4,x,mycolor);
    }
    for(int z1=0;z1<2;z1++){
      for(int y1=3;y1<5;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }

  }
  delay(numstay);
}
/////////////////
void degree(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=6;z1<8;z1++){
      for(int y1=0;y1<2;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    for(int z1=1;z1<6;z1++){
      LED1(z1,2,x,mycolor);
    }
    for(int y1=3;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(6,y1,x,mycolor);
    }
    LED1(1,6,x,mycolor);
    LED1(5,6,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }

  }
  delay(numstay);
}
///////////////////
void E(int mycolor){
  for(int y=0;y<8;y++){
    for(int x=1;x<7;x++){
      LED1(0,y,x,mycolor);
      LED1(4,y,x,mycolor);
      LED1(7,y,x,mycolor);
    }
    for(int z=0;z<8;z++){
      LED1(z,y,1,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(y!=7){
      clearCube();
    }

  }
  delay(numstay);
}

//////////Read sensor co2 data and display digital animation based on the data
///////////////////
void co2data(int co2){
  String co2data=String(co2);
  //////number of digits
  int numlength= co2data.length();
  Serial.println(co2data);
  Serial.println(numlength);
  if(co2>1500){
    mycolor =1;
  }else{
    mycolor =15;
  }
  C(mycolor);
  num0(mycolor);
  num2(mycolor);
  colon(mycolor);
  numspeed=30;
//Skip remaining animation
  for(int i=0;i<co2data.length();i++){
    if( option!= 0 && !isco2){
  
      clearCube();
      return;
    }else if(option!= 3 && isco2){
      clearCube();
      
      return;
    }
    ////Numeric animation showing each digit in sequence
    char Digit= co2data.charAt(i);
    Serial.println(Digit);
    switch(Digit){
      case '0':
      num0(mycolor);
      break;
      case '1':
      num1(mycolor);
      break;
      case '2':
      num2(mycolor);
      break;
      case '3':
      num3(mycolor);
      break;
      case '4':
      num4(mycolor);
      break;
      case '5':
      num5(mycolor);
      break;
      case '6':
      num6(mycolor);
      break;
      case '7':
      num7(mycolor);
      break;
      case '8':
      num8(mycolor);
      break;
      case '9':
      num9(mycolor);
      break;
      default:
      break;
    }


    //delay(450);
    
  }
    

  
}
////////////Read sensor temp data and display digital animation based on the data
void Tempdata(float temp){
  String tempdata=String(temp,0);
  int numlength= tempdata.length();
  Serial.println(tempdata);
  Serial.println(numlength);
  /////number animation color
  if(temp>=20){
    mycolor =1;
  }else if(10>temp>=15){
    mycolor =44;
  }else if(5>temp>=10){
    mycolor =45;
  }else if(temp<=5){
    mycolor =29;
  }
  T(mycolor);
  E(mycolor);
  M(mycolor);
  P(mycolor);
  colon(mycolor);
  ////skip
  for(int i=0;i<tempdata.length();i++){
    if( option!= 0 && !istemp){
  
      return;
    }else if(option!= 1 && istemp){
      
      return;
    }

    ///////////Numeric animation showing each digit in sequence
    char Digit= tempdata.charAt(i);
    Serial.println(Digit);
    switch(Digit){
      case '0':
      num0(mycolor);
      break;
      case '1':
      num1(mycolor);
      break;
      case '2':
      num2(mycolor);
      break;
      case '3':
      num3(mycolor);
      break;
      case '4':
      num4(mycolor);
      break;
      case '5':
      num5(mycolor);
      break;
      case '6':
      num6(mycolor);
      break;
      case '7':
      num7(mycolor);
      break;
      case '8':
      num8(mycolor);
      break;
      case '9':
      num9(mycolor);
      break;
      default:
      break;
    }
    
    delay(450);
  }
  delay(500);
  clearCube();
}
//////////////////////////////sun animation
struct Ball {
  int Bx, By, Bz;
  int vx, vy, vz;
} ball;

int calculateRadius() {
  // The radius changes periodically between 1 and 4
  float radius = 1.0 + (sin(radiusChange * 0.2) + 1.0) * 1.5; // Adjust formulas to change amplitude and offset

 
  radius = max(1.0, min(radius, 4.0));
  return static_cast<int>(radius);
}

void initBall() {
  // The center point of the randomly initialized ball is within the internal 4x4x4 matrix
  ball.Bx = random(2, 6); 
  ball.By = random(2, 6); 
  ball.Bz = random(2, 6); 

  // Initialize ball speed
  ball.vx = random(-1, 2); 
  ball.vy = random(-1, 2);
  ball.vz = random(-1, 2);
}

void moveBall(float tump) {
  static unsigned long lastMoveTime = 0; 
  unsigned long currentTime = millis(); 
  unsigned long moveInterval;

  ///moving interval
  if (tump < 5) {
    moveInterval = 1200; 
  } else if(tump>20){
    moveInterval = 50; 
  } else if(5<tump<20){
        moveInterval = 200;
  }
 
  if (currentTime - lastMoveTime >= moveInterval) {
    lastMoveTime = currentTime; 

    
    ball.Bx += ball.vx;
    ball.By += ball.vy;
    ball.Bz += ball.vz;

  
    if (ball.Bx < 2 || ball.Bx > 5) {
      ball.vx *= -1;
      ball.Bx += ball.vx;
    }
    if (ball.By < 2 || ball.By > 5) {
      ball.vy *= -1;
      ball.By += ball.vy;
    }
    if (ball.Bz < 2 || ball.Bz > 5) {
      ball.vz *= -1;
      ball.Bz += ball.vz;
    }

  
    radiusChange++;
  }
}

void drawBall(float temp) {
  int radius = calculateRadius(); 


  if(temp>=20){
    mycolor =1;
  }else if(10>temp>=15){
    mycolor =44;
  }else if(5>temp>=10){
    mycolor =45;
  }else if(temp<=5){
    mycolor =29;
  }


  for (int dx = -radius; dx <= radius; dx++) {
    for (int dy = -radius; dy <= radius; dy++) {
      for (int dz = -radius; dz <= radius; dz++) {
        if (dx*dx + dy*dy + dz*dz <= radius*radius) {
          int drawX = ball.Bx + dx;
          int drawY = ball.By + dy;
          int drawZ = ball.Bz + dz;
          if (drawX >= 0 && drawX < 8 && drawY >= 0 && drawY < 8 && drawZ >= 0 && drawZ < 8) {
            LED(drawX, drawY, drawZ, mycolor);
          }
        }
      }
    }
  }
}


//////////////////////////////////
///////Read sensor humidity data and display digital animation based on the data
void Humdata(float hum){
  String humdata=String(hum,0);
  int numlength= humdata.length();
  Serial.println(humdata);
  Serial.println(numlength);
  H(Blue);
  U(Blue);
  M(Blue);
  colon(Blue);
  ///skip
  for(int i=0;i<humdata.length();i++){
    if( option!= 0 && !ishum){
      
      clearCube();
      return;
    }else if(option!= 2 && ishum){
      clearCube();
      
      return;
    }
    char Digit= humdata.charAt(i);
    Serial.println(Digit);
    switch(Digit){
      case '0':
      num0(Blue);
      break;
      case '1':
      num1(Blue);
      break;
      case '2':
      num2(Blue);
      break;
      case '3':
      num3(Blue);
      break;
      case '4':
      num4(Blue);
      break;
      case '5':
      num5(Blue);
      break;
      case '6':
      num6(Blue);
      break;
      case '7':
      num7(Blue);
      break;
      case '8':
      num8(Blue);
      break;
      case '9':
      num9(Blue);
      break;

    }
    delay(450);
  }
  clearCube();
}


////humidity animation
void rain(float hum){
  int Humlevel;
  String humdata=String(hum,0);
  int numlength= humdata.length();
  char Digit= humdata.charAt(0);
  switch(Digit){
    case '1':
      Humlevel=0;
      break;
    case '2':
      Humlevel=1;
      break;
    case '3':
      Humlevel=2;
      break;
    case '4':
      Humlevel=3;
      break;
    case '5':
      Humlevel=4;
      break;
    case '6':
      Humlevel=5;
      break;
    case '7':
      Humlevel=6;
      break;
    case '8':
      Humlevel=7;
      break;
    case '9':
      Humlevel=8;
      break;
  }
  int num=64;
  int num2=0;

  int x[num], y[num], z[num], zlevel[num], leds = 25,lednum;
  int xold[num], yold[num], zold[num];
  int ranled[leds];

    for(int x1=0;x1<8;x1++){
      for(int y1=0;y1<8;y1++){
        x[num2] = x1;
        y[num2] = y1;
        z[num2] = random(7,26);
        zlevel[num2] = 0;
        num2++;
      }
    }
  for(int i = 0;i<leds;i++){
    ranled[i]=random(0,64);
  }
    int numnnnn=0;
  start = millis();
  while (millis() - start < 30000){
    if( option!= 0 && !ishum){
    
      clearCube();
      return;
    }else if(option!= 2 && ishum){
      clearCube();
     
      return;
    }
    for(lednum = 0;lednum<leds;lednum++){
      // if(z[ranled[lednum]]>0){
        
      // }
      LED1(z[ranled[lednum]], x[ranled[lednum]], y[ranled[lednum]], Blue);
    }
    cube.show();
    for (lednum = 0; lednum < leds; lednum++){
      xold[ranled[lednum]] = x[ranled[lednum]];
      yold[ranled[lednum]] = y[ranled[lednum]];
      zold[ranled[lednum]] = z[ranled[lednum]];
    }
    
    delay(120);
    for (lednum = 0; lednum < leds; lednum++) {
      if(z[ranled[lednum]]!=zlevel[ranled[lednum]]){
        z[ranled[lednum]] = z[ranled[lednum]] -1;
        LED1(zold[ranled[lednum]], xold[ranled[lednum]], yold[ranled[lednum]], Black);
        //Serial.println(z[ranled[lednum]]);
      }
      if (z[ranled[lednum]] == zlevel[ranled[lednum]]) {
        z[ranled[lednum]] = random(7,26);
        LED1(zlevel[ranled[lednum]], x[ranled[lednum]], y[ranled[lednum]], Blue);
        if(zlevel[ranled[lednum]] != Humlevel){
          zlevel[ranled[lednum]]++;
          }
        
        //Serial.println(zlevel[ranled[lednum]]);
        ranled[lednum]=random(0,64);
      }
      
      
    }
    cube.show();
  }
}
//////////////////button
void button111(){
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) { // Check if button is pressed
      option++; // Increment option
      delay(100);
      if (option > 3){
        option = 0;
      }  // Cycle back to the first option after the last one
      delay(100);
      
      // Change color based on the current option and print a message to the Serial Monitor
      switch (option) {
        case 0:
          setColor(85, 85, 255); // red
          Serial.println("Option a selected: Red");
          istemp=false;
          ishum=false;
          isco2=false;
          //looptype2();
          break;
        case 1:
          setColor(255, 0, 0); // green
          Serial.println("Option b selected: Green");
          istemp=true;
          //looptype4();
          break;
        case 2:
          setColor(0, 0, 255); // blue
          Serial.println("Option c selected: Blue");
          ishum=true;
          break;
        case 3:
          setColor(0, 255, 0); // yellow
          Serial.println("Option d selected: Yellow");
          isco2=true;
          break;
      }
    }
    delay(150); // Debounce delay
  }
  lastButtonState = buttonState; // Save the last button state
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

//play three animations in loop
void looptype1(){
  looptype2();
  if(option!=0){
    return;
  }
  looptype3();
  if(option!=0){
    return;
  }
  looptype4();
}


//play sun animation
void looptype2(){
  float temp= airSensor.getHumidity();
  delay(1000);
  Serial.println(temp);
  start = millis();
  while (millis() - start < 30000){

    clearCube(); 
    moveBall(temp); 
    drawBall(temp); 
    cube.show(); 
    delay(100); 
    if( option!= 0 && !istemp){
      clearCube();
    
      return;
    }else if(option!= 1 && istemp){
      clearCube();
    
      return;
    }
  }
  if( option!= 0 && !istemp){
    clearCube();
    
    return;
  }else if(option!= 1 && istemp){
    clearCube();
   
    return;
  }
  Tempdata(temp);
  clearCube();
}

//play rain animation
void looptype3(){
  float hum= airSensor.getHumidity();
  delay(100);
  rain(hum);
  clearCube();
  if( option!= 0 && !ishum){
      
      clearCube();
      return;
    }else if(option!= 2 && ishum){
      clearCube();
    
      return;
    }
  
  delay(500);
  Humdata(hum);
  clearCube();
  delay(500);
}

///play wave animation
void looptype4(){
  int co2= airSensor.getCO2();
  delay(100);
  wave(co2);
  clearCube();
  delay(500);
  if( option!= 0 && !isco2){
      clearCube();
      return;
    }else if(option!= 3 && isco2){
      clearCube();
      
      return;
    }
  co2data(co2);
  clearCube();
  delay(500);
}



























