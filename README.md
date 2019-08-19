# botKeeper

Project for the course *"Real-Time Embedded Systems"*, (master degree in Computer Engineering, UniMoRe).
The purpose is to create a game that handle events and tasks with a periodic scheduling.


## Dependencies

To play this game you will need :
- g++ compiler
- [Allegro 4](https://liballeg.org) graphic gaming library
- [PTask](https://github.com/glipari/ptask) a Periodic Real-Time Task interface facility to pthreads

*Note : While cloning this repo you will also get the ptask library, so don't worry on installing it!*


## How to install Allegro and ptask

From Linux terminal :
```bash
$ sudo apt-get install liballegro4-dev
```


## How to play

First of all you need to compile the game :
```bash
$ make clean
$ make game
```

Then launch the game :
```bash
$ ./game
```

Now the game should start.
Press **space** to stop direction and one more time to choose power.
**Esc** to quit the game.

**Enjoy**


## Architecture

This is the architecture of our project. It displays the interactions between
the tasks and the resource.
τ stands for task (yellow circle), and S stand for state, or resource, (green rectangle).
- τ_palla -> task that update the position and the speed of the ball.
- τ_port -> task for the goalkeeper, it updates his position based on the ball position,
we also implemented a sort of prediction for better block.
- τ_frec -> updates the rotation of arrow that stores the shot direction.
- τ_pow -> updates the indicator of the power bar on the lower left corner.
- τ_user -> waits for the ball to stop. Then the user can press space two times and
the task will store the of S_frec and S_pow in the **ball state**.
- τ_grafico -> The task that periodically reads the info of each state and draws on screen.

![](!img/architecture.png)

## Troubleshooting

If you meet some errors in compilation try :
```bash
$ sudo apt-get install g++
```
