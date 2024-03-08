//The rain 1 code is a simple animation of falling raindrops

#include <Adafruit_NeoPixel.h>

#define PIN 6 // LED data input pin
#define WIDTH 8 // Width of the cube
#define HEIGHT 8 // Height of the cube
#define DEPTH 8 // Depth of the cube
#define LED_COUNT (WIDTH * HEIGHT * DEPTH) // Calculate the total number of LEDs
#define NUM_DROPS 50 //The number of raindrops displayed on the screen at the same time
#define DROP_SPEED 100 // The speed at which raindrops fall, milliseconds

Adafruit_NeoPixel cube(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

struct Drop {
   int x;
   int y;
   int z;
} drops[NUM_DROPS]; // Array to store raindrops

void setup() {
   cube.begin(); // Initialize the LED cube
   cube.show(); // Turn off all LEDs during initialization
   //Initialize raindrop position
   for (int i = 0; i < NUM_DROPS; i++) {
     drops[i].x = random(WIDTH);
     drops[i].y = random(DEPTH);
     drops[i].z = random(HEIGHT);
   }
}

void loop() {
   cube.clear(); // Clear the raindrops of the previous frame

   //Update raindrop position
   for (int i = 0; i < NUM_DROPS; i++) {
     // Draw the current raindrop
     cube.setPixelColor(getPixelIndex(drops[i].x, drops[i].y, drops[i].z), cube.Color(0, 0, 255)); // GRB order, blue

     //Update the raindrop to the next position
     drops[i].z--;
     // If the raindrop reaches the bottom, regenerate it at the top
     if (drops[i].z < 0) {
       drops[i].x = random(WIDTH);
       drops[i].y = random(DEPTH);
       drops[i].z = HEIGHT - 1;
     }
   }

   cube.show(); // Display the raindrops of the current frame
   delay(DROP_SPEED); // Control the speed of raindrops falling
}

// A helper function to convert x, y, z coordinates to LED indices
int getPixelIndex(int x, int y, int z) {
   return (z * WIDTH * DEPTH) + (y * WIDTH) + x;
}
