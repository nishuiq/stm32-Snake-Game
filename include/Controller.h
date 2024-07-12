#pragma once
#include <queue>
#include "config.h"
#include "snake.h"

class Controller {
 public:
  void HandleInput(Snake &snake) const;

  static void HandleK2Press();
  static void HandleK3Press();

 private:
  static volatile Snake::Direction lastInputDirection_;
  void ChangeDirection(Snake &snake) const;
};
