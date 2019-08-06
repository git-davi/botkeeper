# botKeeper

Project for the course *"Real-Time Embedded Systems"*, (master degree in Computer Engineering, UniMoRe).
The purpose is to create a game that handle events and tasks with a periodic scheduling.


## Dependencies

To play this game you will need :
- g++ compiler
- [Allegro 4](https://liballeg.org) graphic gaming library
- [PTask](https://github.com/glipari/ptask) a Periodic Real-Time Task interface facility to pthreads

*Note : While cloning this repo you will also get the ptask library, so don't worry on installing it!*


# How to install Allegro and ptask

From Linux terminal :
```bash
$ sudo apt-get install liballegro4-dev
```


## How to play

First of all you need to complie the game :
```bash
$ make clean
$ make game
```

Then launch the game :
```bash
$ ./game
```

Now the game should start.
Press space to stop direction and one more time to choose power.

**Enjoy**


## Architecture

[On Work]


## Troubleshooting

If you meet some errors in compilation try :
```bash
$ sudo apt-get install g++
```
