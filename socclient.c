/*

File: socclient.c

Description: Client side to send inputs to the server and take in the response from the server and print out the data from the server. The client should connect to the server, stay connected, and retrieve a number of readings. 

Input: IP address of Server

Output: Readings returned from the server

 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


#define MAXLINE 1000 /*max text line length*/
#define SERV_PORT 5567 /*port*/
#define ITER 20

int main(int argc, char **argv)
{
 int sockfd;
 struct sockaddr_in servaddr;

 //The inptus to be sent to the server
 char sendline1[MAXLINE]={"TEMPERATURE"};
 char sendline2[MAXLINE]={"HUMIDITY"};
 char sendline3[MAXLINE]={"LIGHT"};
 char recvline[MAXLINE];

 //basic check of the arguments
 //additional checks can be inserted
 if (argc !=2) {
  perror("Usage: TCPClient <IP address of the server");
  exit(1);
 }

 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }

 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[1]);
 servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order

 //Connection of the client to the socket
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }


 //Run it 20 times
 for(int i=0; i<ITER; i++)
   {
     printf("Data returned by the server:\n");

     //Send the inputs
     send(sockfd, sendline1, strlen(sendline1), 0);
     
     if (recv(sockfd, recvline, MAXLINE,0) == 0){
       //error: server terminated prematurely
       perror("The server terminated prematurely");
       exit(4);
     }
     
     //Print out what the server returns
     printf("TEMPERATURE= %s", recvline);

     //Reset the receive line
     memset(recvline,0,sizeof(recvline));
     
     //Send the Humidity line
     send(sockfd, sendline2, strlen(sendline2), 0);
     
     if (recv(sockfd, recvline, MAXLINE,0) == 0){
       //error: server terminated prematurely
       perror("The server terminated prematurely");
       exit(4);
     }
     
     //Print out the return
     printf(" HUMIDITY= %s", recvline);

     //Reset
     memset(recvline,0,sizeof(recvline));
     
     //Light input
     send(sockfd, sendline3, strlen(sendline3), 0);
     
     if (recv(sockfd, recvline, MAXLINE,0) == 0){
       //error: server terminated prematurely
       perror("The server terminated prematurely");
       exit(4);
     }
     
     //Print the return value
     printf(" LIGHT= %s", recvline);
     memset(recvline,0,sizeof(recvline));


     printf("\n");
     
     //Sleep
     sleep(1);
   
   }    
 close(sockfd);
 exit(0);
}
