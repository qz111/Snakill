#include "game.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>


Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(1, static_cast<int>(grid_width-1)),
      random_h(1, static_cast<int>(grid_height-1)) 
      {
        food=std::make_shared<SDL_Point>();
        PlaceFood();
        rsnake=std::make_shared<Rival_Snake>(grid_width, grid_height);
      }

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  bool checkalive = true;
  SDL_Event e;

  // set a new thread for the rival snake to run
  std::thread t(&Rival_Snake::Update,rsnake,food, std::ref(running), std::ref(checkalive));

  // set a new thread for controller to check the input at any time
  std::thread t1(&Controller::HandleInput,&controller,std::ref(running),std::ref(snake),std::ref(e));

  // main thread set messagequeue send and receive to make the two threads for snake and 
  // rival snake to coorperate with each other
  while (running) {
    frame_start = SDL_GetTicks();

    //communicate with thread rsnake to make the two snakes at the same frequenz
    rsnake->getptr()->send(1);
    Update();
    checkalive = snake.getalive();
    if(foodiseaten)
    {
      PlaceFood();
      foodiseaten=false;
    }
    renderer.Render(snake, rsnake, *food);
    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  // wait until the other threads finished
  rsnake->getptr()->send(1);
  t.join();
  t1.join();
}

void Game::PlaceFood() 
{
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!(snake.SnakeCell(x, y))) {
      food->x = x;
      food->y = y;
      return;
    }
  }
}

void Game::Update() 
{
  if (!snake.getalive()) 
  {
    return;
  }
  else if(!rsnake->getalive())
  {
    // if rsnake is dead, the snake you are controlling can also move and manipulated.
    // But it can not eat the food any more.
    snake.Update();
  }
  else
  {
    snake.Update(rsnake);

    int new_x = static_cast<int>(snake.getHeadx());
    int new_y = static_cast<int>(snake.getHeady());
    // Check if there's food over here
    if (food->x == new_x && food->y == new_y) {
      score++;
      foodiseaten=true;
      // Grow snake and increase speed.
      // Decrease the speed of the rsnake.
      snake.GrowBody();
      float sspeed=snake.getSpeed();
      float rspeed=rsnake->getSpeed();
      sspeed += 0.01;
      rspeed-=0.01;
      snake.setSpeed(std::move(sspeed));
      rsnake->setSpeed(std::move(rspeed));
    }
    // Check if the rsnake get the food. If so grow the body.
    else if(rsnake->GetFood())
    {
      foodiseaten=true;
      rsnake->GrowBody();
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.getSize(); }