#pragma once
#include <utility>
#include <vector>

class Snake {
 public:
  using Point = std::pair<int, int>;
  enum class Direction { kUp, kDown, kLeft, kRight, KNone };

  Snake(int grid_width, int grid_height)
      : grid_width_(grid_width), grid_height_(grid_height), head_({grid_width / 2, grid_height / 2}) {}

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction_ = Direction::kUp;

  int size_{1};
  bool alive_{true};
  Point head_;
  bool ate_food{false};
  Point prevTail_;
  std::vector<Point> body_;

 private:
  void UpdateHead();
  void UpdateBody(Point &current_head, Point &prev_head);

  bool growing_{false};
  int grid_width_;
  int grid_height_;
};
