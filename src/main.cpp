#include <Arduino.h>
#include <config.h>
#include <snake.hpp>
// #include <ezButton.h> // 带防抖动
#include <TFT.h>      // Hardware-specific library
#include <SPI.h>

#include <map>
#include <queue>
#include <random>

typedef std::pair<int, int> PII;

  // /* LED Pin，具体看板子原理图 */
  // const int LED_PIN0 = PC4;
  // const int LED_PIN1 = PC5;

  // /* Button Pin */
  // const int KEY1 = PA8;
  // const int KEY2 = PC9;

  // /* TFT Pin */
  // const int CS = PA4;
  // const int DC = PB1;
  // const int RST = PB0; // RESET 命名冲突了

  // /* TFT screen size 160x128 */
  // const int WIDTH = 160;
  // const int HEIGTH = 128;

  // /* global var */
  // volatile int key_state; // 按键值，{0,1,2} = {key_1, key_2, 无操作} 映射谁按的

  // /* interrupts */
  // void KEY1_press()
  // {
  //   static auto KEY1_TIME = millis();
  //   auto t = millis();
  //   if (t - KEY1_TIME >= 50) {  // Debounce
  //     KEY1_TIME = t;
  //     key_state = 0;
  //   }
  // }
  // void KEY2_press()
  // {
  //   static auto KEY2_TIME = millis();
  //   auto t = millis();
  //   if (t - KEY2_TIME >= 50) {  // Debounce
  //     KEY2_TIME = t;
  //     key_state = 1; 
  //   }
  // }

  // /* body size */
  // const int body_x = 8;
  // const int body_y = 8;

  // /* Real snake map size */
  // const int WIDTH_SIZE = WIDTH / body_x;
  // const int HEIGHT_SIZE = HEIGTH / body_y;

  // /* icon */
  // #include <snake_icno.h>
  // extern const unsigned char gImage_snake_icno[2500];

  // /* frame_rate ms */
  // const int frame_rate = 500;

TFT TFTscreen = TFT(CS, DC, RST);
Snake snake;

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_PIN0, OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(KEY1, INPUT);
  pinMode(KEY2, INPUT);

  // initialize the display
  TFTscreen.begin();
  // black background
  TFTscreen.background(0, 0, 0);
  TFTscreen.textSize(2);

  /* init snake */
  snake.init(&TFTscreen);
  /* interrupts */
  attachInterrupt(digitalPinToInterrupt(KEY1), KEY1_press, RISING);
  attachInterrupt(digitalPinToInterrupt(KEY2), KEY2_press, RISING);

  delay(2000);
}

/**
 * @brief print RGB text when game win or lose
*/
void game_print(const char* s)
{
  TFTscreen.textSize(2);
  while (true) {
    // generate a random color
    int redRandom = random(0, 255);
    int greenRandom = random(0, 255);
    int blueRandom = random(0, 255);

    // set a random font color
    TFTscreen.stroke(redRandom, greenRandom, blueRandom);
    TFTscreen.text(s, 10, 62);
    delay(200);
    if (key_state != 2) {
      break;
    }
  }
}

/**
 * @brief print icon to screen
 */
void testIMG()
{
  for (int i = 0; i < 50; ++i) {
    for (int j = 0; j < 50; ++j) {
      TFTscreen.stroke(gImage_snake_icno[i + j * 50], 123, 123);
      TFTscreen.point(i + WIDTH / 3, j + HEIGTH / 3);
    }
  }
}

/**
 * @brief fake loading progress bar
 */
void loading(int start, int end)
{
  TFTscreen.background(0, 0, 0);
  testIMG();

  for (int i = start; i < end; i += 4) {
    TFTscreen.stroke(255, 20, 147); // DeepPink
    TFTscreen.fill(255, 20, 147);
    TFTscreen.rect(0, HEIGTH - 20, i, 20);

    TFTscreen.stroke(255, 255, 255); // WHITE text
    TFTscreen.text("Loading ...", 0, HEIGTH - 20);
  }
}

void loop()
{
  /* reset key value */
  key_state = TOWARD_UNDO;

  loading(0, WIDTH);
  snake.start();
  while (!snake.is_over()) {
    /* get key_press_value */
    snake.go(key_state);

    /* reset key value */
    key_state = TOWARD_UNDO;

    // if (snake.is_win()) {
    //   break;
    // }
    /* game frame rate */
    delay(frame_rate);
  }

  if (snake.is_over()) {
    game_print("You lose!");
  } else {
    game_print("You Win!");
  }

  delay(1000);
}