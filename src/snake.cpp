#include "snake.h"
#include <cmath>
#include <iostream>
#include "rival_snake.h"

std::mutex Snake::_mtx;
void Snake::Update(std::shared_ptr<Rival_Snake> rsnake) 
{
  SDL_Point prev_cell{
      static_cast<int>(_head_x),
      static_cast<int>(_head_y)};  
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(_head_x),
      static_cast<int>(_head_y)};  

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) 
  {
    UpdateBody(current_cell, prev_cell, rsnake);
  }
}

// This way of update the snake will be used when rsnake is dead. The snake will still be manipulated.
// BUt it can not eat the food anymore.
void Snake::Update()
{
  SDL_Point prev_cell{
      static_cast<int>(_head_x),
      static_cast<int>(_head_y)};  
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(_head_x),
      static_cast<int>(_head_y)};  
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) 
  {
    _body.push_back(std::move(prev_cell));
    _body.erase(_body.begin());
  }
}

void Snake::UpdateHead() 
{
  switch (_direction) {
    case Direction::kUp:
      _head_y -= _speed;
      break;

    case Direction::kDown:
      _head_y += _speed;
      break;

    case Direction::kLeft:
      _head_x -= _speed;
      break;

    case Direction::kRight:
      _head_x += _speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  _head_x = fmod(_head_x + _grid_width, _grid_width);
  _head_y = fmod(_head_y + _grid_height, _grid_height);
}

// Check if the head of the snake touches the body of the rsnake.
// If it touches, then the snake will be dead.
bool Snake::CheckTouch(std::shared_ptr<Rival_Snake> rsnake,SDL_Point &current_head_cell)
{
  for(auto const &item:rsnake->getBody())
  {
    if(abs(current_head_cell.x-item.x)<_speed && abs(current_head_cell.y-item.y)<_speed)
    {
      return true;
    }
  }
  return false;
}

// Update the body of the snake.
// If its head touches the body of the rsnake. It will be dead.
// If its head touches the bodz itself. It will also be dead.
void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell, std::shared_ptr<Rival_Snake> rsnake) 
{
  CheckBody(prev_head_cell);
  std::unique_lock<std::mutex> lck(_mtx);
  if(CheckTouch(rsnake,current_head_cell)) 
  {
    std::cout<<"You touched the body of the rsnake and Lost!"<<std::endl;
    _alive=false;
    return;
  }
  lck.unlock();

  // Check if the snake has died.
  for (auto const &item : _body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) 
    {
      std::cout<<"You touched yourself and Lost!"<<std::endl;
      _alive = false;
    }
  }
}

void Snake::GrowBody() { _growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(const int &x, const int &y) {
  if (x == static_cast<int>(_head_x) && y == static_cast<int>(_head_y)) {
    return true;
  }
  for (auto const &item : _body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

void Snake::CheckBody(SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  _body.push_back(std::move(prev_head_cell));

  if (!_growing) {
    // Remove the tail from the vector.
    _body.erase(_body.begin());
  } else {
    _growing = false;
    _size++;
  }
}