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
      : _grid_width(grid_width),
        _grid_height(grid_height),
        _head_x(grid_width / 2),
        _head_y(grid_height / 2) {}
  Snake(){}
  void Update(std::shared_ptr<Rival_Snake> rsnake);
  void Update();
  void GrowBody();
  bool SnakeCell(const int &x, const int &y);
  void setSpeed(float &&s){_speed=s;}
  void setDirection(Direction d) {_direction=d;}
  Direction getDirection() const {return _direction;}
  float getSpeed() const {return _speed;}
  float getHeadx() const {return _head_x;}
  float getHeady() const {return _head_y;}
  bool getalive() const {return _alive;}
  int getSize() const {return _size;}
  std::vector<SDL_Point> getBody() const {return _body;}
  int getGridw(){return _grid_height;}

 protected:
  Direction _direction = Direction::kUp;
  void CheckBody(SDL_Point &prev_head_cell);
  float _speed{0.1f};
  int _size{1};
  
  float _head_x;
  float _head_y;
  std::vector<SDL_Point> _body;
  bool _growing{false};
  int _grid_width;
  int _grid_height;
  bool _alive{true};
  static std::mutex _mtx;
 private:
  
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell, std::shared_ptr<Rival_Snake> rsnake);
  void UpdateHead();
  bool CheckTouch(std::shared_ptr<Rival_Snake> rsnake,SDL_Point &current_head_cell);
  
};

#endif