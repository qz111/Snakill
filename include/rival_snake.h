#ifndef RIVAL_SNAKE_H
#define RIVAL_SNAKE_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include "snake.h"
#include "SDL.h"
#include <future>
#include <memory>

//template <class T>
class MessageQueue
{
public:
int receive();
void send(int &&msg);
MessageQueue(){}
private:
std::vector<int> _queue;
std::condition_variable _condition;
std::mutex _mutex;
};

class Rival_Snake : public Snake
{
    public:
    Rival_Snake(int grid_width, int grid_height):Snake()
    {
      _grid_width=grid_width;
      _grid_height=grid_height;
      _head_x=grid_width;
      _head_y=grid_height;
      _queue=std::make_shared<MessageQueue>();
      _speed=0.1f;
      _size=1;
      _growing=false;
      _alive=true;
    }
    void Update(std::shared_ptr<SDL_Point> food, bool &running, bool &s_status);
    std::shared_ptr<MessageQueue> getptr(){return _queue;}
    bool GetFood(){return _foodiseaten;}

    private:
    void UpdateHead(std::shared_ptr<SDL_Point> food);
    void UpdateHead();
    std::shared_ptr<MessageQueue> _queue;
    bool _foodiseaten{false};   
};

#endif