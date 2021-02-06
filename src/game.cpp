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
      random_h(1, static_cast<int>(grid_height-1)) {
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
  SnakeStatus s_status=SnakeStatus::Allalive;
  SDL_Event e;
  std::thread t(&Rival_Snake::Update,rsnake,food, std::ref(running), std::ref(s_status));
  std::thread t1(&Controller::HandleInput,&controller,std::ref(running),std::ref(snake),std::ref(e));
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    //controller.HandleInput(running, snake);
    rsnake->getptr()->send(1);
    Update(s_status);
    
    if(foodiseaten)
    {
      //std::cout<<"it is here"<<std::endl;
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
  rsnake->getptr()->send(1);
  t.join();
  t1.join();
}

void Game::PlaceFood() {
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

void Game::Update(SnakeStatus &s_status) {
  if (!snake.alive) 
  {
    s_status=SnakeStatus::sDead;
    return;
  }
  else if(s_status==SnakeStatus::rDead)
  {
    snake.Update();
  }
  else
  {
    snake.Update();

    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);
    // Check if there's food over here
    if (food->x == new_x && food->y == new_y) {
      score++;
      foodiseaten=true;
      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.01;
      rsnake->speed-=0.02;
    }
    else if(rsnake->GetFood())
    {
      foodiseaten=true;
      rsnake->GrowBody();
      //std::cout<<"it is here 1"<<std::endl;
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }