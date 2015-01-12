#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 5567 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/

#define MAXLINES 1000
#define MAXLEN 1000

int main (int argc, char **argv)
{
  //We need to get the numbers from each respective file first and store them into an integer array

 //Getting the numbers from the file
  char lig1[MAXLINES][MAXLEN];
  FILE *file3;
  //Number of lines tell us how many numbers there are
  int lines3=0;
  file3 = fopen("light.dat", "r");
  //Check to see that file is valid
  if( file3 == NULL)
    return 3;
  //Get the numbers and store them as a string array
  while(fscanf(file3,"%s",lig1[lines3])>0)
    lines3++;
  fclose(file3);
 //Integer array to hold the numbers in integer format
  int lig2[MAXLINES];
  
  //Convert the sorted string numbers into integers
  for(int i=0;i<lines3;i++)
    {
      lig2[i]=atoi(lig1[i]);
    }



  //Getting the numbers from the file
  char temp1[MAXLINES][MAXLEN];
  FILE *file1;
  //Number of lines tell us how many numbers there are
  int lines1=0;
  file1 = fopen("temperature.dat", "r");
  //Check to see that file is valid
  if( file1 == NULL)
    return 1;
  //Get the numbers and store them as a string array
  while(fscanf(file1,"%s",temp1[lines1])>0)
    {
      lines1++;
    }
  fclose(file1);
 //Integer array to hold the numbers in integer format
  int temp2[MAXLINES];
  
  //Convert the sorted string numbers into integers
  for(int i=0;i<lines1;i++)
    {
      temp2[i]=atoi(temp1[i]);
    }



 
  //Getting the numbers from the file
  char hum1[MAXLINES][MAXLEN];
  FILE *file2;
  //Number of lines tell us how many numbers there are
  int lines2=0;
  file2 = fopen("humidity.dat", "r");
  //Check to see that file is valid
  if( file2 == NULL)
    return 2;
  //Get the numbers and store them as a string array
  while(fscanf(file2,"%s",hum1[lines2])>0)
    lines2++;
  fclose(file2);
 
 //Integer array to hold the numbers in integer format
  int hum2[MAXLINES];
  
  //Convert the sorted string numbers into integers
  for(int i=0;i<lines2;i++)
    {
      hum2[i]=atoi(hum1[i]);
    }


  //Variables for the sockets 
  int listenfd, connfd, n;
  pid_t childpid;
  socklen_t clilen;
  char buf[MAXLINE];
  struct sockaddr_in cliaddr, servaddr;
 
  //Create a socket for the soclet
  //If sockfd<0 there was an error in the creation of the socket
  if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
    perror("Problem in creating the socket");
    exit(2);
  }

  
  //preparation of the socket address
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);
  
  //bind the socket
  bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
  
  //listen to the socket by creating a connection queue, then wait for clients
  listen (listenfd, LISTENQ);
  
  printf("%s\n","Server running...waiting for connections.");
  
  //Infinite loop
  for ( ; ; ) {
    
    clilen = sizeof(cliaddr);
    //accept a connection
    connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
  
    printf("%s\n","Received request...");
    
    if ( (childpid = fork ()) == 0 ) {//if it’s 0, it’s child process
      
      //Get the time right when client connects
        time_t then;
	then = time(NULL);


      printf ("%s\n","Child created for dealing with client requests");

      //close listening socket
      close (listenfd);
      
      //Whiel the client sends input
      while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {

	//Find out the time amd the difference since when the client first connected
	time_t now;
	now = time(NULL);
	double diff;
	diff = difftime(now,then);
	int diffi;
	diffi= (int)diff;
	
	
	//If the client wants the temperature
	if(strcmp(buf,"TEMPERATURE")==0)
	  {
	    //Mod the time and send back the correct temprature from the array
	    int modded1;
	    modded1=(diffi%(lines1+1));

	    //String to be passed back to the client
	    char str1[MAXLINE];
	    memset(str1,0,sizeof(MAXLINE));

	    //Change it from an int array to a string array
	    sprintf(str1,"%d",temp2[modded1]);

	    //Send it back to the client
	    send(connfd,str1, sizeof(MAXLINE), 0);
	    //Reset the string array
	    memset(str1,0,sizeof(MAXLINE));
	  }
	

	//Humidity
	if(strcmp(buf,"HUMIDITY")==0)
	  {
	    //Mod the time and send back the appropriate humidity from the array
	    int modded2;
	    modded2=(diffi%(lines2+1));
	    
	    //String to be passed back
	    char str2[MAXLINE];
	    memset(str2,0,sizeof(MAXLINE));
	    
	    //Print the integer into a string array
	    sprintf(str2,"%d",hum2[modded2]);
	    send(connfd,str2, sizeof(MAXLINE), 0);
	    memset(str2,0,sizeof(MAXLINE));
	  }
      
	//Light
	if(strcmp(buf,"LIGHT")==0)
	  {
	    //Mod the time and send back the appropriate light from the array
	    int modded3;
	    modded3=(diffi%(lines3+1));

	    //String to be passed back
	    char str3[MAXLINE];
	    memset(str3,0,sizeof(MAXLINE));
	    sprintf(str3,"%d",lig2[modded3]);
	    send(connfd,str3, sizeof(MAXLINE), 0);
	    memset(str3,0,sizeof(MAXLINE));
	  } 
	//Reset the buffer
	memset(buf,0,sizeof(buf));
      }
      
      
      if (n < 0)
	printf("%s\n", "Read error");
      exit(0);
    }
    
    //close socket of the server
    close(connfd);
 }
}

  
