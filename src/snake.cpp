#include "snake.h"
#include <cmath>
#include <iostream>
#include "rival_snake.h"

std::mutex Snake::_mtx;
void Snake::Update(std::shared_ptr<Rival_Snake> rsnake) {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell, rsnake);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}
bool Snake::CheckTouch(std::shared_ptr<Rival_Snake> rsnake,SDL_Point &current_head_cell)
{
  for(auto const &item:rsnake->body)
  {
    if(abs(current_head_cell.x-item.x)<=speed && abs(current_head_cell.y-item.y)<=speed)
    {
      alive=false;
      //std::cout<<"You Lost!"<<std::endl;
      //std::cout<<"hx "<<head_x<<"hy "<<head_y<<"bx"<<item.x<<"by"<<item.y<<std::endl;
      return true;
    }
  }
  return false;
}
void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell, std::shared_ptr<Rival_Snake> rsnake) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }
  std::unique_lock<std::mutex> lck(_mtx);
  if(CheckTouch(rsnake,current_head_cell)) 
  {
    std::cout<<"You Lost!"<<std::endl;
    return;
  }
  lck.unlock();
  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(const int &x, const int &y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}