#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include <mutex>
#include <memory>
class Rival_Snake;

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}
  Snake(){}
  void Update(std::shared_ptr<Rival_Snake> rsnake);

  void GrowBody();
  bool SnakeCell(const int &x, const int &y);
  bool CheckTouch(std::shared_ptr<Rival_Snake> rsnake,SDL_Point &current_head_cell);
  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, std::shared_ptr<Rival_Snake> rsnake);
  static std::mutex _mtx;

 private:
  void UpdateHead();
  

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif