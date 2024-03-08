//Falling raindrops and accumulated water both have a gradient from dark to light blue depending on their height within the cube.


#include <Adafruit_NeoPixel.h>

#define PIN 6
#define WIDTH 8
#define HEIGHT 8
#define DEPTH 8
#define LED_COUNT (WIDTH * HEIGHT * DEPTH)
#define DROP_SPEED 200
#define NUM_DROPS 10

Adafruit_NeoPixel cube(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

struct Drop {
  int x;
  int y;
  int z = 0; // 初始化在顶部开始下落
} drops[NUM_DROPS];

int waterLevel[WIDTH][DEPTH];

void setup() {
  cube.begin();
  cube.show(); // Initialize all pixels to 'off'
  memset(waterLevel, 0, sizeof(waterLevel));
  randomSeed(analogRead(0));
  for (int i = 0; i < NUM_DROPS; i++) {
    resetDrop(i);
  }
}

void loop() {
  cube.clear();

  for (int i = 0; i < NUM_DROPS; i++) {
    // 根据雨滴当前高度设置颜色，以符合深蓝到浅蓝的渐变
    uint8_t blue = map(drops[i].z, 0, HEIGHT - 1, 255, 64); // 从深蓝到浅蓝的渐变
    int index = getIndex(drops[i].x, drops[i].y, drops[i].z);
    cube.setPixelColor(index, cube.Color(0, 0, blue));

    drops[i].z--;

    if (drops[i].z < 0 || waterLevel[drops[i].x][drops[i].y] >= drops[i].z) {
      waterLevel[drops[i].x][drops[i].y]++;
      resetDrop(i);
    }
  }

  // 显示积累的水，颜色从底层的深蓝到顶层的浅蓝
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < DEPTH; y++) {
      for (int z = 0; z < waterLevel[x][y]; z++) {
        uint8_t blue = map(z, 0, HEIGHT - 1, 255, 64); // 从深蓝到浅蓝的渐变
        int index = getIndex(x, y, z);
        cube.setPixelColor(index, cube.Color(0, 0, blue));
      }
    }
  }

  cube.show();
  delay(DROP_SPEED);

  checkAndReset();
}

void resetDrop(int i) {
  drops[i].x = random(WIDTH);
  drops[i].y = random(DEPTH);
  drops[i].z = HEIGHT - 1;
}

void checkAndReset() {
  bool isFull = true;
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < DEPTH; y++) {
      if(waterLevel[x][y] < HEIGHT) {
        isFull = false;
        break;
      }
    }
  }

  if(isFull) {
    memset(waterLevel, 0, sizeof(waterLevel)); // Reset water levels
    for (int i = 0; i < NUM_DROPS; i++) {
      resetDrop(i); // Reset drops for a new cycle
    }
  }
}

int getIndex(int x, int y, int z) {
  return z * WIDTH * DEPTH + y * WIDTH + x;
}
