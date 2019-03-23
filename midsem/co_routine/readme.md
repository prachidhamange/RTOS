#Implementation of co-routine
#Requirements
1. It should be possible to simulate the start and stopping of a co-routine.

2. We should be able to Send data to co-routine with something like handle.send() API (You need to implement this)

3.We should be able to pause and resume the execution of co-routines from a central routine.

4. We should be able to modify and existing routine written in C to a Co-routine pattern

5. We should be able to run and manage multiple coroutines in a single program.

6. Using this concept implement a snake and ladder game with four players. Where each player gets a chance to throw the dice get a random number between 0 and 7 and advance their position to next column the game continues till one of them reaches the max col 100. Have the starting position of snakes and ladders. This again will be simulation of the game with automatic players, each player will be co routine.  Once starts the players will automatically play and game will get over within a specific time. Repeat this game again and again and find out the time taken for each round and tabulate it. Though  snake  and  ladder  is  a very  simple  game  this can  be  the  first  step  towards  modelling  games with timing constraints. 
