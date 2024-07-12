#pragma once
#include <random>
#include "Controller.h"
#include "Renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);

  void Run(Controller &controller, Renderer &renderer, std::size_t target_frame_duration);

  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake_;

  std::random_device dev_;
  std::mt19937 engine_;
  std::uniform_int_distribution<int> random_w_;
  std::uniform_int_distribution<int> random_h_;

  const int max_attempts_;
  int score_{0};
  bool running_{true};
  std::pair<int, int> food_;

  void PlaceFood();
  void Update();
};
