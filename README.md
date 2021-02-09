# Game: Snakill

This game is developed from the classic game Greedy Snake. Except the traditional functions it has some new features.
A new Snake called Rival_Snake is added into the game. You should compete with this rival snake to eat the food by manipulating your good snake with the input UP, DOWN, LEFT and RIGHT. Both snakes start with the same speed. Every time your snake eat the food, it will be quicker and your rival will be slower. When your rival's speed is 0 and can not move any more, then you win. But do not touch the body of the rival snake. because it is toxic. If the head of your snake touches the body of the rival snake, then you lost. And do not touch the body youselves. If so, you lost. DO not give any chance to your rival. The longer the rival snake is, the more dangerous you are. SO get fight for it!

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