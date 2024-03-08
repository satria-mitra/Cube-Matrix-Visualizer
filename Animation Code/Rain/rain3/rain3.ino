//Updated the gradient animation of the code. 
//After the raindrops fall, they will change from dark blue to light blue.


#include <Adafruit_NeoPixel.h>

#define PIN 6 // LED data input pin
#define WIDTH 8 // Width of the cube
#define HEIGHT 8 // Height of the cube
#define DEPTH 8 // Depth of the cube
#define LED_COUNT (WIDTH * HEIGHT * DEPTH) // Calculate the total number of LEDs
#define DROP_SPEED 200 // The speed at which raindrops fall, milliseconds
#define NUM_DROPS 10 // Control the number of raindrops falling at the same time

Adafruit_NeoPixel cube(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

struct Drop {
   int x;
   int y;
   int z = 0; // Initialization starts falling at the top
} drops[NUM_DROPS];

//Record the water level at each location
int waterLevel[WIDTH][DEPTH];

void setup() {
   cube.begin(); // Initialize the LED cube
   cube.show(); // Turn off all LEDs during initialization
   memset(waterLevel, 0, sizeof(waterLevel)); // Initialize the water array
   randomSeed(analogRead(0)); // Initialize random seed
   initDrops(); // Initialize raindrop position
}

void loop() {
   cube.clear(); // Clear all LEDs before starting a new frame

   //Update raindrop position
   updateDrops();

   //display water level
   displayWater();

   cube.show(); // Update LED display
   delay(DROP_SPEED); //Control the falling speed

   // Check if the water level needs to be reset
   checkAndResetWaterLevel();
}

void initDrops() {
   for(int i = 0; i < NUM_DROPS; i++) {
     drops[i].x = random(WIDTH);
     drops[i].y = random(DEPTH);
     drops[i].z = HEIGHT - 1; // Start falling from the top
   }
}

void updateDrops() {
   for(int i = 0; i < NUM_DROPS; i++) {
     if(drops[i].z >= 0) { // If the raindrops have started falling
       int index = getIndex(drops[i].x, drops[i].y, drops[i].z);
       cube.setPixelColor(index, cube.Color(0, 0, 255)); // Set to blue
     }

     // Update raindrop position or reset
     if(drops[i].z == 0 || waterLevel[drops[i].x][drops[i].y] >= drops[i].z) {
       waterLevel[drops[i].x][drops[i].y]++;
       drops[i].x = random(WIDTH);
       drops[i].y = random(DEPTH);
       drops[i].z = HEIGHT - 1;
     } else {
       drops[i].z--;
     }
   }
}

void displayWater() {
   for(int x = 0; x < WIDTH; x++) {
     for(int y = 0; y < DEPTH; y++) {
       for(int z = 0; z < waterLevel[x][y]; z++) {
         int index = getIndex(x, y, z);
         cube.setPixelColor(index, cube.Color(0, 0, 255)); // Set to blue to represent water
       }
     }
   }
}

void checkAndResetWaterLevel() {
   bool isFull = true;
   for(int x = 0; x < WIDTH; x++) {
     for(int y = 0; y < DEPTH; y++) {
       if(waterLevel[x][y] < HEIGHT) {
         isFull = false;
         break;
       }
     }
     if(!isFull) break;
   }

   if(isFull) {
     memset(waterLevel, 0, sizeof(waterLevel)); // Reset water level
   }
}

int getIndex(int x, int y, int z) {
   // Convert three-dimensional coordinates to linear index of LED
   return (z * WIDTH * DEPTH) + (y * WIDTH) + x;
}