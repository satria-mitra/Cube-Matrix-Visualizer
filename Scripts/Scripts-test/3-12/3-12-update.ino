#define myLayer 2
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include <SparkFun_SCD30_Arduino_Library.h> // Include the SCD30 library
#define PIN 6
//////////////////////////////////

// #define CPU_HZ 48000000
// #define TIMER_PRESCALER_DIV 1024



/////////////////////////////////////
// int frequencyHz =60;
// void startTimer(int frequencyHz);
// void setTimerFrequency(int frequencyHz);
// void TC3_Handler();
//////////////////////////////////////
volatile uint16_t mymatrix[16][8];  // color of each LED in the matrix
int brightness = 80;  // set brightness here  0-256 with 10 for easy on eyes

Adafruit_NeoMatrix cube = Adafruit_NeoMatrix(8, 8, 8, 1, PIN,
                          NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                          NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                          NEO_RGB            + NEO_KHZ800);



#define White 43
#define Black 0
#define Red 1
#define Orange 4
#define Yellow 9
#define Green 15
#define Aqua 20
#define Blue 29
#define Purple 34
uint32_t mycolor;
const byte color [44][3] PROGMEM = {  // the color pallette table
  {0, 0, 0}, //Black
  {7, 0, 0}, {7, 1, 0}, {6, 1, 0}, {6, 2, 0}, {5, 2, 0}, {5, 3, 0}, {4, 3, 0}, {4, 4, 0}, {3, 4, 0}, {3, 5, 0}, {2, 5, 0}, {2, 6, 0}, {1, 6, 0}, {1, 7, 0}, // Red to Green
  {0, 7, 0}, {0, 7, 1}, {0, 6, 1}, {0, 6, 2}, {0, 5, 2}, {0, 5, 3}, {0, 4, 3}, {0, 4, 4}, {0, 3, 4}, {0, 3, 5}, {0, 2, 5}, {0, 2, 6}, {0, 1, 6}, {0, 1, 7}, // Green to Blue
  {0, 0, 7}, {1, 0, 7}, {1, 0, 6}, {2, 0, 6}, {2, 0, 5}, {3, 0, 5}, {3, 0, 4}, {4, 0, 4}, {4, 0, 3}, {5, 0, 3}, {5, 0, 2}, {6, 0, 2}, {6, 0, 1}, {7, 0, 1}, // Blue to Red
  {7, 7, 7},  // White
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
int radiusChange = 0; // 用于控制半径变化的变量
////////////////////
bool istump=false;
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
  /////////////////////////
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  pinMode(buttonPin, INPUT_PULLUP); // Set the button as an input
  attachInterrupt(digitalPinToInterrupt(buttonPin),button111,CHANGE);
////////////////////////////


  // startTimer(10);

  

  
  delay(2000);
}


int Tup= 26;
void loop() {
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

  while (millis() - start < 10000) {
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
  
if( option!= 0 && !isco2){
      
      clearCube();
      return;
    }else if(option!= 3 && isco2){
      clearCube();
      //isco2=false;
      return;
    }

}




void num0(int mycolor){
  int z,y,x;
  for(x=7;x>-1;x--){
    for(y=2;y<6;y++){
      LED1(0, y, x, mycolor);
      LED1(7, y, x, mycolor);
    }
    for(z=1;z<7;z++){
      LED1(z, 1, x, mycolor);
      LED1(z, 6, x, mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    //cube.show();
    
  }
  
  delay(numstay);
}
//////////////////////////////
void num1(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=1;y1<7;y1++){
      LED1(0, y1, x, mycolor);
    }
    for(int z1=1;z1<8;z1++){
      LED1(z1, 3, x, mycolor);
      LED1(z1, 4, x, mycolor);
    }
    LED1(5, 1, x, mycolor);
    LED1(6, 2, x, mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  delay(numstay);
}
///////////////////////////

void num2(int mycolor){
  int z2[4]={1,2,3,4};
  int y2[4]={2,3,4,5};
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<7;y1++){
      LED1(0, y1, x, mycolor);
    }
    for(int i=0;i<4;i++){
      LED1(z2[i],y2[i],x,mycolor);
    }
    for(int z3=5;z3<7;z3++){
      LED1(z3,2,x,mycolor);
      LED1(z3,6,x,mycolor);
    }
    for(int y2=3;y2<6;y2++){
      LED1(7,y2,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }

  }
  delay(numstay);
}

////////////////////////////

void num3(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=3;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int z1=1;z1<3;z1++){
      LED1(z1,6,x,mycolor);
    }
    for(int z2=5;z2<7;z2++){
      LED1(z2,6,x,mycolor);
    }
    for(int z3=3;z3<5;z3++){
      LED1(z3,5,x,mycolor);
    }
    LED1(1,2,x,mycolor);
    LED1(6,2,x,mycolor);
    LED1(3,4,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  
  }
  delay(numstay);
}

//////////////////////////////////////////////

void num4(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=0;z1<8;z1++){
      LED1(z1,4,x,mycolor);
    }
    for(int z2=3;z2<8;z2++){
      LED1(z2,1,x,mycolor);
    }
    for(int y1=1;y1<7;y1++){
      LED1(3,y1,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }

  }
  delay(numstay);
}

///////////////////////////////////

void num5(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<7;y1++){
      LED1(7,y1,x,mycolor);
    }
    for(int y2=2;y2<6;y2++){
      LED1(4,y2,x,mycolor);
      LED1(0,y2,x,mycolor);
    }
    for(int z1=5;z1<7;z1++){
      LED1(z1,2,x,mycolor);
    }
    for(int z2=1;z2<4;z2++){
      LED1(z2,6,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    
  }
  delay(numstay);

}
////////////////////

void num6(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<7;y1++){
      LED1(0,y1,x,mycolor);
      LED1(4,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int z1=1;z1<4;z1++){
      LED1(z1,2,x,mycolor);
    }
    for(int z2=1;z2<7;z2++){
      LED1(z2,2,x,mycolor);
    }
    for(int z2=0;z2<5;z2++){
      LED1(z2,6,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
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
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<7;y1++){
      LED1(7,y1,x,mycolor);
    }
    for(int y2=3;y2<7;y2++){
      for(int z=z1[y2];z<z2[y2];z++){
        LED1(z,y2,x,mycolor);
      }
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    //cube.show();
  }
  delay(numstay);
}
/////////////////
void num8(int mycolor){
  int z[6]={6,6,5,5,3,3};
  int y[6]={6,1,5,2,5,2,};
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int z1=1;z1<3;z1++){
      LED1(z1,1,x,mycolor);
      LED1(z1,6,x,mycolor);
    }
    for(int y2=3;y2<5;y2++){
      LED1(4,y2,x,mycolor);
    }
    for(int i=0;i<6;i++){
      LED1(z[i],y[i],x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  //cube.show();
  delay(numstay);
}
/////////////
void num9(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=3;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(3,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int z1=1;z1<7;z1++){
      LED1(z1,6,x,mycolor);
    }
    for(int z2=4;z2<7;z2++){
      LED1(z2,2,x,mycolor);
    }
    LED1(1,2,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  //cube.show();
  delay(numstay);
}
/////////////
void T(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=1;y1<8;y1++){
      LED1(7,y1,x,mycolor);
    }
    for(int z1=0;z1<7;z1++){
      LED1(z1,4,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    
  }
  delay(numstay);
}
///////////////////
void U(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=2;y1<6;y1++){
      LED1(0,y1,x,mycolor);
    }
    for(int z1=1;z1<8;z1++){
      LED1(z1,1,x,mycolor);
      LED1(z1,6,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  delay(numstay);
}
///////////////////////
void M(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=0;z1<8;z1++){
      LED1(z1,1,x,mycolor);
      LED1(z1,7,x,mycolor);
    }
    for(int z2=3;z2<5;z2++){
      LED1(z2,4,x,Red);
    }
    LED1(6,2,x,mycolor);
    LED1(6,6,x,mycolor);
    LED1(5,3,x,mycolor);
    LED1(5,5,x,mycolor);

    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    
  }
  delay(numstay);
}
///////////////////////////////
void P(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=0;z1<8;z1++){
      LED1(z1,1,x,mycolor);
    }
    for(int y1=1;y1<5;y1++){
      LED1(2,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    LED1(6,5,x,mycolor);
    LED1(3,5,x,mycolor);
    LED1(4,6,x,mycolor);
    LED1(5,6,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  }
  delay(numstay);
}
///////////////////////////
void H(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=0;z1<8;z1++){
      LED1(z1,2,x,mycolor);
      LED1(z1,6,x,mycolor);
    }
    for(int y1=2;y1<7;y1++){
      LED1(4,y1,x,mycolor);
    }
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
  
  }
  delay(numstay);
}
/////////////////////////
void C(int mycolor){
  for(int x=7;x>-1;x--){
    for(int y1=3;y1<6;y1++){
      LED1(0,y1,x,mycolor);
      LED1(7,y1,x,mycolor);
    }
    for(int x1=2;x1<6;x1++){
      LED1(x1,1,x,mycolor);
    }
    LED1(1,2,x,mycolor);
    LED1(1,6,x,mycolor);
    LED1(6,2,x,mycolor);
    LED1(6,6,x,mycolor);
    cube.show();
    delay(numspeed);
    if(x!=0){
      clearCube();
    }
    
  }
  delay(numstay);
}
///////////////////////
void colon(int mycolor){
  for(int x=7;x>-1;x--){
    for(int z1=5;z1<7;z1++){
      for(int y1=3;y1<5;y1++){
        LED1(z1,y1,x,mycolor);
      }
    }
    for(int z1=1;z1<3;z1++){
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
void co2data(int co2){
  String co2data=String(co2);
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

  for(int i=0;i<co2data.length();i++){
    if( option!= 0 && !isco2){
  
      clearCube();
      return;
    }else if(option!= 3 && isco2){
      clearCube();
      
      return;
    }
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

void Tumpdata(float tump){
  String tumpdata=String(tump,0);
  int numlength= tumpdata.length();
  Serial.println(tumpdata);
  Serial.println(numlength);
  T(Red);
  M(Red);
  P(Red);
  for(int i=0;i<tumpdata.length();i++){
    if( option!= 0 && !istump){
  
      return;
    }else if(option!= 1 && istump){
      
      return;
    }
    char Digit= tumpdata.charAt(i);
    Serial.println(Digit);
    switch(Digit){
      case '0':
      num0(Red);
      break;
      case '1':
      num1(Red);
      break;
      case '2':
      num2(Red);
      break;
      case '3':
      num3(Red);
      break;
      case '4':
      num4(Red);
      break;
      case '5':
      num5(Red);
      break;
      case '6':
      num6(Red);
      break;
      case '7':
      num7(Red);
      break;
      case '8':
      num8(Red);
      break;
      case '9':
      num9(Red);
      break;
      default:
      break;
    }
    
    delay(450);
  }
  delay(500);
  clearCube();
}
//////////////////////////////sun
struct Ball {
  int Bx, By, Bz;
  int vx, vy, vz;
} ball;

int calculateRadius() {
  // 半径周期性地在1到4之间变化
  float radius = 1.0 + (sin(radiusChange * 0.2) + 1.0) * 1.5; // 调整公式以改变振幅和偏移

  // 确保半径至少为1，最大为4
  radius = max(1.0, min(radius, 4.0));
  return static_cast<int>(radius);
}

void initBall() {
  // 随机初始化小球的中心点在内部4x4x4矩阵内
  ball.Bx = random(2, 6); // 保证x在2到5之间
  ball.By = random(2, 6); // 保证y在2到5之间
  ball.Bz = random(2, 6); // 保证z在2到5之间

  // 初始化小球速度，确保小球移动
  ball.vx = random(-1, 2); // -1, 0, 或 1
  ball.vy = random(-1, 2);
  ball.vz = random(-1, 2);
}

void moveBall(float tump) {
  static unsigned long lastMoveTime = 0; // 上次移动时间
  unsigned long currentTime = millis(); // 当前时间
  unsigned long moveInterval;

  //根据T值设定移动间隔
  if (tump < 5) {
    moveInterval = 1200; // T大于等于25时，每秒移动3次
  } else if(tump>20){
    moveInterval = 50; // T小于25时，每1.5秒移动一次
  } else if(5<tump<20){
        moveInterval = 200;
  }
  // 检查是否达到移动间隔
  if (currentTime - lastMoveTime >= moveInterval) {
    lastMoveTime = currentTime; // 更新上次移动时间

    // 根据当前速度向量更新位置
    ball.Bx += ball.vx;
    ball.By += ball.vy;
    ball.Bz += ball.vz;

    // 边界检查与处理
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

    // 在小球移动后更新半径变化状态，以产生“呼吸”效果
    radiusChange++;
  }
}

void drawBall(float tump) {
  int radius = calculateRadius(); // 使用新的半径计算逻辑
  //uint32_t ballColor = calculateColor(tump); // 可以保留T来控制颜色，或者自定义颜色控制逻辑
  if (tump < 5) {
    mycolor=29;
  } else if(tump>20){
    mycolor=1; 
  } else if(5<tump<20){
    mycolor=4;
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

void Humdata(float hum){
  String humdata=String(hum,0);
  int numlength= humdata.length();
  Serial.println(humdata);
  Serial.println(numlength);
  H(Red);
  U(Red);
  M(Red);
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
      num0(Red);
      break;
      case '1':
      num1(Red);
      break;
      case '2':
      num2(Red);
      break;
      case '3':
      num3(Red);
      break;
      case '4':
      num4(Red);
      break;
      case '5':
      num5(Red);
      break;
      case '6':
      num6(Red);
      break;
      case '7':
      num7(Red);
      break;
      case '8':
      num8(Red);
      break;
      case '9':
      num9(Red);
      break;

    }
    delay(450);
  }
  clearCube();
}



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
  while (millis() - start < 10000){
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
          setColor(255, 0, 0); // red
          Serial.println("Option a selected: Red");
          istump=false;
          ishum=false;
          isco2=false;
          //looptype2();
          break;
        case 1:
          setColor(0, 255, 0); // green
          Serial.println("Option b selected: Green");
          istump=true;
          //looptype4();
          break;
        case 2:
          setColor(0, 0, 255); // blue
          Serial.println("Option c selected: Blue");
          ishum=true;
          break;
        case 3:
          setColor(252, 226, 0); // yellow
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



void looptype2(){
  float tump= airSensor.getHumidity();
  delay(1000);
  Serial.println(tump);
  start = millis();
  while (millis() - start < 15000){
    clearCube(); // 清空LED矩阵
    moveBall(tump); // 移动小球
    drawBall(tump); // 绘制小球
    cube.show(); // 显示小球
    delay(100); // 控制动画速度
    if( option!= 0 && !istump){
      clearCube();
    
      return;
    }else if(option!= 1 && istump){
      clearCube();
    
      return;
    }
  }
  if( option!= 0 && !istump){
    clearCube();
    
    return;
  }else if(option!= 1 && istump){
    clearCube();
   
    return;
  }
  Tumpdata(tump);
  clearCube();
}

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



























