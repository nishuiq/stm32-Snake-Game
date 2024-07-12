#include "snake.h"

void Snake::Update() {
  Point prev_cell(head_);
  UpdateHead();
  // Point current_head(head_);
  UpdateBody(head_, prev_cell);
}

void Snake::UpdateHead() {
  // 需要注意，后续使用到TFT，坐标轴是x向右，y向下
  switch (direction_) {
    case Direction::kUp:
      head_.second -= 1;
      break;
    case Direction::kDown:
      head_.second += 1;
      break;
    case Direction::kLeft:
      head_.first -= 1;
      break;
    case Direction::kRight:
      head_.first += 1;
      break;
  }

  // 跨越地图边界
  head_.first = (head_.first + grid_width_) % grid_width_;
  head_.second = (head_.second + grid_height_) % grid_height_;
}

void Snake::UpdateBody(Point &current_cell, Point &prev_cell) {
  body_.push_back(prev_cell);
  if (!growing_) {
    prevTail_ = body_.front();
    body_.erase(body_.begin());
  } else {
    size_++;
    growing_ = false;
    ate_food = true;
  }

  // 检查蛇头是否碰撞到蛇身
  for (auto const &item : body_) {
    if (item == current_cell) {
      alive_ = false;
      return;
    }
  }
}

void Snake::GrowBody() { growing_ = true; }

// 检查是否是蛇的坐标
bool Snake::SnakeCell(int x, int y) {
  if (head_.first == x && head_.second == y) {
    return true;
  }
  for (auto const &item : body_) {
    if (item.first == x && item.second == y) {
      return true;
    }
  }
  return false;
}
