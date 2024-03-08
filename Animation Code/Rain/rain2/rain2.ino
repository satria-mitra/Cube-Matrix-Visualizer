//The rain 2 code adds an animation of accumulation of rain after the raindrops fall.


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
  int z = -1; // Initialized to -1, indicating that the raindrops have not yet started falling
} drops[NUM_DROPS];

//Record the water level at each location
int waterLevel[WIDTH][DEPTH];

void setup() {
   cube.begin(); // Initialize the LED cube
   cube.show(); // Turn off all LEDs during initialization
   memset(waterLevel, 0, sizeof(waterLevel)); // Initialize the water array
   //Initialize the raindrop array
   for(int i = 0; i < NUM_DROPS; i++) {
     drops[i].x = random(WIDTH);
     drops[i].y = random(DEPTH);
     drops[i].z = HEIGHT - 1 - waterLevel[drops[i].x][drops[i].y]; // Set the starting height of raindrops
   }
}

void loop() {
   cube.clear(); // Clear all LEDs before starting a new frame

   //Update raindrop position
  for(int i = 0; i < NUM_DROPS; i++) {
    if(drops[i].z >= 0) { // If the raindrops have started falling
      cube.setPixelColor(getPixelIndex(drops[i].x, drops[i].y, drops[i].z), cube.Color(0, 0, 255)); //Set to blue
    }

drops[i].z--; // Raindrops falling

     // Check if the raindrop reaches the bottom or water level
     if(drops[i].z < 0 || drops[i].z < waterLevel[drops[i].x][drops[i].y]) {
       waterLevel[drops[i].x][drops[i].y] = min(HEIGHT - 1, waterLevel[drops[i].x][drops[i].y] + 1); // Increase water level
       drops[i].x = random(WIDTH); //Reset raindrop position
       drops[i].y = random(DEPTH);
       drops[i].z = HEIGHT - 1 - waterLevel[drops[i].x][drops[i].y]; // Reset the starting height of raindrops
     }
   }

   //Display water level
   for(int x = 0; x < WIDTH; x++) {
     for(int y = 0; y < DEPTH; y++) {
       for(int z = 0; z < waterLevel[x][y]; z++) {
         cube.setPixelColor(getPixelIndex(x, y, z), cube.Color(0, 0, 255)); // Set to blue to represent water
       }
     }
   }

   cube.show(); // Update LED display
   delay(DROP_SPEED); //Control the falling speed
}

int getPixelIndex(int x, int y, int z) {
   // Convert three-dimensional coordinates to linear index of LED
   return (z * WIDTH * DEPTH) + (y * WIDTH) + x;
}
