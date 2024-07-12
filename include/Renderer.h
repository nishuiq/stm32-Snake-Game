#pragma once
#include <TFT.h>
#include <string>
#include "snake.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height, const std::size_t grid_width,
           const std::size_t grid_height);

  void Render(Snake &snake, Snake::Point &food);
  void UpdateTitle(int score, int fps);
  void UpdateTitle(std::string const &str);
  void showLoading();

 private:
  TFT tft_renderer_;
  const std::size_t screen_width_;
  const std::size_t screen_height_;
  const std::size_t grid_width_;
  const std::size_t grid_height_;

  void showImg();
};
