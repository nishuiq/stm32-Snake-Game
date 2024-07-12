#include "Renderer.h"
#include <iostream>
#include "config.h"
#include "snake_icno.h"

Renderer::Renderer(const std::size_t screen_width, const std::size_t screen_height, const std::size_t grid_width,
                   const std::size_t grid_height)
    : screen_width_(screen_width),
      screen_height_(screen_height),
      grid_width_(grid_width),
      grid_height_(grid_height),
      tft_renderer_(TFT(CS, DC, RST)) {
  // initialize the display
  tft_renderer_.begin();
  tft_renderer_.background(0, 0, 0);
  tft_renderer_.textSize(2);
}

void Renderer::Render(Snake &snake, Snake::Point &food) {
  typedef struct {
    int x, y;
    int w, h;
  } Rect;
  Rect block;
  block.w = screen_width_ / grid_width_;
  block.h = screen_height_ / grid_height_;

  // clean screen
  // 不要使用这种方法，很慢且闪烁
  // tft_renderer_.background(0, 0, 0);

  // Render food
  tft_renderer_.stroke(255, 0, 204);
  tft_renderer_.fill(255, 0, 204);
  block.x = food.first * block.w;
  block.y = food.second * block.h;
  tft_renderer_.rect(block.x, block.y, block.w, block.h);

  // Render Snake's Body
  // tft_renderer_.stroke(0, 139, 0);
  // tft_renderer_.fill(0, 139, 0);
  // for (auto const &[x, y] : snake.body_) {
  //   block.x = x * block.w;
  //   block.y = y * block.h;
  //   tft_renderer_.rect(block.x, block.y, block.w, block.h);
  // }

  // Erase Snake's tail
  if (!snake.ate_food) {
    tft_renderer_.stroke(0, 0, 0);
    tft_renderer_.fill(0, 0, 0);
    block.x = snake.prevTail_.first * block.w;
    block.y = snake.prevTail_.second * block.h;
    tft_renderer_.rect(block.x, block.y, block.w, block.h);
  } else {
    snake.ate_food = false;
  }

  // Render Snake's head
  block.x = snake.head_.first * block.w;
  block.y = snake.head_.second * block.h;
  if (snake.alive_) {
    tft_renderer_.stroke(0, 122, 204);
    tft_renderer_.fill(0, 122, 204);
  } else {
    tft_renderer_.stroke(255, 0, 0);
    tft_renderer_.fill(255, 0, 0);
  }
  tft_renderer_.rect(block.x, block.y, block.w, block.h);
}

void Renderer::UpdateTitle(int score, int fps) {
  // std::string str = "fps: " + std::to_string(fps);
  // tft_renderer_.text(str.c_str(), 0, screen_height_ - 20);
  // Serial.printf("FPS : %d\n", fps);
}

void Renderer::UpdateTitle(std::string const &str) {
  // Serial.println(str.c_str());
  tft_renderer_.text(str.c_str(), 0, screen_height_ - 20);
}

void Renderer::showLoading() {
  tft_renderer_.background(0, 0, 0);
  showImg();
  delay(1000);
  for (int i = 0; i < screen_width_; i += 4) {
    tft_renderer_.stroke(255, 20, 147);
    tft_renderer_.fill(255, 20, 147);
    tft_renderer_.rect(0, screen_height_ - 20, i, 20);

    tft_renderer_.stroke(255, 255, 255);
    tft_renderer_.text("Loading ...", 0, screen_height_ - 20);
  }
  delay(1000);
  tft_renderer_.background(0, 0, 0);
}

void Renderer::showImg() {
  for (int i = 0; i < 50; ++i) {
    for (int j = 0; j < 50; ++j) {
      tft_renderer_.stroke(gImage_snake_icno[i + j * 50], 123, 123);
      tft_renderer_.point(i + screen_height_ / 3, j + screen_height_ / 3);
    }
  }
}
