# Game: Snakill

This game is developed from the classic game Greedy Snake. Except the traditional functions it has some new features.
A new Snake called Rival_Snake is added into the game. You should compete with this rival snake to eat the food by manipulating your good snake with the input UP, DOWN, LEFT and RIGHT. Both snakes start with the same speed. Every time your snake eat the food, it will be quicker and your rival will be slower. When your rival's speed is 0 and can not move any more, then you win. But do not touch the body of the rival snake. because it is toxic. If the head of your snake touches the body of the rival snake, then you lost. And do not touch the body youselves. If so, you lost. DO not give any chance to your rival. The longer the rival snake is, the more dangerous you are. SO get fight for it!

The following rubric points are fulfilled in the code.
* All class data members are explicitly specified as public, protected, or private.
 * It is covered in every .h file.
* All class members that are set to argument values are initialized through member initialization lists.
 * It is covered in snake.h
* Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.
 * It is covered in snake.cpp and rival_snake.cpp. There are getter and setter funtions to access the member data.
* One function is overloaded with different signatures for the same function name.
 * It is covered in snake.h at line 20 and 21. It is also covered in rival_snake.h at line 45 and 46.
* At least two variables are defined as references, or two functions use pass-by-reference in the project code.
 * It is covered in snake.h, rival_snake at line 40 and also in renderer.h at line 15, game.h at line 14 and controller.h at line 9 and 12. 
* For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object.
 * It is covered in snake.cpp at lines 38, 124 and in rival_snake.cpp at lines 12, 24, 85.
* The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.
 * It is covered in rival_snake.h, snake.h, gmae.h, renderer.h.
* The project uses multiple threads in the execution.
 * It is covered in game.cpp at lines 34,37. One for controller and one for rsnake.
* A mutex or lock (e.g. std::lock_guard or std::unique_lock) is used to protect data that is shared across multiple threads in the project code.
 * It is covered in rival_snake.cpp at lines 10 and 22. And also at lines 85, 103 and in snake.cpp at line 88 to protect the body update of the rsnake.
* A std::condition_variable is used in the project code to synchronize thread execution.
 * It is covered in rival_snake.h at line 21 to synchronize the thread rsnake and the main thread.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.
