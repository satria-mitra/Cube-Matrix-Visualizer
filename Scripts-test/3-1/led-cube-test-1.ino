#define myLayer 2
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SparkFun_SCD30_Arduino_Library.h> // Include the SCD30 library
//#include <Timer.h>
#define PIN 6
//////////////////////////////////
#define CPU_HZ 48000000
#define TIMER_PRESCALER_DIV 1024
int frequencyHz =60;
void startTimer(int frequencyHz);
void setTimerFrequency(int frequencyHz);
void TC3_Handler();
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


startTimer(10);

  

  
  delay(2000);
}



void loop() {
  //int co2= airSensor.getCO2();
  //Serial.println(co2);
  float tump= airSensor.getTemperature();
  float hum= airSensor.getHumidity();
  //Serial.println(tump);
  //Serial.print(hum);


  num1();
  delay(500);
  //sinwaveTwo(tump,hum);
//bouncyvTwo(co2);

  //clearCube();
  //delay(1000);
    //clearCube();
}

////////////////////////////////////////////////////
void setTimerFrequency(int frequencyHz) {
  int compareValue = (CPU_HZ / (TIMER_PRESCALER_DIV * frequencyHz)) - 1;
  TcCount16* TC = (TcCount16*) TC3;
  // Make sure the count is in a proportional position to where it was
  // to prevent any jitter or disconnect when changing the compare value.
  TC->COUNT.reg = map(TC->COUNT.reg, 0, TC->CC[0].reg, 0, compareValue);
  TC->CC[0].reg = compareValue;
  Serial.println(TC->COUNT.reg);
  Serial.println(TC->CC[0].reg);
  while (TC->STATUS.bit.SYNCBUSY == 1);
}

void startTimer(int frequencyHz) {
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync

  TcCount16* TC = (TcCount16*) TC3;

  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  // Use the 16-bit timer
  TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  // Use match mode so that the timer counter resets when the count matches the compare register
  TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  // Set prescaler to 1024
  TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  setTimerFrequency(frequencyHz);

  // Enable the compare interrupt
  TC->INTENSET.reg = 0;
  TC->INTENSET.bit.MC0 = 1;

  NVIC_EnableIRQ(TC3_IRQn);

  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
}

void TC3_Handler() {
  TcCount16* TC = (TcCount16*) TC3;
  // If this interrupt is due to the compare register matching the timer count
  // we toggle the LED.
  if (TC->INTFLAG.bit.MC0 == 1) {
    TC->INTFLAG.bit.MC0 = 1;
    cube.show();
  }
}
////////////////////////////////////////////////////




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
void sinwaveTwo(float tump,float hum) {
  int wavespeed;
  if(tump<20){
    mycolor =29;
  }else{
    mycolor =1;
  }

  if(hum<20){
    wavespeed=120;
  }else{
    wavespeed=30;
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

  while (millis() - start < 15000) {
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
////////////////////////////////////////////
    // for (addr = 0; addr < 8; addr++) {
    //   LED1(sinewavearrayOLD[addr], addr, 0, White);
    //   LED1(sinewavearrayOLD[addr], 0, addr,White);
    //   LED1(sinewavearrayOLD[addr], subT - addr, 7, White);
    //   LED1(sinewavearrayOLD[addr], 7, subT - addr, White);
    //   LED2(sinewavearray[addr], addr, 0, rr, gg, bb);
    //   LED2(sinewavearray[addr], 0, addr, rr, gg, bb);
    //   LED2(sinewavearray[addr], subT - addr, 7, rr, gg, bb);
    //   LED2(sinewavearray[addr], 7, subT - addr, rr, gg, bb);
    // }

    // for (addr = 1; addr < 7; addr++) {
    //   LED1(sinewavearrayOLD[addr + multi * 1], addr, 1, White);
    //   LED1(sinewavearrayOLD[addr + multi * 1], 1, addr, White);
    //   LED1(sinewavearrayOLD[addr + multi * 1], subT - addr, 6, White);
    //   LED1(sinewavearrayOLD[addr + multi * 1], 6, subT - addr, White);
    //   LED2(sinewavearray[addr + multi * 1], addr, 1, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 1], 1, addr, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 1], subT - addr, 6, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 1], 6, subT - addr, rr, gg, bb);
    // }

    // for (addr = 2; addr < 6; addr++) {
    //   LED1(sinewavearrayOLD[addr + multi * 2], addr, 2, White);
    //   LED1(sinewavearrayOLD[addr + multi * 2], 2, addr, White);
    //   LED1(sinewavearrayOLD[addr + multi * 2], subT - addr, 5,White);
    //   LED1(sinewavearrayOLD[addr + multi * 2], 5, subT - addr, White);
    //   LED2(sinewavearray[addr + multi * 2], addr, 2, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 2], 2, addr, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 2], subT - addr, 5, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 2], 5, subT - addr, rr, gg, bb);
    // }
    // for (addr = 3; addr < 5; addr++) {
    //   LED1(sinewavearrayOLD[addr + multi * 3], addr, 3, White);
    //   LED1(sinewavearrayOLD[addr + multi * 3], 3, addr, White);
    //   LED1(sinewavearrayOLD[addr + multi * 3], subT - addr, 4, White);
    //   LED1(sinewavearrayOLD[addr + multi * 3], 4, subT - addr, White);
    //   LED2(sinewavearray[addr + multi * 3], addr, 3, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 3], 3, addr, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 3], subT - addr, 4, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 3], 4, subT - addr, rr, gg, bb);
    // }
//////////////////////////////////////////////
    // for (addr = 0; addr < 8; addr++) {
    //   LED1(sinewavearrayOLD[addr], addr, 0, Black);
    //   LED1(sinewavearrayOLD[addr], 0, addr,Black);
    //   LED1(sinewavearrayOLD[addr], subT - addr, 7, Black);
    //   LED1(sinewavearrayOLD[addr], 7, subT - addr, Black);
    //   LED2(sinewavearray[addr], addr, 0, rr, gg, bb);
    //   LED2(sinewavearray[addr], 0, addr, rr, gg, bb);
    //   LED2(sinewavearray[addr], subT - addr, 7, rr, gg, bb);
    //   LED2(sinewavearray[addr], 7, subT - addr, rr, gg, bb);
    // }

    // for (addr = 1; addr < 7; addr++) {
    //   LED1(sinewavearrayOLD[addr + multi * 1], addr, 1, Black);
    //   LED1(sinewavearrayOLD[addr + multi * 1], 1, addr, Black);
    //   LED1(sinewavearrayOLD[addr + multi * 1], subT - addr, 6, Black);
    //   LED1(sinewavearrayOLD[addr + multi * 1], 6, subT - addr, Black);
    //   LED2(sinewavearray[addr + multi * 1], addr, 1, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 1], 1, addr, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 1], subT - addr, 6, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 1], 6, subT - addr, rr, gg, bb);
    // }

    // for (addr = 2; addr < 6; addr++) {
    //   LED1(sinewavearrayOLD[addr + multi * 2], addr, 2, Black);
    //   LED1(sinewavearrayOLD[addr + multi * 2], 2, addr, Black);
    //   LED1(sinewavearrayOLD[addr + multi * 2], subT - addr, 5,Black);
    //   LED1(sinewavearrayOLD[addr + multi * 2], 5, subT - addr, Black);
    //   LED2(sinewavearray[addr + multi * 2], addr, 2, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 2], 2, addr, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 2], subT - addr, 5, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 2], 5, subT - addr, rr, gg, bb);
    // }
    // for (addr = 3; addr < 5; addr++) {
    //   LED1(sinewavearrayOLD[addr + multi * 3], addr, 3, Black);
    //   LED1(sinewavearrayOLD[addr + multi * 3], 3, addr, Black);
    //   LED1(sinewavearrayOLD[addr + multi * 3], subT - addr, 4, Black);
    //   LED1(sinewavearrayOLD[addr + multi * 3], 4, subT - addr, Black);
    //   LED2(sinewavearray[addr + multi * 3], addr, 3, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 3], 3, addr, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 3], subT - addr, 4, rr, gg, bb);
    //   LED2(sinewavearray[addr + multi * 3], 4, subT - addr, rr, gg, bb);
    // }
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
    }
    /////////////////////////////////////////////////////
    for (addr = 0; addr < 8; addr++)
      sinewavearrayOLD[addr] = sinewavearray[addr];
    delay(wavespeed);



  }


}
/////////////////////////////
// void harlem_shake() {



//   int greenx = random(1, 7), greeny = random(1, 7), bluex = random(1, 7), bluey = random(1, 7), redx = random(1, 7), redy = random(1, 7);
//   int greenmult = 1, bluemult = 1, redmult = 1;
//   int greenmulty = 1, bluemulty = 1, redmulty = 1;
//   int oredx, oredy, obluex, obluey, ogreenx, ogreeny, cb1 = 15, cb2 = 0, cr1 = 15, cr2 = 0, cg1 = 15, cg2 = 0;
//   int time_counter = 10, timemult = 2;
//   int m;
//   int c1 = 1, c2 = 1, c3 = 1, xmult = 1, ymult = 1, zmult = 1, x = 4, y = 4, z = 4, color_select, xo, yo, zo;
//   int c21 = 1, c22 = 1, c23 = 1, x2mult = 1, y2mult = 1, z2mult = 1, x2 = 2, y2 = 2, z2 = 2, color_select2, x2o, y2o, z2o;

//   int counter, i, j, k;
//   // for (counter = 0; counter < 150; counter++) {
//   //   for (i = 0; i < 8; i++) {
//   //     LED1(i, oredx, oredx, Black);
//   //   }
//   //   for (i = 0; i < 8; i++) {
//   //     LED2(i, redx, redx, 15, 0, 0);
//   //   }

//   //   oredx = redx;
//   //   oredy = redy;

//   //   for (i = 100; i > time_counter; i--)
//   //     delay(1);

//   //   time_counter = time_counter + timemult;
//   //   if (time_counter > 100 || time_counter < 10)
//   //     timemult = timemult * -1;


//   //   if (redy > 6 || redy < 1) {
//   //     redmulty = redmulty * -1;

//   //   }

//   //   if (redx > 6 || redx < 1) {
//   //     redmult = redmult * -1;

//   //     cr1 = random(16);
//   //     cr2 = random(16);
//   //   }

//   //   redy = redy + redmulty;
//   //   redx = redx + redmult;
//   // }


//   // for (counter = 0; counter < 85; counter++) {
//   //   for (i = 0; i < 8; i++) {
//   //     LED1(i, oredx, oredx, Black);
//   //     LED1(ogreenx, i, ogreeny, Black);
//   //   }
//   //   for (i = 0; i < 8; i++) {
//   //     LED2(i, redx, redx, 15, 0, 0);
//   //     LED2(greenx, i, greeny, 0, 15, 0);
//   //   }
//   //   ogreenx = greenx;
//   //   ogreeny = greeny;
//   //   oredx = redx;
//   //   oredy = redy;

//   //   for (i = 100; i > time_counter; i--)
//   //     delay(1);

//   //   time_counter = time_counter + timemult;
//   //   if (time_counter > 100 || time_counter < 10)
//   //     timemult = timemult * -1;


//   //   if (greeny > 6 || greeny < 1)
//   //     greenmulty = greenmulty * -1;

//   //   if (redy > 6 || redy < 1) {
//   //     redmulty = redmulty * -1;

//   //   }


//   //   if (greenx > 6 || greenx < 1) {
//   //     greenmult = greenmult * -1;
//   //     greeny = greeny + greenmulty;
//   //     cg1 = random(16);
//   //     cg2 = random(16);
//   //   }

//   //   if (redx > 6 || redx < 1) {
//   //     redmult = redmult * -1;

//   //     cr1 = random(16);
//   //     cr2 = random(16);
//   //   }
//   //   greenx = greenx + greenmult;

//   //   redy = redy + redmulty;
//   //   redx = redx + redmult;
//   // }


//   // for (counter = 0; counter < 85; counter++) {
//   //   for (i = 0; i < 8; i++) {
//   //     LED1(i, oredx, oredx, Black);
//   //     LED1(obluey, obluex, i, Black);
//   //     LED(ogreenx, i, ogreeny, Black);
//   //   }
//   //   for (i = 0; i < 8; i++) {
//   //     LED2(i, redx, redx, 15, 0, 0);
//   //     LED2(bluey, bluex, i, 0, 0, 15);
//   //     LED2(greenx, i, greeny, 0, 15, 0);
//   //   }
//   //   ogreenx = greenx;
//   //   ogreeny = greeny;
//   //   obluex = bluex;
//   //   obluey = bluey;
//   //   oredx = redx;
//   //   oredy = redy;

//   //   for (i = 100; i > time_counter; i--)
//   //     delay(1);

//   //   time_counter = time_counter + timemult;
//   //   if (time_counter > 100 || time_counter < 10)
//   //     timemult = timemult * -1;


//   //   if (greeny > 6 || greeny < 1)
//   //     greenmulty = greenmulty * -1;

//   //   if (bluey > 6 || bluey < 1)
//   //     bluemulty = bluemulty * -1;

//   //   if (redy > 6 || redy < 1) {
//   //     redmulty = redmulty * -1;

//   //   }


//   //   if (greenx > 6 || greenx < 1) {
//   //     greenmult = greenmult * -1;
//   //     greeny = greeny + greenmulty;
//   //     cg1 = random(16);
//   //     cg2 = random(16);
//   //   }
//   //   if (bluex > 6 || bluex < 1) {
//   //     bluemult = bluemult * -1;
//   //     bluey = bluey + bluemulty;
//   //     cb1 = random(16);
//   //     cb2 = random(16);
//   //   }
//   //   if (redx > 6 || redx < 1) {
//   //     redmult = redmult * -1;

//   //     cr1 = random(16);
//   //     cr2 = random(16);
//   //   }
//   //   greenx = greenx + greenmult;
//   //   bluex = bluex + bluemult;
//   //   redy = redy + redmulty;
//   //   redx = redx + redmult;
//   // }



//   // for (counter = 0; counter < 3; counter++) {
//   //   for (i = 0; i < 8; i++)
//   //     for (j = 0; j < 8; j++)
//   //       for (k = 0; k < 8; k++)
//   //         LED2(i, j, k, 15, 15, 15);
//   //   delay(50);
//   //   for (i = 0; i < 8; i++)
//   //     for (j = 0; j < 8; j++)
//   //       for (k = 0; k < 8; k++)
//   //         LED1(i, j, k, Black);
//   //   delay(50);
//   // }

//   // for (m = 0; m < 1; m++) {


//   //   for (i = 0; i < 8; i++)
//   //     for (j = 0; j < 8; j++)
//   //       for (k = 0; k < 8; k++)
//   //         LED2(i, j, k, 0, random(16), random(16));

//   //   for (i = 7; i >= 0; i--)
//   //     for (j = 0; j < 8; j++)
//   //       for (k = 0; k < 8; k++)
//   //         LED2(i, j, k, random(16), 0, random(16));

//   //   for (i = 0; i < 8; i++)
//   //     for (j = 0; j < 8; j++)
//   //       for (k = 0; k < 8; k++)
//   //         LED2(i, j, k, random(16), random(16), 0);

//   //   for (i = 7; i >= 0; i--)
//   //     for (j = 0; j < 8; j++)
//   //       for (k = 0; k < 8; k++)
//   //         LED2(i, j, k, random(16), 0, random(16));
//   // }

//   // clearCube();


//   // for (m = 0; m < 3; m++) {
//   //   for (k = 0; k < 200; k++) {
//   //     LED2(random(8), random(8), random(8), random(16), random(16), 0);
//   //     LED2(random(8), random(8), random(8), random(16), 0 , random(16));
//   //     LED2(random(8), random(8), random(8), 0, random(16), random(16));




//   //   }
//   //   for (k = 0; k < 200; k++) {
//   //     LED1(random(8), random(8), random(8), Black);
//   //   }

//   // }




//   // clearCube();


//   color_select = random(0, 3);
//   if (color_select == 0) {
//     c1 = 0;
//     c2 = random(0, 16);
//     c3 = random(0, 16);
//   }
//   if (color_select == 1) {
//     c1 = random(0, 16);
//     c2 = 0;
//     c3 = random(0, 16);
//   }
//   if (color_select == 2) {
//     c1 = random(0, 16);
//     c2 = random(0, 16);
//     c3 = 0;
//   }


//   color_select2 = random(0, 3);
//   if (color_select2 == 0) {
//     c21 = 0;
//     c22 = random(0, 16);
//     c23 = random(0, 16);
//   }
//   if (color_select2 == 1) {
//     c21 = random(0, 16);
//     c22 = 0;
//     c23 = random(0, 16);
//   }
//   if (color_select2 == 2) {
//     c21 = random(0, 16);
//     c22 = random(0, 16);
//     c23 = 0;
//   }

//   for (counter = 0; counter < 200; counter++) {

//     LED1(xo, yo, zo, Black);
//     LED1(xo + 1, yo, zo, Black);
//     LED1(xo + 2, yo, zo, Black);
//     LED1(xo - 1, yo, zo, Black);
//     LED1(xo - 2, yo, zo, Black);
//     LED1(xo, yo + 1, zo, Black);
//     LED1(xo, yo - 1, zo, Black);
//     LED1(xo, yo + 2, zo, Black);
//     LED1(xo, yo - 2, zo, Black);
//     LED1(xo, yo, zo - 1, Black);
//     LED1(xo, yo, zo + 1, Black);
//     LED1(xo, yo, zo - 2, Black);
//     LED1(xo, yo, zo + 2, Black);

//     LED1(x2o, y2o, z2o, Black);
//     LED1(x2o + 1, y2o, z2o, Black);
//     LED1(x2o + 2, y2o, z2o, Black);
//     LED1(x2o - 1, y2o, z2o, Black);
//     LED1(x2o - 2, y2o, z2o, Black);
//     LED1(x2o, y2o + 1, z2o, Black);
//     LED1(x2o, y2o - 1, z2o, Black);
//     LED1(x2o, y2o + 2, z2o, Black);
//     LED1(x2o, y2o - 2, z2o, Black);
//     LED1(x2o, y2o, z2o - 1, Black);
//     LED1(x2o, y2o, z2o + 1, Black);
//     LED1(x2o, y2o, z2o - 2, Black);
//     LED1(x2o, y2o, z2o + 2, Black);

//     LED1(xo + 1, yo + 1, zo, Black);
//     LED1(xo - 1, yo + 1, zo, Black);
//     LED1(xo - 1, yo - 1, zo, Black);
//     LED1(xo + 1, yo + 1, zo + 1, Black);
//     LED1(xo + 1, yo - 1, zo + 1, Black);
//     LED1(xo - 1, yo + 1, zo + 1, Black);
//     LED1(xo - 1, yo - 1, zo + 1, Black);
//     LED1(xo + 1, yo + 1, zo - 1, Black);
//     LED1(xo + 1, yo - 1, zo - 1, Black);
//     LED1(xo - 1, yo + 1, zo - 1, Black);
//     LED1(xo - 1, yo - 1, zo - 1, Black);

//     LED1(x2o + 1, y2o + 1, z2o, Black);
//     LED1(x2o + 1, y2o - 1, z2o, Black);
//     LED1(x2o - 1, y2o + 1, z2o, Black);
//     LED1(x2o - 1, y2o - 1, z2o, Black);
//     LED1(x2o + 1, y2o + 1, z2o + 1, Black);
//     LED1(x2o + 1, y2o - 1, z2o + 1, Black);
//     LED1(x2o - 1, y2o + 1, z2o + 1, Black);
//     LED1(x2o - 1, y2o - 1, z2o + 1, Black);
//     LED1(x2o + 1, y2o + 1, z2o - 1, Black);
//     LED1(x2o + 1, y2o - 1, z2o - 1, Black);
//     LED1(x2o - 1, y2o + 1, z2o - 1, Black);
//     LED1(x2o - 1, y2o - 1, z2o - 1, Black);

//     LED2(x, y, z, c1, c2, c3);
//     LED2(x, y, z - 1, c1, c2, c3);
//     LED2(x, y, z + 1, c1, c2, c3);
//     LED2(x, y, z - 2, c1, c2, c3);
//     LED2(x, y, z + 2, c1, c2, c3);
//     LED2(x + 1, y, z, c1, c2, c3);
//     LED2(x - 1, y, z, c1, c2, c3);
//     LED2(x, y + 1, z, c1, c2, c3);
//     LED2(x, y - 1, z, c1, c2, c3);
//     LED2(x + 2, y, z, c1, c2, c3);
//     LED2(x - 2, y, z, c1, c2, c3);
//     LED2(x, y + 2, z, c1, c2, c3);
//     LED2(x, y - 2, z, c1, c2, c3);
//     LED2(x + 1, y + 1, z, c1, c2, c3);
//     LED2(x + 1, y - 1, z, c1, c2, c3);
//     LED2(x - 1, y + 1, z, c1, c2, c3);
//     LED2(x - 1, y - 1, z, c1, c2, c3);
//     LED2(x + 1, y + 1, z + 1, c1, c2, c3);
//     LED2(x + 1, y - 1, z + 1, c1, c2, c3);
//     LED2(x - 1, y + 1, z + 1, c1, c2, c3);
//     LED2(x - 1, y - 1, z + 1, c1, c2, c3);
//     LED2(x + 1, y + 1, z - 1, c1, c2, c3);
//     LED2(x + 1, y - 1, z - 1, c1, c2, c3);
//     LED2(x - 1, y + 1, z - 1, c1, c2, c3);
//     LED2(x - 1, y - 1, z - 1, c1, c2, c3);

//     LED2(x2, y2, z2, c21, c22, c23);
//     LED2(x2, y2, z2 - 1, c21, c22, c23);
//     LED2(x2, y2, z2 + 1, c21, c22, c23);
//     LED2(x2, y2, z2 - 2, c21, c22, c23);
//     LED2(x2, y2, z2 + 2, c21, c22, c23);
//     LED2(x2 + 1, y2, z2, c21, c22, c23);
//     LED2(x2 - 1, y2, z2, c21, c22, c23);
//     LED2(x2, y2 + 1, z2, c21, c22, c23);
//     LED2(x2, y2 - 1, z2, c21, c22, c23);
//     LED2(x2 + 2, y2, z2, c21, c22, c23);
//     LED2(x2 - 2, y2, z2, c21, c22, c23);
//     LED2(x2, y2 + 2, z2, c21, c22, c23);
//     LED2(x2, y2 - 2, z2, c21, c22, c23);
//     LED2(x2 + 1, y2 + 1, z2, c21, c22, c23);
//     LED2(x2 + 1, y2 - 1, z2, c21, c22, c23);
//     LED2(x2 - 1, y2 + 1, z2, c21, c22, c23);
//     LED2(x2 - 1, y2 - 1, z2, c21, c22, c23);
//     LED2(x2 + 1, y2 + 1, z2 + 1, c21, c22, c23);
//     LED2(x2 + 1, y2 - 1, z2 + 1, c21, c22, c23);
//     LED2(x2 - 1, y2 + 1, z2 + 1, c21, c22, c23);
//     LED2(x2 - 1, y2 - 1, z2 + 1, c21, c22, c23);
//     LED2(x2 + 1, y2 + 1, z2 - 1, c21, c22, c23);
//     LED2(x2 + 1, y2 - 1, z2 - 1, c21, c22, c23);
//     LED2(x2 - 1, y2 + 1, z2 - 1, c21, c22, c23);
//     LED2(x2 - 1, y2 - 1, z2 - 1, c21, c22, c23);





//     x2o = x2;
//     y2o = y2;
//     z2o = z2;

//     xo = x;
//     yo = y;
//     zo = z;

//     delay(45);

//     x = x + xmult;
//     y = y + ymult;
//     z = z + zmult;

//     x2 = x2 + x2mult;
//     y2 = y2 + y2mult;
//     z2 = z2 + z2mult;

//     if (x >= 7) {

//       xmult = random(-1, 1);
//     }
//     if (y >= 7) {

//       ymult = random(-1, 1);
//     }
//     if (z >= 7) {

//       zmult = random(-1, 1);
//     }
//     if (x <= 0) {

//       xmult = random(0, 2);
//     }
//     if (y <= 0) {

//       ymult = random(0, 2);
//     }
//     if (z <= 0) {

//       zmult = random(0, 2);
//     }

//     if (x2 >= 7) {

//       x2mult = random(-1, 1);
//     }
//     if (y2 >= 7) {

//       y2mult = random(-1, 1);
//     }
//     if (z2 >= 7) {

//       z2mult = random(-1, 1);
//     }
//     if (x2 <= 0) {

//       x2mult = random(0, 2);
//     }
//     if (y2 <= 0) {

//       y2mult = random(0, 2);
//     }
//     if (z <= 0) {

//       z2mult = random(0, 2);
//     }




//   }



//   for (counter = 0; counter < 15; counter++) {
//     color_select = random(0, 3);
//     if (color_select == 0) {
//       c1 = 0;
//       c2 = random(0, 16);
//       c3 = random(0, 16);
//     }
//     if (color_select == 1) {
//       c1 = random(0, 16);
//       c2 = 0;
//       c3 = random(0, 16);
//     }
//     if (color_select == 2) {
//       c1 = random(0, 16);
//       c2 = random(0, 16);
//       c3 = 0;
//     }


//     int num1 = -1, num2 = -4, num3 = -6, num4 = -10;
//     for (m = 0; m < 20; m++) {

//       num1++;
//       num2++;
//       num3++;
//       num4++;


//       for (i = 3; i < 5; i++) {
//         LED1(num1, i, 3, Black);
//         LED1(num1, 3, i, Black);
//         LED1(num1, 4, i, Black);
//         LED1(num1, i, 4, Black);
//       }
//       for (i = 3; i < 5; i++) {
//         LED2(num1 + 1, i, 4, c1, c2, c3);
//         LED2(num1 + 1, 4, i, c1, c2, c3);
//         LED2(num1 + 1, 3, i, c1, c2, c3);
//         LED2(num1 + 1, i, 3, c1, c2, c3);
//       }
//       for (i = 2; i < 6; i++) {
//         LED1(num2, i, 2, Black);
//         LED1(num2, 2, i, Black);
//         LED1(num2, 5, i, Black);
//         LED1(num2, i, 5, Black);
//       }
//       for (i = 2; i < 6; i++) {
//         LED2(num2 + 1, i, 2, c1, c2, c3);
//         LED2(num2 + 1, 2, i, c1, c2, c3);
//         LED2(num2 + 1, 5, i, c1, c2, c3);
//         LED2(num2 + 1, i, 5, c1, c2, c3);
//       }
//       for (i = 1; i < 7; i++) {
//         LED1(num3, i, 1, Black);
//         LED1(num3, 1, i, Black);
//         LED1(num3, 6, i, Black);
//         LED1(num3, i, 6, Black);
//       }
//       for (i = 1; i < 7; i++) {
//         LED2(num3 + 1, i, 1, c1, c2, c3);
//         LED2(num3 + 1, 1, i, c1, c2, c3);
//         LED2(num3 + 1, 6, i, c1, c2, c3);
//         LED2(num3 + 1, i, 6, c1, c2, c3);
//       }
//       for (i = 0; i < 8; i++) {
//         LED1(num4, i, 0, Black);
//         LED1(num4, 0, i, Black);
//         LED1(num4, 7, i, Black);
//         LED1(num4, i, 7, Black);
//       }
//       for (i = 0; i < 8; i++) {
//         LED2(num4 + 1, i, 0, c1, c2, c3);
//         LED2(num4 + 1, 0, i, c1, c2, c3);
//         LED2(num4 + 1, 7, i, c1, c2, c3);
//         LED2(num4 + 1, i, 7, c1, c2, c3);
//       }

//     }

//     num1 = 8;
//     num2 = 11;
//     num3 = 13;
//     num4 = 17;

//     for (m = 0; m < 20; m++) {
//       num1--;
//       num2--;
//       num3--;
//       num4--;
//       for (i = 3; i < 5; i++) {
//         LED1(num1, i, 3, Black);
//         LED1(num1, 3, i, Black);
//         LED1(num1, 4, i, Black);
//         LED1(num1, i, 4, Black);
//       }
//       for (i = 3; i < 5; i++) {
//         LED2(num1 - 1, i, 4, 0, 0, 15);
//         LED2(num1 - 1, 4, i, 0, 0, 15);
//         LED2(num1 - 1, 3, i, 0, 0, 15);
//         LED2(num1 - 1, i, 3, 0, 0, 15);
//       }
//       for (i = 2; i < 6; i++) {
//         LED1(num2, i, 2, Black);
//         LED1(num2, 2, i, Black);
//         LED1(num2, 5, i, Black);
//         LED1(num2, i, 5, Black);
//       }
//       for (i = 2; i < 6; i++) {
//         LED2(num2 - 1, i, 2, 0, 0, 15);
//         LED2(num2 - 1, 2, i, 0, 0, 15);
//         LED2(num2 - 1, 5, i, 0, 0, 15);
//         LED2(num2 - 1, i, 5, 0, 0, 15);
//       }
//       for (i = 1; i < 7; i++) {
//         LED1(num3, i, 1, Black);
//         LED1(num3, 1, i, Black);
//         LED1(num3, 6, i, Black);
//         LED1(num3, i, 6, Black);
//       }
//       for (i = 1; i < 7; i++) {
//         LED2(num3 - 1, i, 1, 0, 0, 15);
//         LED2(num3 - 1, 1, i, 0, 0, 15);
//         LED2(num3 - 1, 6, i, 0, 0, 15);
//         LED2(num3 - 1, i, 6, 0, 0, 15);
//       }
//       for (i = 0; i < 8; i++) {
//         LED1(num4, i, 0, Black);
//         LED1(num4, 0, i, Black);
//         LED1(num4, 7, i, Black);
//         LED1(num4, i, 7, Black);
//       }
//       for (i = 0; i < 8; i++) {
//         LED2(num4 - 1, i, 0, 0, 0, 15);
//         LED2(num4 - 1, 0, i, 0, 0, 15);
//         LED2(num4 - 1, 7, i, 0, 0, 15);
//         LED2(num4 - 1, i, 7, 0, 0, 15);
//       }

//     }

//   }

// }
//////////////////////////////
void color_wheelTWO() {
  int xx, yy, zz, ww, rr = 1, gg = 1, bb = 1, ranx, rany , ranz, select, swiper;

  start = millis();

  while (millis() - start < 10000) {
    swiper = random(6);
    select = random(3);
    if (select == 0) {
      ranx = 0;
      rany = random(16);
      ranz = random(16);
    }
    if (select == 1) {
      ranx = random(16);
      rany = 0;
      ranz = random(16);
    }
    if (select == 2) {
      ranx = random(16);
      rany = random(16);
      ranz = 0;
    }


    if (swiper == 0) {
      for (yy = 0; yy < 8; yy++) {
        for (xx = 0; xx < 8; xx++) {
          for (zz = 0; zz < 8; zz++) {
            LED2(xx, yy, zz,  ranx, ranz, rany);
          }
        }
        delay(30);
      }
    }
    if (swiper == 1) {
      for (xx = 0; xx < 8; xx++) {
        for (yy = 0; yy < 8; yy++) {
          for (zz = 0; zz < 8; zz++) {
            LED2(xx, yy, zz,  ranx, ranz, rany);
          }
        }
        delay(30);
      }
    }
    if (swiper == 2) {
      for (zz = 0; zz < 8; zz++) {
        for (xx = 0; xx < 8; xx++) {
          for (yy = 0; yy < 8; yy++) {
            LED2(xx, yy, zz,  ranx, ranz, rany);
          }
        }
        delay(30);
      }
    }
    if (swiper == 3) {
      for (yy = 7; yy >= 0; yy--) {
        for (xx = 0; xx < 8; xx++) {
          for (zz = 0; zz < 8; zz++) {
            LED2(xx, yy, zz,  ranx, ranz, rany);
          }
        }
        delay(30);
      }
    }
    if (swiper == 4) {
      for (xx = 7; xx >= 0; xx--) {
        for (yy = 0; yy < 8; yy++) {
          for (zz = 0; zz < 8; zz++) {
            LED2(xx, yy, zz,  ranx, ranz, rany);
          }
        }
        delay(30);
      }
    }
    if (swiper == 5) {
      for (zz = 7; zz >= 0; zz--) {
        for (xx = 0; xx < 8; xx++) {
          for (yy = 0; yy < 8; yy++) {
            LED2(xx, yy, zz,  ranx, ranz, rany);
          }
        }
        delay(30);
      }
    }




  }

}
/////////////////////////////
void bouncyvTwo(int co22222) {
  int delaynum;
  if(co22222<1500){
    delaynum=100;
  }else{
    delaynum=15;
  }


  int wipex, wipey, wipez, ranr, rang, ranb, select, oldx[50], oldy[50], oldz[50];
  int x[50], y[50], z[50], addr, ledcount = 20, direct, direcTwo;
  int xx[50], yy[50], zz[50];
  int xbit = 1, ybit = 1, zbit = 1;
  for (addr = 0; addr < ledcount + 1; addr++) {
    oldx[addr] = 0;
    oldy[addr] = 0;
    oldz[addr] = 0;
    x[addr] = 0;
    y[addr] = 0;
    z[addr] = 0;
    xx[addr] = 0;
    yy[addr] = 0;
    zz[addr] = 0;

  }

  start = millis();

  while (millis() - start < 20000) {
    direct = random(3);
    if(co22222<1500){
      for (addr = 1; addr < ledcount + 1; addr++) {
      LED1(oldx[addr], oldy[addr], oldz[addr], Black);
      LED2(x[addr], y[addr], z[addr], xx[addr], yy[addr], zz[addr]);
      }
    }else{
      for (addr = 1; addr < ledcount + 1; addr++) {
      LED1(oldx[addr], oldy[addr], oldz[addr], Black);
      //LED2(x[addr], y[addr], z[addr], xx[addr], yy[addr], zz[addr]);
      LED1(x[addr], y[addr], z[addr], Red);
      }
      
    }


    for (addr = 1; addr < ledcount + 1; addr++) {
      oldx[addr] = x[addr];
      oldy[addr] = y[addr];
      oldz[addr] = z[addr];
    }
    delay(delaynum);







    if (direct == 0)
      x[0] = x[0] + xbit;
    if (direct == 1)
      y[0] = y[0] + ybit;
    if (direct == 2)
      z[0] = z[0] + zbit;

    if (direct == 3)
      x[0] = x[0] - xbit;
    if (direct == 4)
      y[0] = y[0] - ybit;
    if (direct == 5)
      z[0] = z[0] - zbit;





    if (x[0] > 7) {
      xbit = -1;
      x[0] = 7;
      xx[0] = random(16);
      yy[0] = random(16);
      zz[0] = 0;

    }
    if (x[0] < 0) {
      xbit = 1;
      x[0] = 0;
      xx[0] = random(16);
      yy[0] = 0;
      zz[0] = random(16);

    }
    if (y[0] > 7) {
      ybit = -1;
      y[0] = 7;
      xx[0] = 0;
      yy[0] = random(16);
      zz[0] = random(16);

    }
    if (y[0] < 0) {
      ybit = 1;
      y[0] = 0;
      xx[0] = 0;
      yy[0] = random(16);
      zz[0] = random(16);

    }
    if (z[0] > 7) {
      zbit = -1;
      z[0] = 7;
      xx[0] = random(16);
      yy[0] = 0;
      zz[0] = random(16);

    }
    if (z[0] < 0) {
      zbit = 1;
      z[0] = 0;
      xx[0] = random(16);
      yy[0] = random(16);
      zz[0] = 0;

    }

    for (addr = ledcount; addr > 0; addr--) {
      x[addr] = x[addr - 1];
      y[addr] = y[addr - 1];
      z[addr] = z[addr - 1];
      xx[addr] = xx[addr - 1];
      yy[addr] = yy[addr - 1];
      zz[addr] = zz[addr - 1];
    }


  }
}
//////////////////////////////
void num1(){
  int x,y1,y2,z1,z2;
  for(x=7;x>-1;x--){
    for(y1=2;y1<6;y1++){
      LED(x, y1, 0, Red);
    }
    for(y2=2;y2<6;y2++){
      LED(x, y2, 7, Red);
    }
    for(z1=1;z1<7;z1++){
      LED(x, 1, z1, Red);
    }
    for(z2=1;z2<7;z2++){
      LED(x, 6, z2, Red);
    }
    delay(200);
    if(x!=0){
      clearCube();
    }
  }
  delay(300);
}
//////////////////////////////







