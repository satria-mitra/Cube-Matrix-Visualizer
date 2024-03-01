// This is to test each layer panel when it is completed and 
// before assembly of the cube.  It tests each LED by briefly turning on
// red, then green, then blue LED.  It test them in order in the chain,
// so you will see it snake back and forth following the data line.
#define myLayer 1
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
//#define NUMPIXELS 125
#define PIN 6
// Adafruit_NeoPixel cube = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_RGB + NEO_KHZ400);

// uint32_t Red = cube.Color(255, 0, 0);
// uint32_t Green = cube.Color(0, 255, 0);
// uint32_t Blue = cube.Color(0, 0, 255);
// uint32_t Yellow = cube.Color(128, 128, 0);
// uint32_t White = cube.Color(128, 128, 200);
// uint32_t Black = cube.Color(0, 0, 0);
// uint32_t Aqua = cube.Color(0, 128, 128);


// void setup() {

//   cube.setBrightness(50);
//   cube.begin();
// }

// void loop() {
//   for (int j=0; j<125; j++) {
//      cube.setPixelColor(j, Red);
//      cube.show();
//      delay(300);
//      cube.setPixelColor(j, Green);
//      cube.show();
//      delay(300);
//      cube.setPixelColor(j, Blue);
//      cube.show();
//      delay(300); 
//      cube.clear();
//   }
 
// }

volatile uint16_t mymatrix[16][8];  // color of each LED in the matrix
int brightness = 10;  // set brightness here  0-256 with 10 for easy on eyes

Adafruit_NeoMatrix cube = Adafruit_NeoMatrix(8, 8, 8, 1, PIN,
                          NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                          NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                          NEO_RGB            + NEO_KHZ400);

uint32_t Red = cube.Color(255, 0, 0);
uint32_t Green = cube.Color(0, 255, 0);
uint32_t Blue = cube.Color(0, 0, 255);
//uint32_t Red = cube.ColorHSV(0);
//uint32_t Green = cube.ColorHSV(21845);
//uint32_t Blue = cube.ColorHSV(43690);
uint32_t Yellow = cube.ColorHSV(10922);
uint32_t Black = cube.Color(0, 0, 0);
uint32_t Aqua = cube.ColorHSV(32767);
uint32_t White = cube.Color(255,255,255); 
//uint32_t White = cube.ColorHSV(26000,40,255); 
uint32_t Purple = cube.ColorHSV(49150);

uint32_t mycolor;
byte myred, mygreen, myblue;  // components of a color
const byte color [44][3] PROGMEM = {  // the color pallette table
  {0, 0, 0}, //Black
  {7, 0, 0}, {7, 1, 0}, {6, 1, 0}, {6, 2, 0}, {5, 2, 0}, {5, 3, 0}, {4, 3, 0}, {4, 4, 0}, {3, 4, 0}, {3, 5, 0}, {2, 5, 0}, {2, 6, 0}, {1, 6, 0}, {1, 7, 0}, // Red to Green
  {0, 7, 0}, {0, 7, 1}, {0, 6, 1}, {0, 6, 2}, {0, 5, 2}, {0, 5, 3}, {0, 4, 3}, {0, 4, 4}, {0, 3, 4}, {0, 3, 5}, {0, 2, 5}, {0, 2, 6}, {0, 1, 6}, {0, 1, 7}, // Green to Blue
  {0, 0, 7}, {1, 0, 7}, {1, 0, 6}, {2, 0, 6}, {2, 0, 5}, {3, 0, 5}, {3, 0, 4}, {4, 0, 4}, {4, 0, 3}, {5, 0, 3}, {5, 0, 2}, {6, 0, 2}, {6, 0, 1}, {7, 0, 1}, // Blue to Red
  {7, 7, 7}  // White
};

void setup() {
  
  // cube.setTextWrap(false);
  // cube.setBrightness(brightness);
  // cube.begin();
  // cube.drawPixel(1,1,Red);
  // clearCube();

  // delay(1);

    cube.begin();
  cube.setTextWrap(false);
  cube.setBrightness(brightness);

  // TIMER 1 Setup for interrupt frequency 67 Hz = 15 ms refresh rate

  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 67.00167504187604 Hz increments
  OCR1A = 29849; // = 16000000 / (8 * 67.00167504187604) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 8 prescaler
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts
  delay(1);
}

// void loop() {
//  int z=myLayer;
//     for (int x = 0; x < 8; x++) {
//       for (int y = 0; y < 8; y++) {
//         //LED(x,y,z,Red);  // you could try other colors if you are worried about the LEDs themselves
//         cube.drawPixel(x,y,Red);
//       }                  // but any color will work if you are just testing the wiring
//     }
//     delay(2000);
//     clearCube();
// }

// void clearCube(){
// cube.fill(Black);
// }

void loop() {
 int z=myLayer;
    for (int x = 0; x < 5; x++) {
      for (int y = 0; y < 5; y++) {
        LED(x,y,z,Red);  // you could try other colors if you are worried about the LEDs themselves
      }                  // but any color will work if you are just testing the wiring
    }
    delay(2000);
    clearCube();
}



// these next 3 subroutines were used by

void redON(int x, int y, int z) {
  LED(x,y,z,Red);
}

void greenON(int x, int y, int z) {
  LED(x,y,z,Green);
}

void blueON(int x, int y, int z) {
  LED(x,y,z,Blue);
}

//  This routine generates 42 different colors around the color wheel using our 3 bit BAM
uint32_t GetColor(int thecolor) {
  myred = 36*pgm_read_byte_near(&color[thecolor][0]);
  mygreen = 36* pgm_read_byte_near(&color[thecolor][1]);
  myblue = 36*pgm_read_byte_near(&color[thecolor][2]);
  return cube.Color(myred,mygreen, myblue);
}

// Sets intensity of all three colors of one LED to generate a specified color
void LED(int x, int y, int z, unsigned int mycolor){
  //if (z%2) cube.drawPixel((z+1)*5-x-1, 4-y, mycolor);
  cube.drawPixel(x+(z*8),y, mycolor);
}


void clearCube(){
cube.fill(Black);
}

// show cube for a multiples of 10 ms before clearing it.
void showCube(int mytime) {
  delay(20 * mytime);
  clearCube();
}

void pause() {  // pause between animations
  clearCube();
  delay(500);
}

// get a random color from color definitions
uint32_t randomColor() {
  uint32_t mycolor;
  switch (1 + random(7)) {
    case 1:
      mycolor = Red;
      return mycolor;
      break;
    case 2:
      mycolor = Green;
      return mycolor;
      break;
    case 3:
      mycolor = Blue;
      return mycolor;
      break;
    case 4:
      mycolor = Yellow;
      return mycolor;
      break;
    case 5:
      mycolor = Aqua;
      return mycolor;
      break;
    case 6:
      mycolor = Purple;
      return mycolor;
      break;
    case 7:
      mycolor = White;
      return mycolor;
      break;
  }
}

// *******************************  Start of Refresh Timer Interrupt ************************
ISR(TIMER1_COMPA_vect) {
  cube.show();
}
