#include "Controller.h"

volatile Snake::Direction Controller::lastInputDirection_ = Snake::Direction::KNone;

void Controller::HandleInput(Snake &snake) const {
  if (lastInputDirection_ == Snake::Direction::KNone) {
    return;
  }
  ChangeDirection(snake);
}

void Controller::ChangeDirection(Snake &snake) const {
  auto &input = lastInputDirection_;
  switch (snake.direction_) {
    case Snake::Direction::kUp:
      snake.direction_ = input;
      break;
    case Snake::Direction::kDown:
      snake.direction_ = (input == Snake::Direction::kRight ? Snake::Direction::kLeft : Snake::Direction::kRight);
      break;
    case Snake::Direction::kLeft:
      snake.direction_ = (input == Snake::Direction::kLeft ? Snake::Direction::kDown : Snake::Direction::kUp);
      break;
    case Snake::Direction::kRight:
      snake.direction_ = (input == Snake::Direction::kLeft ? Snake::Direction::kUp : Snake::Direction::kDown);
      break;
  }
  input = Snake::Direction::KNone;
}

void Controller::HandleK2Press() { lastInputDirection_ = Snake::Direction::kLeft; }
void Controller::HandleK3Press() { lastInputDirection_ = Snake::Direction::kRight; }
