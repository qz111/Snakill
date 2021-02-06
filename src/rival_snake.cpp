#include "rival_snake.h"
#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>

//template <typename T>
int MessageQueue::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _condition.wait(uLock,[this]{return !_queue.empty();});
    int msg=std::move(_queue.back());
    _queue.pop_back();
    return msg;
}

//template <typename T>
void MessageQueue::send(int &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.clear();
    _queue.push_back(std::move(msg));
    _condition.notify_one();
}


void Rival_Snake::UpdateHead(std::shared_ptr<SDL_Point> food)
{
    int food_x=static_cast<int>(food->x);
    int food_y=static_cast<int>(food->y);
    float distance_to_food=sqrt(pow((food_x-head_x),2)+pow((food_y-head_y),2));
    //std::cout<<"headx "<<head_x<<"heady "<<head_y<<"fx "<<food_x<<"fy "<<food_y<<std::endl;
    if(distance_to_food<speed)
    {
        foodiseaten=true;
    }
    else
    {
        foodiseaten=false;
        
        head_y+=(food_y-head_y)/distance_to_food*speed;
        head_x+=(food_x-head_x)/distance_to_food*speed;
        head_x = fmod(head_x + grid_width, grid_width);
        head_y = fmod(head_y + grid_height, grid_height);
    }
    
}

void Rival_Snake::Update(std::shared_ptr<SDL_Point> food, bool &running)
{
    while(running)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto msg=_queue->receive();
        SDL_Point prev_cell{
        static_cast<int>(head_x),
        static_cast<int>(head_y)};  // We first capture the head's cell before updating.
        UpdateHead(food);
        SDL_Point current_cell{
        static_cast<int>(head_x),
        static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
        if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) 
        {
            this->UpdateBody(current_cell, prev_cell);
        }
        
    }
    std::cout<<"running in snake "<<running<<std::endl;
}

void Rival_Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }
}

bool Rival_Snake::SnakeCell(const int &x, const int &y) {
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

