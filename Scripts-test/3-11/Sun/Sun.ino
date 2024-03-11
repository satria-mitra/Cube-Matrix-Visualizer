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
int T= 26;
int redPin = 10;
int greenPin = 11;
int bluePin = 12;
int buttonPin = 13;
int buttonState = 0;
int lastButtonState = 0;
int option = 0; // Variable to track the current option
int radiusChange = 0; // 用于控制半径变化的变量
#define COMMON_ANODE
////////////////////



void setup() {
  Wire.begin();
  

  Serial.begin(9600);
  airSensor.begin(Wire); // Initialize the SCD30 sensor
    if (airSensor.begin() == false) {
    Serial.println("SCD30 not detected, please check the wiring!");
    while (1); // If the sensor is not detected, halt the program
  }

  Serial.println("Enter T value (0-100) to control the animation speed and effect:");
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
////////////////////////////


  // startTimer(10);

  

  
  delay(2000);
}
void loop() {
  if (Serial.available() > 0) {
    T = Serial.parseInt(); // 读取输入的整数值
    Serial.print("T is now: ");
    Serial.println(T);
  }
  // put your main code here, to run repeatedly:
  clearCube(); // 清空LED矩阵
  moveBall(); // 移动小球
  drawBall(); // 绘制小球
  cube.show(); // 显示小球
  delay(100); // 控制动画速度

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
  // 直接使用red, green, blue值计算颜色
  // 注意: 不需要使用pgm_read_byte_near，因为这些值已经是直接可用的
  byte myred = 36 * red;
  byte mygreen = 36 * green;
  byte myblue = 36 * blue;
  return cube.Color(myred, mygreen, myblue);
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

struct Ball {
  int x, y, z;
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
  ball.x = random(2, 6); // 保证x在2到5之间
  ball.y = random(2, 6); // 保证y在2到5之间
  ball.z = random(2, 6); // 保证z在2到5之间

  // 初始化小球速度，确保小球移动
  ball.vx = random(-1, 2); // -1, 0, 或 1
  ball.vy = random(-1, 2);
  ball.vz = random(-1, 2);
}

void moveBall() {
  // 随机决定是否改变方向
  if (random(10) < 3) { // 大约30%的几率改变方向
    ball.vx = random(-1, 2); // -1, 0, 1
    ball.vy = random(-1, 2);
    ball.vz = random(-1, 2);
  }

  // 更新位置
  ball.x += ball.vx;
  ball.y += ball.vy;
  ball.z += ball.vz;

  // 确保小球不会离开内部4x4x4矩阵
  if (ball.x < 2 || ball.x > 5) {
    ball.vx *= -1;
    ball.x += ball.vx;
  }
  if (ball.y < 2 || ball.y > 5) {
    ball.vy *= -1;
    ball.y += ball.vy;
  }
  if (ball.z < 2 || ball.z > 5) {
    ball.vz *= -1;
    ball.z += ball.vz;
  }

  // 更新半径变化状态
  radiusChange++;
}

void drawBall() {
  int radius = calculateRadius(); // 使用新的半径计算逻辑
  uint32_t ballColor = calculateColor(T); // 可以保留T来控制颜色，或者自定义颜色控制逻辑
  for (int dx = -radius; dx <= radius; dx++) {
    for (int dy = -radius; dy <= radius; dy++) {
      for (int dz = -radius; dz <= radius; dz++) {
        if (dx*dx + dy*dy + dz*dz <= radius*radius) {
          int drawX = ball.x + dx;
          int drawY = ball.y + dy;
          int drawZ = ball.z + dz;
          if (drawX >= 0 && drawX < 8 && drawY >= 0 && drawY < 8 && drawZ >= 0 && drawZ < 8) {
            LED(drawX, drawY, drawZ, ballColor);
          }
        }
      }
    }
  }
}

uint32_t calculateColor(int T) {
  // T小于25时，设置为蓝色；大于等于25时，设置为红色
  if (T < 25) {
    return GetColor(Blue); // 假设Blue已经定义为蓝色的索引
  } else {
    return GetColor(Red); // 假设Red已经定义为红色的索引
  }
}


