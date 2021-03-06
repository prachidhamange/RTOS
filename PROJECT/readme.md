
# KITCHEN ORDER SCHEDULER
## Problem
Every restaurant kitchen receives orders with a particular deadline. The success of any kitchen in such a scenario is entirely dependent on effective processing of incoming orders. This project implements a scheduling system such that-
1. the order is processed within the deadline
2. the order is processed efficiently
wherein the kitchen has multiple stoves(3 in this case) and the ordering system recieves order from either a file or a client-server based system. In the latter case the system simulating a kitchen resides in server source code.

## Solution
Just take a real life scenario - say a billing counter at a super market.
* we first look for an empty counter
* if all counters are busy, we look for the one with the shortest queue 

The only difference is that these orders have a deadline. We can assume that preparation time of an order will be less than the deadline. Hence, if a stove(analogous to counter) is free, we can assign it the order and it will be completed within the deadline. But if the order has to be added to a queue, preferably the shortest queue,(analogous to joing the shortest queue in the billing counter), then we must also check if the order can be completed within the deadline given there is a 'waiting time' also before the order starts getting prepared.

## Implementation
### Structures
We have three structures - task, stove and queue_node.

```c
struct task
{

	int id;
	int stat;
	int arr;
	int prep;
	int dline;
	int ect;

};

struct stove
{
	int status;
	struct task t;
};

struct queue_node
{

	struct queue_node* prev;
	struct queue_node* next;
	struct task t;
};
```


### Threads
We want threads in order to replicate the live functioning of a stove. There are as many threads as number of stoves in the kitchen. This thread is responsible to process the orders, and once the order is prepared, request for the next order waiting in its queue. In case, the queue is empty, it will wait indefinitely until a new order is added to the queue.
```c
void* cook_stove()//function which stove threads keep running in a loop, simple functions of a stove
{
	int comp_sleep;//thread should sleep till the order is being cooked,
	int current_order;
	int tid=thread_count-1;
	printf("im thread %d\n",tid);	
	pthread_mutex_unlock(&create_lock);
	while(1){
		printf("Waiting for scheduler to unlock %d\n",tid);
		int k=pthread_mutex_lock(&(thread_locks[tid]));//tasks present and queue is a global variable.
		printf("cook has lock return-%d lock-%d\n",k,tid);
		pthread_mutex_unlock(&cook_lock[tid]);
		//printf("THREAD LOCK %d by cook stove\n",tid);	
		if(tasks_present[tid]>0)
			{
			s[tid].status=1;
			s[tid].t=next(s,tid);//queue node to be replaced by queue_node type variable
			s[tid].t.stat=0;
			pthread_mutex_lock(&time_lock);
			s[tid].t.ect=currt+s[tid].t.prep;
			pthread_mutex_unlock(&time_lock);
			comp_sleep=s[tid].t.prep;			
			current_order=s[tid].t.id;
			}
		printf("Preparing order with id : %d\n",current_order);
		pthread_mutex_unlock(&(thread_locks[tid]));
		printf("THREAD UNLOCK %d RELEASED by cook stove\n",tid);	
		sleep(comp_sleep);// sleeping signifies one order being cooked and no activity from stove is expected during this
		printf("Order with id %d prepared\n",current_order);
		tasks_present[tid]--;
		}
}

```



### Locks
We have two different kinds of locks here, the first kind are thread locks which block threads(3, analogous to stoves) from accesing or modifying the same resources which the scheduler uses. Intuitively one can tell that these resources would constitute a scheduling queue. In our construct each stove or cooking station has its own queue which stores the order of food orders to be prepared. Each stove can delete orders from its queue after taking them up for preparation. These queues are populated by the scheduler based on rules which we think would lead to efficient utilization and earliest deliveries.

The second kind of lock is a time lock which has been kept to ensure that there are no simultaenous access/modifications to a real clock counter leading to a race condition.
### allot()
This function is fundamental to our code as it encompasses the implementation for the scheduler.This function is responsible for assigning orders to one of the stoves. 

Condition to be checked;

![API](rtos3.png)

  * For the case when queue is empty, **T(available)=0**.
  * For the case when queue has orders waiting, **T(available)= estimated completion time of the last order in the queue**.

## Gaps
* There might be cases wherein an order received latest has the earliest deadline. There is a possibility that if a currently being processed order is halted and sent back to the queue, and this order is taken up, it can be delivered within deadline. But then, we have to ensure that all the orders that were already in the queue still can be serviced within the deadline(as now an extra waiting time equal to the preparation time of this emergency order is added). This makes the code complex.

## Challenges
*  Handling synchronization using locks can get confusing.
## Extra Info
* We can extend it further to a server-client model using TCP/IP sockets. We can send only the relevant information to the client. 
