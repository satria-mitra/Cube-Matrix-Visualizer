// This test turns on a whold layer at once
// Helps check the panel for cold solder joints and other problems
// You must select the layer you want to test in the line below.
#define myLayer 0  // Specify here the layer you want to test


#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6  // data pin


volatile uint16_t mymatrix[16][8];  // color of each LED in the matrix
int brightness = 30;  // set brightness here  0-256 with 10 for easy on eyes

// create instance of neo_matrix configured for this display
Adafruit_NeoMatrix cube = Adafruit_NeoMatrix(8, 8, 8, 1, PIN,
                          NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                          NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                          NEO_RGB            + NEO_KHZ800);

// Color definitions
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
