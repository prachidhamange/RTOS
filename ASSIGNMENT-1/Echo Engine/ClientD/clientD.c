

#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <time.h>
#include<string.h>
#include<unistd.h>
#define BILLION  1000000000.0;
 
  
// structure for message queue 

struct mesg_text
{
   int cid;//client id
   char data[100];	
};
struct mesg_buffer { 
    long mesg_type; 
    struct mesg_text mesg_text; 
} message; 
  
int main() 
{ 
	key_t key;
	//server MQ id,client MQ id 
	int msgid,cid;

  
	cid=msgget(IPC_PRIVATE,0660);	
    // ftok to generate unique key 
	key = ftok("progfile", 65); 
  
    // msgget creates a message queue and returns identifier 
	msgid = msgget(key, 0666 | IPC_CREAT); 


	char input[100];
	memset(input,'\0',sizeof(char)*100);
	printf("Write Data : ");  
	fgets(input,sizeof(input),stdin);

	while(1)
	{		

		struct timespec start, end;
		
		message.mesg_type = 1; 
		message.mesg_text.cid=cid;
		clock_t t; 
	  
	    // msgsnd to send message 
		for(int i=0;i<100;i++)
	    	message.mesg_text.data[i]=input[i];

		clock_gettime(CLOCK_REALTIME, &start);
		msgsnd(msgid, &message, sizeof(message), 0); 
	  
	    // display the message 
		printf("Data sent is : %s", message.mesg_text.data);
		while(msgrcv(cid, &message, sizeof(message), 1, 0)<=0); 
	  
	    // display the message
		clock_gettime(CLOCK_REALTIME, &end);
		double time_spent = end.tv_nsec - start.tv_nsec;
		printf("Data Received is : %s", message.mesg_text.data); 

		printf("server took %f seconds to execute \n\n", time_spent); 
	}
	msgctl(cid, IPC_RMID, NULL); 

  
    return 0; 
} 

