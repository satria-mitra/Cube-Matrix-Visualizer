#include <Adafruit_NeoPixel.h>

#define PIN 6
#define WIDTH 8
#define HEIGHT 8
#define DEPTH 8
#define LED_COUNT (WIDTH * HEIGHT * DEPTH)
#define DROP_SPEED 100
#define NUM_DROPS 30

Adafruit_NeoPixel cube(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

struct Drop {
  int x;
  int y;
  int z = 0;
  bool active;
} drops[NUM_DROPS];

int waterLevel[WIDTH][DEPTH];

// 定义每层的颜色值
uint8_t layerColors[HEIGHT][3] = {
  {0, 0, 25},    // 最顶层浅蓝色
  {0, 0, 50},
  {0, 0, 85},
  {0, 0, 115},
  {0, 0, 150},
  {0, 0, 180},
  {0, 0, 220},
  {0, 0, 255}   // 最底层深蓝色
};

void setup() {
  cube.begin();
  cube.show(); // 初始化关闭所有LED
  memset(waterLevel, 0, sizeof(waterLevel)); // 初始化水位数组
  randomSeed(analogRead(0)); // 初始化随机种子
  for (int i = 0; i < NUM_DROPS; i++) {
    drops[i].active = false; // Initially, drops are not active
  }
}

void loop() {
  cube.clear(); // 清除所有LED

  activateDrops();
  updateAndDisplayDrops();

  displayWaterLevel();

  cube.show(); // 更新LED显示
  delay(DROP_SPEED); // 控制下落速度

  // 检查是否需要重置水位
  checkAndResetWaterLevel();
}

void activateDrops() {
  for (int i = 0; i < NUM_DROPS; i++) {
    if (!drops[i].active) {
      drops[i].x = random(WIDTH);
      drops[i].y = random(DEPTH);
      drops[i].z = HEIGHT - 1; // Start from the top
      drops[i].active = true; // Activate the drop
      break; // Activate only one drop per cycle
    }
  }
}

void updateAndDisplayDrops() {
  for (int i = 0; i < NUM_DROPS; i++) {
    if (drops[i].active && drops[i].z >= 0) {
      if (waterLevel[drops[i].x][drops[i].y] < drops[i].z) {
        // Display drop
        int index = getIndex(drops[i].x, drops[i].y, drops[i].z);
        cube.setPixelColor(index, cube.Color(layerColors[HEIGHT - 1 - drops[i].z][1], 
                                             layerColors[HEIGHT - 1 - drops[i].z][0], 
                                             layerColors[HEIGHT - 1 - drops[i].z][2])); // Adjust for GRB format
        drops[i].z--; // Move the drop down
      } else {
        // Drop has reached its water level or bottom
        drops[i].active = false; // Deactivate the drop
        if (waterLevel[drops[i].x][drops[i].y] < HEIGHT) {
          waterLevel[drops[i].x][drops[i].y]++; // Increase water level
        }
      }
    }
  }
}

void displayWaterLevel() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < DEPTH; y++) {
      for (int z = 0; z < waterLevel[x][y]; z++) {
        int index = getIndex(x, y, HEIGHT - 1 - z);
        cube.setPixelColor(index, cube.Color(layerColors[z][1], layerColors[z][0], layerColors[z][2])); // Adjust for GRB format
      }
    }
  }
}

void checkAndResetWaterLevel() {
  bool isFull = true;
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < DEPTH; y++) {
      if(waterLevel[x][y] < HEIGHT) {
        isFull = false; // If any column is not full, continue
        break;
      }
    }
    if (!isFull) break;
  }

  if (isFull) {
    memset(waterLevel, 0, sizeof(waterLevel)); // Reset water levels
    for (int i = 0; i < NUM_DROPS; i++) {
      drops[i].active = false; // Deactivate all drops
    }
  }
}

int getIndex(int x, int y, int z) {
  return z * WIDTH * DEPTH + y * WIDTH + x;
}
