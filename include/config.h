#pragma once
#include <Arduino.h>

/* 0左转 1右转 2方向不改变 */
enum SNAKE_TOWARD {
    TOWARD_LEFT,
    TOWARD_RIGHT,
    TOWARD_UNDO
};

/* LED Pin，具体看板子原理图 */
const int LED_PIN0 = PC4;
const int LED_PIN1 = PC5;

/* Button Pin */
const int KEY1 = PA8;
const int KEY2 = PC9;

/* TFT Pin */
const int CS = PA4;
const int DC = PB1;
const int RST = PB0; // RESET 命名冲突了

/* TFT screen size 160x128 */
const int WIDTH = 160;
const int HEIGTH = 128;

/* global var */
volatile SNAKE_TOWARD key_state; // 按键值，{0,1,2} = {key_1, key_2, 无操作} 映射谁按的

/* interrupts */
void KEY1_press()
{
  static auto KEY1_TIME = millis();
  auto t = millis();
  if (t - KEY1_TIME >= 50) {  // Debounce
    KEY1_TIME = t;
    key_state = TOWARD_LEFT;
  }
}
void KEY2_press()
{
  static auto KEY2_TIME = millis();
  auto t = millis();
  if (t - KEY2_TIME >= 50) {  // Debounce
    KEY2_TIME = t;
    key_state = TOWARD_RIGHT; 
  }
}

/* body size */
const int body_x = 8;  // 8
const int body_y = 8;

/* Real snake map size */
const int WIDTH_SIZE = WIDTH / body_x;
const int HEIGHT_SIZE = HEIGTH / body_y;

/* icon */
#include <snake_icno.h>
extern const unsigned char gImage_snake_icno[2500];

/* frame_rate ms */
const int frame_rate = 500;
