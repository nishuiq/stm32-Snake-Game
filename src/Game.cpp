#include "Game.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake_(grid_width, grid_height),
      engine_(dev_()),
      random_w_(0, static_cast<int>(grid_width - 1)),
      random_h_(0, static_cast<int>(0, grid_height - 1)),
      max_attempts_(grid_width * grid_height * 2) {
  PlaceFood();
}

void Game::Run(Controller &controller, Renderer &renderer, std::size_t target_frame_duration) {
  uint32_t title_timestamp;
  uint32_t frame_start;
  uint32_t frame_end;
  uint32_t frame_duration;
  int frame_count{0};

  renderer.showLoading();

  while (running_) {
    frame_start = millis();

    // Input - Update - Render
    controller.HandleInput(snake_);
    Update();
    renderer.Render(snake_, food_);

    frame_end = millis();

    frame_count++;
    frame_duration = frame_end - frame_start;
    // Serial.printf("frame_duration: %d\n", frame_duration);

    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateTitle(score_, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    if (frame_duration < target_frame_duration) {
      delay(target_frame_duration - frame_duration);
    }
  }

  renderer.UpdateTitle("Score: " + std::to_string(score_));
}

void Game::PlaceFood() {
  int x, y;
  for (int i = 0; i < max_attempts_; ++i) {
    x = random_w_(engine_);
    y = random_h_(engine_);
    if (!snake_.SnakeCell(x, y)) {
      food_ = {x, y};
      return;
    }
  }
  running_ = false;
}

void Game::Update() {
  if (!snake_.alive_) {
    running_ = false;
    return;
  }

  snake_.Update();
  if (food_ == snake_.head_) {
    PlaceFood();
    snake_.GrowBody();
    score_++;
  }
}

int Game::GetScore() const { return score_; }
int Game::GetSize() const { return snake_.size_; }
