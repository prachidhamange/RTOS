
#include<stdio.h> 
#include<string.h> 
#include<pthread.h> 
#include<stdlib.h> 
#include<unistd.h> 


pthread_t thread_id[2]; 
int thread_count = 0;

pthread_mutex_t thread_locks[2]; 
pthread_mutex_t main_lock;

char str[100] = "\0";



// Create a co-routine
int routine(void *(*f)())
{
	pthread_create(&(thread_id[thread_count++]), NULL, f, NULL);
	pthread_mutex_lock(&main_lock);
	return thread_count - 1;
}

// Send data to global data
void copyString(int crid, char * msg)
{
	strcpy(str, msg);
	pthread_mutex_unlock(&(thread_locks[crid]));
	pthread_mutex_lock(&main_lock);
}


// Kill a co routine
void kill(int crid)
{
	pthread_cancel(thread_id[crid]);
	pthread_mutex_destroy(&(thread_locks[crid])); 
}

// co routine 1
void* printLoop() 
{ 
	int crid = thread_count - 1;
	
	int i;

	for(i=1;i<=10;i++)
	{
		pthread_mutex_unlock(&main_lock);
		pthread_mutex_lock(&(thread_locks[crid]));
		printf("iteration %d\n", i);  
	}

	return NULL; 
} 

// co routine 2
void* printString() 
{ 
	int crid = thread_count - 1;
	
	while(1)
	{
		pthread_mutex_unlock(&main_lock);
		pthread_mutex_lock(&(thread_locks[crid]));
		if(str[0]=='N'||str[0]=='n')
			continue;		
		printf("Message %s\n", str);  
	}

	return NULL; 
} 


int main() 
{ 	
	// Initialize
	int i = 0; 
	for(i = 0; i < 2; i++)
	{
		pthread_mutex_init(&(thread_locks[i]), NULL);
		pthread_mutex_lock(&(thread_locks[i]));
	}

	pthread_mutex_lock(&main_lock);


	// Create some co routines 
	int cr1 = routine(&printLoop);
	int cr2 = routine(&printString);

	// Execute the threads
	copyString(cr2, "Xoro");
	copyString(cr2, "Yash");
	copyString(cr2, "Nithin");
	copyString(cr1, "C");    //Message doesn't matter for loop functions
	copyString(cr1, "S");
	copyString(cr1, "Q");

	// Kill the routines
	kill(cr1);
	kill(cr2);

	return 0; 
} 
