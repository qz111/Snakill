#include "rival_snake.h"
#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>


int MessageQueue::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _condition.wait(uLock,[this]{return !_queue.empty();});
    int msg=std::move(_queue.back());
    _queue.pop_back();
    return msg;
}


void MessageQueue::send(int &&msg)
{
    // The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.clear();
    _queue.push_back(std::move(msg));
    _condition.notify_one();
}

// The rsnake moves with the shortest trajectory towards the food.
void Rival_Snake::UpdateHead(std::shared_ptr<SDL_Point> food)
{
    int food_x=static_cast<int>(food->x);
    int food_y=static_cast<int>(food->y);
    float distance_to_food=sqrt(pow((food_x-_head_x),2)+pow((food_y-_head_y),2));
    if(distance_to_food<_speed)
    {
        _foodiseaten=true;
    }
    else
    {
        _foodiseaten=false;
        _head_y+=(food_y-_head_y)/distance_to_food*_speed;
        _head_x+=(food_x-_head_x)/distance_to_food*_speed;
        _head_x = fmod(_head_x + _grid_width, _grid_width);
        _head_y = fmod(_head_y + _grid_height, _grid_height);
    }
    
}

// The rsnake moves to its right with the current speed if the snake you are controllng is dead
void Rival_Snake::UpdateHead()
{
  _head_x+=_speed;
  _head_x = fmod(_head_x + _grid_width, _grid_width);
}

// Update th rsnake. If rsnake is not dead, then update it in the way which has the shortest line to the food.
// If the speed of the rsnake is 0 then it is dead. The thread will be ended.
void Rival_Snake::Update(std::shared_ptr<SDL_Point> food, bool &running, bool &s_status)
{
    while(running)
    {
        if(_speed<=0.0f) 
        {
          _alive=false;
          std::cout<<"You WIn!!"<<std::endl;
          return;
        }
        // wait for the message from the main thread to keep the same rate with the snake in main thread
        auto msg=_queue->receive();
        SDL_Point prev_cell{
        static_cast<int>(_head_x),
        static_cast<int>(_head_y)};  // We first capture the head's cell before updating.
        if(s_status==false)
        {
          UpdateHead();
          SDL_Point current_cell{
          static_cast<int>(_head_x),
          static_cast<int>(_head_y)};  // Capture the head's cell after updating.

    // Update all of the body vector items if the snake head has moved to a new
    // cell.
          if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) 
          {
              std::unique_lock<std::mutex> lck(_mtx);
              _body.push_back(prev_cell);
              _body.erase(_body.begin());
              lck.unlock();
              std::this_thread::sleep_for(std::chrono::milliseconds(1));
          }

        }
        else
        {
          UpdateHead(food);
          SDL_Point current_cell{
          static_cast<int>(_head_x),
          static_cast<int>(_head_y)};  // Capture the head's cell after updating.

    // Update all of the body vector items if the snake head has moved to a new
    // cell.
          if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) 
          {
              std::unique_lock<std::mutex> lck(_mtx);
              this->CheckBody(prev_cell);
              lck.unlock();
              std::this_thread::sleep_for(std::chrono::milliseconds(1));
          }
        }
    }
}



