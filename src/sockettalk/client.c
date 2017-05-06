/*
Steph Oro
I pledge my honor that I have abided by the Stevens Honor System.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "my.h"

#define STDIN 0

int alive;

/*
quits things
 */
int error(const char *msg)
{
  alive = 0;
  my_str(msg);
  return -1;
}

/*
send a message somewhere
 */
void writemessage(int fd, char * buffer){
  int n;
  n = write(fd,buffer,my_strlen(buffer));
  fsync(fd);
  if (n < 0) 
    error("ERROR writing to socket");
}

/*
zeros out things
 */
void my_zero(char * c, int size){
  int i;
  for(i = 0; i < size; ++i){
    c[i] = 0;
  }
}

/*
copies things
 */
void my_copy(char * a, char * b, int len){
  int i = 0;
  for(i = 0; i < len; ++i){
    b[i] = a[i];
  }
}

/*
a basic client capable of sending ascii.
will terminate if there is no server or it recieves the 
string /exit from the server. Type /exit to quit.
 */
int main(int argc, char *argv[])
{
  int sockfd, portno, n;
  int alive, exitrequested;
  int shouldwrite;
  fd_set fds;
  int maxfd;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char inbuffer[256];
  char outbuffer[256];
  exitrequested = 0;
  alive = 1;
  if (argc < 3) {
    my_str("usage: "),my_str(argv[0]), my_str(" hostname port\n");
    exit(0);
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    return error("ERROR opening socket");
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    return error("ERROR, no such host\n");
  }
  my_zero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  my_copy((char *)server->h_addr, 
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    return error("ERROR connecting");
  my_str("Please enter your username: ");
  my_zero(inbuffer,256);
  fgets(inbuffer,255,stdin);
  writemessage(sockfd, inbuffer);

  maxfd = STDIN < sockfd ? sockfd : STDIN;

  while(alive){
    inbuffer[0] = 0;
    outbuffer[0] = 0;    
    FD_ZERO(&fds);
    FD_SET(sockfd, &fds);
    FD_SET(STDIN, &fds);
    
    select(maxfd+1, &fds, NULL, NULL, NULL);
    shouldwrite = 0;
    if (FD_ISSET(STDIN, &fds)){
      my_zero(inbuffer,256);
      n = read(STDIN,inbuffer,255);
      inbuffer[n] = 0;
      if (n < 0) 
	return error("whao couldn't read from stdin\n");
      shouldwrite = 1;
    }
   
    
    if (FD_ISSET(sockfd, &fds)){
      my_zero(outbuffer,256);
      n = read(sockfd,outbuffer,255);
      outbuffer[n] = 0;
      if (n < 0) {
	return error("whao, couldn't read from server\n");
      }else if(n == 0){
	alive = 0;
	my_str("Server unexpectedly terminated\n");
      }
      if(!(outbuffer[0] == '/' && my_strcmp("/exit", outbuffer) == 0))
	my_str(outbuffer), my_str("\n");
    }
    
    if(alive && inbuffer[0] == '/'){
      if(my_strcmp("/exit", inbuffer) == 0){
	exitrequested = 1;
	writemessage(sockfd, inbuffer);
	inbuffer[0] = 0;
      }
    }
    
    if(alive && outbuffer[0] == '/'){
      if(my_strcmp("/exit", outbuffer) == 0){
	my_str("Session terminated by server\n");
	alive = 0;
      }
    }

    if(exitrequested){
      shouldwrite = 0;
    }
    
    if(alive && shouldwrite){
      writemessage(sockfd, inbuffer);
    }    
    
  }

  return 0;
}
