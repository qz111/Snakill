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
    Rival_Snake(int grid_width, int grid_height)
      : Snake(),
        grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width),
        head_y(grid_height) {_queue=std::make_shared<MessageQueue>();}
    void Update(std::shared_ptr<SDL_Point> food, bool &running);
    bool SnakeCell(const int &x, const int &y);
    std::shared_ptr<MessageQueue> getptr(){return _queue;}
    float head_x;
    float head_y;
    float speed{0.1f};
    int size{1};
    std::vector<SDL_Point> body;
    void GrowBody() { growing = true; }
    bool GetFood(){return foodiseaten;}
    private:
    void UpdateHead(std::shared_ptr<SDL_Point> food);
    void UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell);
    std::shared_ptr<MessageQueue> _queue;
    bool growing{false};
    bool foodiseaten{false};
    int grid_width;
    int grid_height;
};

#endif