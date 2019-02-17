#include <stdio.h> 
#include <sys/ipc.h> 
#include<unistd.h>
#include <sys/msg.h> 
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
  
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
  
	while(1)
	    {
	    	   // msgrcv to receive message 
		while(msgrcv(msgid, &message, sizeof(message), 1, 0)<=0); 
		  
		    // display the message 
		printf("Data Received is : %s \n",  
				    message.mesg_text.data); 
		    //extract the filename i.e ignore 'GET'	
		char filename[10];
		memset(filename,'\0',sizeof(char)*10); 
		int i;
		int count=0;		    
		for(i=4;i<14;i++)
		{
			if(message.mesg_text.data[i]!='\n')
				filename[i-4]=message.mesg_text.data[i];
		
			else
				break;
		}
		FILE *fptr;
		char ch;

		    /*  open the file for reading */
		fptr = fopen(filename, "r");
		memset(message.mesg_text.data,'\0',sizeof(char)*100); 
		if (fptr == NULL)
		{
			char reply[30]="data not found\n";
			for(int i=0;i<30;i++)
				message.mesg_text.data[i]=reply[i];

		}
		else
		{
			    	
			ch = fgetc(fptr);
			int i=0;
			while (ch != EOF)
			{

				message.mesg_text.data[i]=ch;
				ch = fgetc(fptr);
				i++;

			}

			fclose(fptr);
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
