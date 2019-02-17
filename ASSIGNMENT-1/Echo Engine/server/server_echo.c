#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
  
// structure for message queue 

struct mesg_text
{
   int cid;
   char data[100];	
};
struct mesg_buffer { 
    long mesg_type; 
    struct mesg_text mesg_text; 
} message; 
  
int main() 
{ 
    key_t key; 
    int msgid; 
  
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
  
    // msgrcv to receive message 
    while(1)
    {
	    while(msgrcv(msgid, &message, sizeof(message), 1, 0)<=0); 
	  
	    // display the message 
	    printf("Data Received is : %s \n",  
		            message.mesg_text.data); 
	    
	    for(int i=0;i<100;i++)
	    {
		if(message.mesg_text.data[i]>='a'&&message.mesg_text.data[i]<='z')
			message.mesg_text.data[i]=message.mesg_text.data[i]-32;
		else if(message.mesg_text.data[i]>='A'&&message.mesg_text.data[i]<='Z')
			message.mesg_text.data[i]=message.mesg_text.data[i]+32;
	    }
			
	    printf("Data sent is : %s \n", message.mesg_text.data);  
            int cid=message.mesg_text.cid;
	    message.mesg_text.cid=msgid;
	    msgsnd(cid, &message, sizeof(message), 0); 
    }
   
    // to destroy the message queue 
    //msgctl(msgid, IPC_RMID, NULL); 
  
    return 0; 
} 
