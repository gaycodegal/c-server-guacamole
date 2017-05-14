#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "my.h"

#define STDIN 0

#define IOMAP_S 1024
#define BUFFERSIZE 256
#define SEGMENT_SIZE 32
#define NUM_SAVED_SEGMENTS 5

typedef int (* mapping)(int key);

typedef struct s_node Node;
typedef struct s_prompt{
  char * message;
  char * begin;
} Prompt;
typedef struct s_message{
  char contents[SEGMENT_SIZE];
  char paginate;
} MessageSegment;

mapping map[IOMAP_S];
int lastlinestart = 0;
char buffer[BUFFERSIZE];
char * bufptr = buffer, *maxbuf, *lastbuf, *firstbuf;
int MAX_X;
int MAX_Y;
short colors_inverse = 1, colors_custom = 2;
int BOX_HEIGHT = 3, DISPLAY_HEIGHT;
Prompt PROMPT;
int writePrompt(int);
Node * __DONT_TOUCH_2__ = NULL;
Node ** display_queue = &__DONT_TOUCH_2__;
int cursorx, cursory;
void PROMPT_DISPLAY(char * message);
void redraw();
int resize(int key);
int shiftLeft();
void recieveMessage(char * text);
int alive;
int sockfd;

void resetCursor(){
  cursory = MAX_Y - BOX_HEIGHT;
  cursorx = 0;
  move(cursory, cursorx);
  bufptr = buffer;
  firstbuf = buffer;
}

int error(char * message){
  PROMPT_DISPLAY(message);
  return -1;
}

int errorstr(char * message){
  my_str(message),my_str("\n");
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

int drawQueueInRect(Node ** queue, int sx, int sy, int mx, int my){
  int oldx, oldy;
  int x = sx, y = sy;
  Node * head;
  MessageSegment * message;
  char c;
  char * str;
  int paginate;
  getyx(stdscr, oldy, oldx);
  move(sy,sx);
  

  for(head = *queue; head != NULL; head = head->next){
    message = (MessageSegment *)head->elem;
    str = message->contents;
    paginate = message->paginate;
    
    c = 1;
    while(y < my && c){
      while(x < mx && (c = *(str++))){
	addch(c);
	++x;
      }
      if(c){
	x = sx;
	++y;
	move(y,x);
      }
    }
    if(paginate){
      x = sx;
      ++y;
      move(y,x);
    }
  }
    
  move(oldy, oldx);
  return y - paginate;
}

void WIPE_PROMPT(){
  PROMPT.message = NULL;
  PROMPT.begin = NULL;
  writePrompt(0);
}

int quit(int key){
  return 1;
}

int backspace(int key){
  int x,y;
  char * p;
  if(bufptr == buffer){
    beep();
    return 0;
  }
  getyx(stdscr, y, x);
  if((x == 0 && y == 0)){
    --firstbuf;
  }else{
    --x;
    if(x < 0){
      x = MAX_X - 1;
      --y;
    }
    move(y,x);
  }
    
  for(p = bufptr - 1; *p; ++p){
    *(p) = *(p+1);
  }
  --lastbuf;
  --bufptr;
  redraw();
  
  return 0;
}

int fallbackfn(int key){
  int x,y;
  int len;
  char * p;
  if(bufptr == maxbuf){
    beep();
    return 0;
  }

  if(key >= 32 && key <= 126){
    getyx(stdscr, y, x);
    if((x == MAX_X - 1 && y == MAX_Y - 1)){
      --x;
      move(y,x);
      ++firstbuf;
      redraw();
    }
    addch(key);

    lastbuf++;
    if(bufptr != maxbuf && *bufptr){
      len = my_strlen(bufptr);
      for(p = bufptr + len; p >= bufptr; --p){
	*(p+1) = *p;
      }
      *(bufptr++) = (char)key;
      redraw();
    }else{
      *(bufptr++) = (char)key;
      *bufptr = 0;
    }
  }
  return 0;
}

int enterfn(int key){
  writemessage(sockfd, buffer);
  /*PROMPT_DISPLAY(buffer);*/
  buffer[0] = 0;
  bufptr = buffer;
  lastbuf = buffer + 1;
  firstbuf = buffer;
  resetCursor();
  resize(0);
  return 0;
}

int conditional_move(y,x){
  if(y >= MAX_Y - BOX_HEIGHT && y < MAX_Y && x >= 0 && x < MAX_X ){
    move(y,x);
    return 1;
  }else{
    /*beep();*/
  }
  return 0;
}

int shiftRight(){
  if(buffer == firstbuf && buffer == bufptr){
    return 0;
  }

  --firstbuf;
  if(firstbuf < buffer)
    firstbuf = buffer;

  --bufptr;
  if(bufptr < buffer)
    bufptr = buffer;  
  
  redraw();
  return 1;
}

int shiftLeft(){
  long diff = (MAX_X*BOX_HEIGHT);
  if(bufptr >= lastbuf - 1)
    return 0;
  
  if(lastbuf - buffer < diff){
    return 0;
  }

  ++firstbuf;
  if(firstbuf > lastbuf-diff)
    firstbuf = lastbuf-diff;

  ++bufptr;
  if(bufptr >= lastbuf - 1)
    bufptr = lastbuf - 1;  
  
  redraw();
  return 1;
}

int leftArr(int key){
  int x, y;
  getyx(stdscr, y, x);
  --x;
  if(x < 0){
    x = MAX_X - 1;
    --y;
  }
  if(!conditional_move(y,x)){
    if(!shiftRight())
      beep();
  }else{
    --bufptr;
  }
  return 0;
}

int rightArr(int key){
  int x, y;
  getyx(stdscr, y, x);
  ++x;
  if(x >= MAX_X){
    x = 0;
    ++y;
  }
  if(bufptr >= lastbuf - 1){
    beep();
    return 0;
  }
  if(!conditional_move(y,x)){
    if(!shiftLeft())
      beep();
  }else{
    ++bufptr;
  }
  return 0;
}

int upArr(int key){
  beep();
  return 0;
}

int downArr(int key){
  beep();
  return 0;
}

void blankInRect(int sx, int sy, int mx, int my, char c){
  int oldx, oldy;
  int x = sx, y = sy;
  getyx(stdscr, oldy, oldx);
  move(sy,sx);
  while(y < my){
    while(x < mx){
      addch(c);
      ++x;
    }
    x = sx;
    ++y;
    move(y,x);
  }
  move(oldy, oldx);
}


void fillInRect(int sx, int sy, int mx, int my, char c, short color){
  attron(COLOR_PAIR(color));
  blankInRect(sx, sy, mx, my, c);
  attroff(COLOR_PAIR(color));
}

void displayMessages(){
  drawQueueInRect(display_queue, 0, 0, MAX_X, MAX_Y - BOX_HEIGHT - 1);
}

void textInRect(char * str, int sx, int sy, int mx, int my){
  int oldx, oldy;
  int x = sx, y = sy;
  char c;
  getyx(stdscr, oldy, oldx);
  move(sy,sx);
  c = 1;
  while(y < my && c){
    while(x < mx && (c = *(str++))){
      addch(c);
      ++x;
    }
    x = sx;
    ++y;
    if(x < mx && y < my)
      move(y,x);
  }
      
  move(oldy, oldx);
}

void redrawBox(){
  blankInRect(0, MAX_Y - BOX_HEIGHT, MAX_X, MAX_Y, ' ');
  textInRect(firstbuf, 0, MAX_Y - BOX_HEIGHT, MAX_X, MAX_Y);
  
  /*my_int(my_strlen(buffer));*/
}

void redrawScreen(){
  blankInRect(0, 0, MAX_X, MAX_Y - BOX_HEIGHT - 1, ' ');
  displayMessages();
}

void redraw(){
  writePrompt(0);
  redrawBox();
  redrawScreen();
}

int resize(int key){
  getmaxyx(stdscr, MAX_Y, MAX_X);

  redraw();
  resetCursor();
  refresh();

  return 0;
}

int writePrompt(int key){
  char * text = PROMPT.message;
  int x = 0, y;
  int oldx, oldy;
  getyx(stdscr, oldy, oldx);
  attron(COLOR_PAIR(colors_inverse));
  y = MAX_Y - BOX_HEIGHT;
  move(y - 1, 0);
  
  
  if(text != NULL){
    for(x = 0; x < MAX_X && text[x]; ++x){
      addch(text[x]);
    }
  }
  for(; x < MAX_X; ++x){
    addch(' ');
  }
  attroff(COLOR_PAIR(colors_inverse));
  move(oldy, oldx);
  return 0;
}

void PROMPT_DISPLAY(char * message){
  PROMPT.message = message;
  PROMPT.begin = message;
  writePrompt(0);  
}

void pushMessage(MessageSegment * message){
  add_elem((void*)message, display_queue);
}

MessageSegment * newMessageSegment(int paginate, char * text){
  MessageSegment * message = (MessageSegment *)malloc(sizeof(MessageSegment));
  message->paginate = paginate;
  my_strncpy(message->contents, text, SEGMENT_SIZE - 1);
  return message;
}

MessageSegment * newMessageSegment2(int paginate, char * text, int seg){
  MessageSegment * message = (MessageSegment *)malloc(sizeof(MessageSegment));
  message->paginate = paginate;
  my_strncpy(message->contents, text, seg);
  return message;
}

void recieveMessage(char * text){
  int len = my_strlen(text);
  char * p;
  int paginate = 1;
  int moveBy = SEGMENT_SIZE - 1;
  PROMPT_DISPLAY(text);  
  for(p = text + len - moveBy; p >= text; p -= moveBy){
    pushMessage(newMessageSegment(paginate, p));
    paginate = 0;
  }
  len = moveBy - (text - p);
  if(len != 0)
    pushMessage(newMessageSegment2(paginate, text, len));
}

void * freeAThing(void * elem){
  free(elem);
  return NULL;
}

mapping fallback;
mapping chosen;

int startup(){  
  int i;
  maxbuf = bufptr + BUFFERSIZE - 1;
  lastbuf = buffer + 1;
  firstbuf = buffer;
  fallback = &fallbackfn;
  chosen = fallback;
  /*PROMPT_DISPLAY(buffer);*/
  
  recieveMessage("-Three Dog Night");
  recieveMessage(" ");
  recieveMessage("In the halls of Shambala");
  recieveMessage("How does your light shine");
  recieveMessage("In the halls of Shambala");
  recieveMessage("How does your light shine");
  recieveMessage(" ");
  recieveMessage("On the road to Shambala");
  recieveMessage("I can tell my sister by the flowers in her eyes");
  recieveMessage("On the road to Shambala");
  recieveMessage("I can tell my sister by the flowers in her eyes");
  recieveMessage(" ");
  recieveMessage("On the road to Shambala");
  recieveMessage("Everyone is lucky, everyone is so kind");
  recieveMessage("On the road to Shambala");
  recieveMessage("Everyone is helpful, everyone is kind"); 
  recieveMessage(" ");
  recieveMessage("With the rain in Shambala");
  recieveMessage("Wash away my sorrow, wash away my shame");
  recieveMessage("With the rain in Shambala");
  recieveMessage("Wash away my troubles, wash away my pain");

  WIPE_PROMPT();

  
  /*  struct _win_st *win;*/

  for(i = 0; i < IOMAP_S; ++i){
    map[i] = NULL;
  }

  /*map[23] = &quit;*/
  map[10] = &enterfn;

  /*arrs*/
  map[258] = &downArr;
  map[259] = &upArr;
  map[260] = &leftArr;
  map[261] = &rightArr;

  /*backspace*/
  map[263] = &backspace;

  /* resizing */
  map[410] = &resize;
  
  initscr();
  if(has_colors() == FALSE){
    endwin();
    my_str("Your terminal does not support color\n");
    return 1;
  }
  start_color();
  init_pair(colors_custom, COLOR_BLACK, COLOR_RED);
  init_pair(colors_inverse, COLOR_BLACK, COLOR_WHITE);
  /*scrollok(stdscr, TRUE);*/
  /*win = newwin(0,0,LINES,COLS);*/
  
  keypad(stdscr, TRUE);
  noecho();
  erase();
  /*raw();*/
  nodelay(stdscr, TRUE);
  resize(0);

  return 0;
}

int loopIter(int c){
  /*fillInRect(2, 2, 4, 4, 'p', colors_custom);*/
  /*beep();*/
  /*move(x/80, x%80);*/
  /*timeout(-1);*/
  chosen = map[c];
  if(chosen == NULL)
    chosen = fallback;
  return chosen(c);
  /*beep();*/
}

int closeUp(){
  /*delwin(win);*/
  echo();
  /*noraw();*/
  endwin();
  static_map_list(*display_queue, freeAThing);
  empty_list(display_queue);
  return 0;
}

int my_exit(int code){
  closeUp();
  return code;
}

/*
a basic client capable of sending ascii.
will terminate if there is no server or it recieves the 
string /exit from the server. Type /exit to quit.
 */
int main(int argc, char *argv[])
{
  int portno, n;
  int alive, exitrequested;
  int shouldwrite;
  fd_set fds;
  int c;
  int maxfd;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char outbuffer[256];
  struct timeval timeout;
  /*startup();
  while(!loopIter(getch()));
  if(my_exit(1))
  return 0;*/

  exitrequested = 0;
  alive = 1;
  if (argc < 3) {
    my_str("usage: "),my_str(argv[0]), my_str(" hostname port\n");
    exit(0);
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    return errorstr("ERROR opening socket");
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    return errorstr("ERROR, no such host\n");
  }
  my_zero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  my_copy((char *)server->h_addr, 
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    return errorstr("ERROR connecting");

  PROMPT_DISPLAY("Please enter your username: ");
  /*my_zero(inbuffer,256);
  fgets(inbuffer,255,stdin);
  writemessage(sockfd, inbuffer);*/
  
  maxfd = STDIN < sockfd ? sockfd : STDIN;
  if(startup()){
    return my_exit(1);
  }

  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  while(alive){
    outbuffer[0] = 0;    
    FD_ZERO(&fds);
    FD_SET(sockfd, &fds);
    FD_SET(STDIN, &fds);
    refresh();
    select(maxfd+1, &fds, NULL, NULL, &timeout);
    shouldwrite = 0;
    c = getch();
    if(c != ERR){
    /*if (FD_ISSET(STDIN, &fds)){*/
      
      if(loopIter(c))
	return my_exit(1);
      refresh();
      /*if((c = getch()) != ERR && loopIter(c))
	return my_exit(1);
	refresh();*/

      /*my_zero(inbuffer,256);
      n = read(STDIN,inbuffer,255);
      inbuffer[n] = 0;
      if (n < 0) 
	return my_exit(error("whao couldn't read from stdin\n"));
	shouldwrite = 1;*/
    }
   
    
    if (FD_ISSET(sockfd, &fds)){
      my_zero(outbuffer,256);
      n = read(sockfd,outbuffer,255);
      outbuffer[n] = 0;
      if (n < 0) {
	my_exit(-1);
	return errorstr("whao, couldn't read from server");
      }else if(n == 0){
	alive = 0;
	my_exit(-1);
	return errorstr("Server unexpectedly terminated");	 
      }
      if(!(outbuffer[0] == '/' &&
	   my_strcmp("/exit", outbuffer) == 0)){
	recieveMessage(outbuffer), redraw();
      }
    }
    
        
    if(alive && outbuffer[0] == '/'){
      if(my_strcmp("/exit", outbuffer) == 0){
	my_exit(0);
	errorstr("Session Terminated by Server");
	return 0;
      }
    }

    if(exitrequested){
      shouldwrite = 0;
    }
    
    /*if(alive && shouldwrite){
      writemessage(sockfd, inbuffer);
      } */   
    ++shouldwrite;
  }
  return my_exit(0);
}
