#pragma once
#include <TFT.h>
#include <config.h>
#include <map>
#include <queue>
#include <random>

typedef std::pair<int, int> PII;

class Snake
{
public:
  Snake() = default;
  ~Snake() = default;

  /**
   * @brief init important data value one time
   * @param TFTscreen* store the pointer
   */
  void init(TFT *TFTscreen);

  /**
   * @brief Get ready for the game
   * Reset game data...
   */
  void start();

  /**
   * @brief create food {x,y}
   *
   */
  void genered_food();

  /**
   * @brief Game Core.
   * @param key_state key_press direction
   */
  void go(SNAKE_TOWARD key_state);

  /**
   * @brief draw point
   * @param x x axist
   * @param y y axist
   * @param type draw head_body = 0 -> GREEN, food = 1 -> BLUE, -1 erase draw
   */
  void draw(int x, int y, int type);

  bool is_over() { return is_over_; }

  bool is_win() { return score_ == WIDTH_SIZE * HEIGHT_SIZE - 1; }

  /**
   * @brief True if legal moving.
   * False if illegal moving.
   */
  bool check_rule(int x, int y)
  {
    if (x < 0 || y < 0 || x >= WIDTH_SIZE || y >= HEIGHT_SIZE || map_[{x, y}])
    {
      return false;
    }
    return true;
  }

  /**
   * @brief head{x,y}, food{x,y}, snake direction
   */
  struct SnakeEntity
  {
    PII head_;
    PII food_;
    int direction_;
  };

private:
  /* TFT */
  TFT *TFTscreen_;
  /* game dataset */
  bool is_over_;
  /* check position T/F empty */
  std::map<PII, bool> map_;
  /* get tail snake (x,y) */
  std::queue<PII> q_;
  /* Sanke data */
  SnakeEntity snake_entity_;
  /* score */
  int score_{0};

  /** snake offset
   * 转向 0左 1右 2不变
   * diff_[蛇头方向][转向]{x,y坐标值}
   * snake_direction_ = {0,1,2,3};  // 0 向↓ 1 向↑ 2向← 3向→
   */
  const int diff_[4][3][2] = {
      {{1, 0}, {-1, 0}, {0, 1}},
      {{-1, 0}, {1, 0}, {0, -1}},
      {{0, 1}, {0, -1}, {-1, 0}},
      {{0, -1}, {0, 1}, {1, 0}}};

  /**
   * 转向后蛇头变化矩阵
   * snacke_direction_next[蛇头状态][转向] --> [得到下一个蛇头状态]
   * [4][] 0左 1右 2不变
   */
  const int snack_toward_[4][3] = {
      {3, 2, 0},
      {2, 3, 1},
      {0, 1, 2},
      {1, 0, 3}};

  /* random */
  std::random_device device_;
  std::uniform_int_distribution<int> unitwidth_;
  std::uniform_int_distribution<int> unitheight_;
};

void Snake::init(TFT *TFTscreen)
{
    /* init TFT* */
    TFTscreen_ = TFTscreen;
    /* init random */
    unitwidth_ = std::uniform_int_distribution<int>(0, WIDTH_SIZE - 1);
    unitheight_ = std::uniform_int_distribution<int>(0, HEIGHT_SIZE - 1);
}

void Snake::start()
{
  /* clean screen */
  TFTscreen_->background(0, 0, 0);
  /* reset data */
  score_ = 0;
  map_.clear();
  is_over_ = false;
  snake_entity_.direction_ = 0;
  snake_entity_.head_ = {WIDTH_SIZE / 2, HEIGHT_SIZE / 2};

  q_ = std::queue<PII>();
  q_.push(snake_entity_.head_);
  map_[snake_entity_.head_] = true;
  draw(snake_entity_.head_.first, snake_entity_.head_.second, 0);

  genered_food();
}

void Snake::genered_food()
{
  /* random */
  for (int miss = 0; miss < 2000; ++miss) {
    int x = unitwidth_(device_);
    int y = unitheight_(device_);
    if (!map_[{x, y}]) {
      snake_entity_.food_ = {x, y};
      draw(x, y, 1);
      return;
    }
  }
  /* for choice */
  for (int i = 0; i < WIDTH; ++i) {
    for (int j = 0; j < HEIGTH; ++j) {
      if (!map_[{i, j}]) {
        snake_entity_.food_ = {i, j};
        draw(i, j, 1);
        return;
      }
    }
  }
}

void Snake::go(SNAKE_TOWARD key_state)
{
  // noInterrupts();
  /* just go */
  int x = snake_entity_.head_.first + diff_[snake_entity_.direction_][key_state][0];
  int y = snake_entity_.head_.second + diff_[snake_entity_.direction_][key_state][1];
  /**
   * @bug 当蛇头碰到原先的蛇尾位置时，会判断出错，因为蛇尾此时还未移动
   * 修复方案：先将蛇尾 map_[{x,y}] = false, 如果没有吃到食物，那就消除该点屏幕；如果吃到，重新置为true，不需要消除屏幕
   * 同时改进 map 的擦除，因为我们是 20x16 映射的小地图了，不要erase
  */

  auto t = q_.front();
  map_[t] = false;  // 蛇尾先去掉标识

  if (!check_rule(x, y)) {
    is_over_ = true;
  } else {
    auto xy = PII(x, y);
    /* change snake head direction */
    snake_entity_.direction_ = snack_toward_[snake_entity_.direction_][key_state];
    /* draw snake new head_body */
    q_.push(xy);
    map_[xy] = true;
    snake_entity_.head_ = xy;
    draw(x, y, 0);

    /* erase snake tail */
    if (xy != snake_entity_.food_) {
      draw(t.first, t.second, -1);
      // map_[t] = false;    // | improve
      q_.pop();
    } else {
      ++score_;
      map_[t] = true;  // 如果吃到食物，蛇尾不擦除
      genered_food();
    }
  }
  // interrupts();
}

void Snake::draw(int x, int y, int type)
{
  x *= body_x;
  y *= body_y;
  /* Erase old point. */
  if (type == -1) {
    TFTscreen_->stroke(0, 0, 0); // WHITE
    TFTscreen_->fill(0, 0, 0);
  } else if (type == 0) {
    /* snake body */
    TFTscreen_->stroke(0, 139, 0); // GREEN
    TFTscreen_->fill(0, 139, 0);
  } else if (type == 1) {
    /* food */
    TFTscreen_->stroke(255, 0, 0); // RED
    TFTscreen_->fill(255, 0, 0);
  }
  TFTscreen_->rect(x, y, body_x, body_y);
}

/*
  由于我们只有2个按键，我们选取两个按键作为蛇头的转向
  注意，我们TFT里是向右为x轴，向下为y轴 160x128
  我们让左手边为0，右手边为1
       0       1
  1↓0  →  0↑1  ←
       1       0

  在画图的时候，point(x,y) 是在坐标轴 x向右，y向下，来进行画图的
向下   左转|右转
  [0] -> [0] -> (x+1, y) = (+1, 0)  ==> [3] 向右
  [0] -> [1] -> (x-1, y) = (-1, 0)  ==> [2] 向左
向上   左转|右转
  [1] -> [0] -> (x-1, y) = (-1, 0)  ==> [2] 向左
  [1] -> [1] -> (x+1, y) = (+1, 0)  ==> [3] 向右
向左   左转|右转
  [2] -> [0] -> (x, y+1) = (0, +1)  ==> [0] 向下
  [2] -> [1] -> (x, y-1) = (0, -1)  ==> [1] 向上
向右   左转|右转
  [3] -> [0] -> (x, y-1) = (0, -1)  ==> [1] 向上
  [3] -> [1] -> (x, y+1) = (0, +1)  ==> [0] 向下


snake_direction_ = {0,1,2,3};  // 0 向↓ 1 向↑ 2向← 3向→
diff_[蛇头状态][转向][x,y坐标值]。
对转向再加一维表示蛇头方向不变，具体看class里说明 diff_[4][3][2]
const int diff_[4][2][2] = {
  {{1,0},{-1,0}},
  {{-1,0},{1,0}},
  {{0,1},{0,-1}},
  {{0,-1},{0,1}}
};

转向后蛇头变化矩阵
snacke_direction_next[蛇头状态][转向] --> [得到下一个蛇头状态]。
对转向再加一维表示蛇头方向不变，具体看class里说明 snack_toward[4][3]
const int snack_toward[4][2] = {
  {3, 2},
  {2, 3},
  {0, 1},
  {1, 0}
};
*/
