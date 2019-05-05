
# KITCHEN ORDER SCHEDULER
## Problem
Every restaurant kitchen receives orders with a particular deadline. This project implements a scheduling algorithm such that-
1. the order is processed within the deadline
2. the order is processed at the earliest

## Solution
Just take a real life scenario - say a billing counter at a super market.
* we first look for an empty counter
* if all counters are busy, we look for the one with the shortest queue 

The only difference is that these orders have a deadline. We can assume that preparation time of an order will be less than the deadline. Hence, if a stove(analogous to counter) is free, we can assign it the order and it will be completed within the deadline. But if the order has to be added to a queue, preferably the shortest queue,(analogous to joing the shortest queue in the billing counter), then we must also check if the order can be completed within the deadline given there is a 'waiting time' also before the order starts getting prepared.

## Implementation
### Structures
We have three structures - task, stove and queue.
....ADD IMAGE
### Threads
We have threads equal to the number of stoves(here 3) running in parallel.
### Locks
.....
### allot()
This function is responsible for assigning orders to one of the stoves. 
Condition to be checked;
....Add image
## Gaps
## Challenges
* There might be cases wherein an order received latest has the earliest deadline. There is a possibility that if a currently being processed order is halted and sent back to the queue, and this order is taken up, it can be delivered within deadline. But then, we have to ensure that all the orders that were already in the queue still can be serviced within the deadline(as now an extra waiting time equal to the preparation time of this emergency order is added).

*  Keeping track of locks
## Extra Info
