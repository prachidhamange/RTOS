#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

#include<stdio.h> 
#include<string.h> 
#include<pthread.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>

pthread_t cooking_threads[3]; 
int thread_count= 0;
pthread_mutex_t thread_locks[3];
pthread_mutex_t cook_lock[3];
pthread_mutex_t time_lock;
pthread_mutex_t array_lock; 
pthread_mutex_t create_lock;
int tasks_present[3]={0,0,0};
int arr[3]={0,0,0};

int currt=0;



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


struct stove s[3];//an array of stove
int q[3]={0,0,0}; //keeps track of estimated completion time of last task in queue of a stove
struct queue_node* head[3];
struct queue_node* tail[3];

//checking if deadline can be met for an incoming task 
int check(struct task t,int min,int i)
{
	if(t.prep+min<t.arr+t.dline)
		return 1;
	return 0;
}

//function for creating threads
int create(void *(*f)())
{
	pthread_create(&(cooking_threads[thread_count++]), NULL, f, NULL);
	//printf("im thread %d\n",thread_count-1);		
	return thread_count - 1;
}

//function to add task to the queue of a stove
void add(struct task t,int min,int i)
{
	
	t.ect=t.prep+min;
	struct queue_node *temp=(struct queue_node*)malloc(sizeof(struct queue_node));
	if(head[i]==NULL)
	{
		head[i]=(struct queue_node*)malloc(sizeof(struct queue_node));
		//printf("im in head loop..\n");
		head[i]->prev=NULL;
		head[i]->next=NULL;
		t.stat=1;
		head[i]->t=t;
		printf("Adding....task- id %d, arr: %d, prep: %d, dline: %d \n",head[i]->t.id,head[i]->t.arr,head[i]->t.prep,head[i]->t.dline);			
		tail[i]=head[i];

	}
	else
	{
		t.stat=1;
		temp->t=t;
		printf("Adding....task- id %d, arr: %d, prep: %d, dline: %d \n",temp->t.id,temp->t.arr,temp->t.prep,temp->t.dline);	
		temp->prev=tail[i];
		temp->next=NULL;
		tail[i]->next=temp;
		tail[i]=tail[i]->next;
	}
	tasks_present[i]=tasks_present[i]+1;
	q[i]=t.ect;
	

//function to allocate the next task in the ith stove queue
struct task next(struct stove s[],int i)
{
	
	while(tasks_present[i]==0);
		
	
	struct queue_node *temp=head[i];
	struct task t=head[i]->t;
	head[i]=head[i]->next;
	free(temp);
	printf("dispatching task id %d to stove %d\n",t.id,i);
	return t;
	//tasks_present[i]=tasks_present[i]-1;
}

void* cook_stove()//function which stove threads keep running in a loop, simple functions of a stove
{
	int comp_sleep;//thread should sleep till the order is being cooked,
	int current_order;
	int tid=thread_count-1;
	printf("im thread %d\n",tid);	
	pthread_mutex_unlock(&create_lock);
	//printf("CREATE UNLOCK by cook stove\n");
	while(1){
		printf("Waiting for scheduler to unlock %d\n",tid);
		int k=pthread_mutex_lock(&(thread_locks[tid]));//tasks present and queue is a global variable.
		printf("cook has lock return-%d lock-%d\n",k,tid);
		pthread_mutex_unlock(&cook_lock[tid]);
		//printf("THREAD LOCK %d by cook stove\n",tid);	
		if(tasks_present[tid]>0){
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
		//arr[tid]=arr[tid]+1;
		pthread_mutex_unlock(&(thread_locks[tid]));
		printf("THREAD UNLOCK %d RELEASED by cook stove\n",tid);	
		sleep(comp_sleep);// sleeping signifies one order being cooked and no activity from stove is expected during this
		printf("Order with id %d prepared\n",current_order);
		tasks_present[tid]--;
		//arr[tid]=arr[tid]-1;
		}
}


//,,,,we also have to create threads for each stove and an additional thread that will assign a new order to one of the stoves. 

void allot(struct task t)
{
	//check for an unoccupied stove
	int j;
	//pthread_mutex_lock(&array_lock);
	
	for(int i=0;i<3;i++)
	{
		if(tasks_present[i]==0)
		{
			//assign(s,t,i);
			printf("allottment\n");
			add(t,0,i);
			//tasks_present[i]=tasks_present[i]+1;
			q[i]=t.prep;
			t.stat=0;
			j=i;
			
			int r=pthread_mutex_unlock(&(thread_locks[i]));//as task is added to only one stove we can only let that stove cook
			printf("i unlocked..return-%d. stove active-%d\n ",r,i);
			//sleep(1);
			pthread_mutex_lock(&cook_lock[j]);
			int we=pthread_mutex_lock(&(thread_locks[j]));
			//pthread_mutex_lock(&cook_lock[j]);
			printf("we %d\n",we);
			//printf("THREAD LOCK %d by allot\n",j);			
			break;
		}
		
	}


	if(t.stat==-1)
	{
		//look for the stove that will be available first and check for the deadline condition
		printf("allottment\n");
		int min=q[0],snum=0;
		
		for(int i=0;i<3;i++)
		{
			if(min>q[i])
			{
				min=q[i];
				snum=i;
			}
		}
		if(check(t,min,snum)==1)//check if the task can be finished within the deadline
		{
			//pthread_mutex_lock(&(thread_locks[snum]));	
			add(t,min,snum); //function to add the task in the queue of the ith stove
			t.stat=1;
			
			pthread_mutex_unlock(&(thread_locks[snum]));
			pthread_mutex_lock(&cook_lock[snum]);
			pthread_mutex_lock(&(thread_locks[snum]));
		}


	}
	printf("task with id %d allotted\n",t.id);
	//pthread_mutex_unlock(&array_lock);
	
}


int main()
{
	FILE *file=fopen("order.txt","r");
	int read_array[4];
	//int 
	for(int i=0;i<3;i++)
	{
		
		head[i]=NULL;
		tail[i]=NULL;
		//tasks_present[i]=0;
	}
	
	for(int i = 0; i < 3; i++)
	{
		pthread_mutex_init(&(thread_locks[i]), NULL);
		pthread_mutex_lock(&(thread_locks[i]));
		pthread_mutex_init(&(cook_lock[i]), NULL);
		pthread_mutex_lock(&(cook_lock[i]));
	}
	pthread_mutex_init(&(create_lock), NULL);
	pthread_mutex_lock(&create_lock);	
	int cr1 = create(&cook_stove);
	pthread_mutex_lock(&create_lock);	
	int cr2 = create(&cook_stove);
	pthread_mutex_lock(&create_lock);		
	int cr3 = create(&cook_stove);
	pthread_mutex_lock(&create_lock);			
	//while(1){

	struct task t;
	memset(read_array,0,sizeof(read_array));

	while(!feof(file))
	{
		for(int i=0;i<4;i++)
			fscanf(file,"%d",&read_array[i]);
		printf("Incoming order\n");
		t.id=read_array[0];
		t.arr=read_array[1];
		t.prep=read_array[2];
		t.dline=read_array[3];
		t.stat=-1;
		t.ect=0;
		allot(t);
		pthread_mutex_lock(&time_lock);
		currt=currt+1;
		pthread_mutex_unlock(&time_lock);	
	}
	sleep(8);
	
		
	printf("ALL ORDERS FINISHED\n");
}
