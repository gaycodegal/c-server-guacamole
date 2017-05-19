#include <curses.h>
#include "list.h"
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
#define IN_BUFFSIZE 278
#define SEGMENT_SIZE 32
#define NUM_SAVED_SEGMENTS 300

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
short colors_prompt = 1, colors_display = 2, colors_input = 3;
int BOX_HEIGHT = 3, DISPLAY_HEIGHT;
Prompt PROMPT;
int writePrompt(int);
int display_queue_len = 0;
Node ** display_queue, **display_tail;
int cursorx, cursory;
void PROMPT_DISPLAY(char * message);
void redraw();
int resize(int key);
int rightArr(int key);
int shiftLeft();
void recieveMessage(char * text);
int alive;
int sockfd;


/*
  reset the cursor's position.
 */
void resetCursor(){
  cursory = MAX_Y - BOX_HEIGHT;
  cursorx = 0;
  move(cursory, cursorx);
  bufptr = buffer;
  firstbuf = buffer;
}

/*
  error, and print out to the prompt
 */
int error(char * message){
  PROMPT_DISPLAY(message);
  return -1;
}

/*
  error, and print out onto the screen.
 */
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

/*
draw a message queue in a rectangle.
 */
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

/*
  wipe the prompt's contents and redraw.
 */
void WIPE_PROMPT(){
  PROMPT.message = NULL;
  PROMPT.begin = NULL;
  writePrompt(0);
}

/*
  bind to keys that should make the program quit
 */
int quit(int key){
  return 1;
}

/*
  acts like a backspace should
 */
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

/*
  types things to the screen normally if they fall within the accepted input range.
 */
int fallbackfn(int key){
  int x,y;
  int len;
  char * p;
  if(lastbuf - 1 == maxbuf){
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
    attron(COLOR_PAIR(colors_input));
    addch(key);
    attroff(COLOR_PAIR(colors_input));

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

/*
  deletes the character currently at the cursor or beeps 
 */
int deletekey(int key){
  if(bufptr < lastbuf - 1){
    rightArr(0);
    backspace(0);
  }else{
    beep();
  }
  return 0;
}

/*
  send the current buffer as a message, wipe the current buffer.
 */
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

/*
 exec the move command iff it's a legal move within the user input area
 */
int conditional_move(int y, int x){
  if(y >= MAX_Y - BOX_HEIGHT && y < MAX_Y && x >= 0 && x < MAX_X ){
    move(y,x);
    return 1;
  }else{
    /*beep();*/
  }
  return 0;
}

/*
scrolls the user input text right
 */
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

/*
scrolls the user input text left
 */
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

/*
moves the cursor left, also scrolls text if at left edge
 */
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

/*
moves the cursor right, also scrolls text if at right edge
 */
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

/*
not assigned to functionality, beep at the user.
 */
int upArr(int key){
  beep();
  return 0;
}

/*
  not assigned to functionality, beep at the user.
 */
int downArr(int key){
  beep();
  return 0;
}

/*
  fill a rectangle to the screen of a certain character,
  with whatever color is currently set.
 */
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

/*
  fill a rectangle to the screen with a color and a character.
 */
void fillInRect(int sx, int sy, int mx, int my, char c, short color){
  attron(COLOR_PAIR(color));
  blankInRect(sx, sy, mx, my, c);
  attroff(COLOR_PAIR(color));
}

/*
  draw the messages saved to the screen.
 */
void displayMessages(){
  drawQueueInRect(display_queue, 0, 0, MAX_X, MAX_Y - BOX_HEIGHT - 1);
}

/*
  draw text in a rectangular area.
 */
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

/*
  redraw the user input box.
 */
void redrawBox(){
  attron(COLOR_PAIR(colors_input));
  blankInRect(0, MAX_Y - BOX_HEIGHT, MAX_X, MAX_Y, ' ');
  textInRect(firstbuf, 0, MAX_Y - BOX_HEIGHT, MAX_X, MAX_Y);
  attroff(COLOR_PAIR(colors_input));
  /*my_int(my_strlen(buffer));*/
}

/*
  redraw the message area.
 */
void redrawScreen(){
  attron(COLOR_PAIR(colors_display));
  blankInRect(0, 0, MAX_X, MAX_Y - BOX_HEIGHT - 1, ' ');
  displayMessages();
  attroff(COLOR_PAIR(colors_display));
}

/*
  redraw the whole client.
 */
void redraw(){
  writePrompt(0);
  redrawBox();
  redrawScreen();
}

/*
  binding to resize the screen.
 */
int resize(int key){
  getmaxyx(stdscr, MAX_Y, MAX_X);

  redraw();
  resetCursor();
  refresh();

  return 0;
}

/*
  draw the prompt to the screen
 */
int writePrompt(int key){
  char * text = PROMPT.message;
  int x = 0, y;
  int oldx, oldy;
  getyx(stdscr, oldy, oldx);
  attron(COLOR_PAIR(colors_prompt));
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
  attroff(COLOR_PAIR(colors_prompt));
  move(oldy, oldx);
  return 0;
}

/*
  display a message on the prompt
 */
void PROMPT_DISPLAY(char * message){
  PROMPT.message = message;
  PROMPT.begin = message;
  writePrompt(0);  
}

/*
  add a message segment to the queue.
 */
void pushMessage(MessageSegment * message){
  add_elem((void*)message, display_queue);
}

/*
  creates a message segment, assuming text is a string
  of length SEGMENT_SIZE - 1
 */
MessageSegment * newMessageSegment(int paginate, char * text){
  MessageSegment * message = (MessageSegment *)malloc(sizeof(MessageSegment));
  if(text == NULL)
    return message;
  message->paginate = paginate;
  my_strncpy(message->contents, text, SEGMENT_SIZE - 1);
  return message;
}

/*
  Creates a new message segment with a specified length (seg).
  seg must be smaller than SEGMENT_SIZE
 */
MessageSegment * newMessageSegment2(int paginate, char * text, int seg){
  MessageSegment * message = (MessageSegment *)malloc(sizeof(MessageSegment));
  if(text == NULL || seg >= SEGMENT_SIZE)
    return message;
  message->paginate = paginate;
  my_strncpy(message->contents, text, seg);
  return message;
}

/*
  Splits a message up into segments of SEGMENT_SIZE.
  Adds this message to those messages that should be displayed.
 */
void recieveMessage(char * text){
  int len = my_strlen(text);
  char * p;
  int paginate = 1;
  int moveBy = SEGMENT_SIZE - 1;
  PROMPT_DISPLAY(text);  
  for(p = text + len - moveBy; p >= text; p -= moveBy){
    pushMessage(newMessageSegment(paginate, p));
    paginate = 0;
    ++display_queue_len;
  }
  len = moveBy - (text - p);
  if(len != 0){
    pushMessage(newMessageSegment2(paginate, text, len));
    ++display_queue_len;
  }
  if(*display_tail == NULL)
    *display_tail = last_node(*display_queue);
  while(display_queue_len > NUM_SAVED_SEGMENTS){
    remove_node(display_tail);
    --display_queue_len;
  }
}

/*
  frees something and returns NULL
  used for mapping over a list
 */
void * freeAThing(void * elem){
  free(elem);
  return NULL;
}

mapping fallback;
mapping chosen;

/*
  starts the graphical curses client
 */
int startup(){  
  int i;
  display_queue = make_empty_list();
  display_tail = make_empty_list();
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
  recieveMessage("Tell me, how does your light shine");
  recieveMessage(" ");
  recieveMessage("On the road to Shambala");
  recieveMessage("I can tell my brother by the flowers in her eyes");
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

  PROMPT_DISPLAY("(Guacamole-Client v0.9.1) Please Enter Your Name.");

  
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
  /*delete*/
  map[KEY_DC] = &deletekey;
  map[58] = &deletekey;

  /* resizing */
  map[410] = &resize;
  
  initscr();
  if(has_colors() == FALSE){
    endwin();
    my_str("Your terminal does not support color\n");
    return 1;
  }
  start_color();
  init_color(COLOR_RED, 184, 142, 12);
  init_color(COLOR_GREEN, 78, 154, 6);
  /*init_color(COLOR_BLUE, 78, 154, 6);*/
  init_pair(colors_display, COLOR_BLACK, COLOR_GREEN);
  init_pair(colors_input, COLOR_BLACK, COLOR_YELLOW);
  init_pair(colors_prompt, COLOR_BLACK, COLOR_WHITE);
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

/*
  one iteration of input handling for the client.
 */
int loopIter(int c){
  /*beep();*/
  /*move(x/80, x%80);*/
  /*timeout(-1);*/
  chosen = map[c];
  if(chosen == NULL)
    chosen = fallback;
  return chosen(c);
  /*beep();*/
}

/*
  end the graphical client.
 */
int closeUp(){
  /*delwin(win);*/
  echo();
  /*noraw();*/
  endwin();
  static_map_list(*display_queue, freeAThing);
  free_list(display_queue);
  *display_tail = NULL;
  free(display_tail);
  return 0;
}

/*
  in case we need to wrap the closup
 */
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
  char outbuffer[IN_BUFFSIZE];
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
  /*my_zero(inbuffer,IN_BUFFSIZE);
  fgets(inbuffer,IN_BUFFSIZE - 1,stdin);
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

      /*my_zero(inbuffer,IN_BUFFSIZE);
      n = read(STDIN,inbuffer,IN_BUFFSIZE - 1);
      inbuffer[n] = 0;
      if (n < 0) 
	return my_exit(error("whao couldn't read from stdin\n"));
	shouldwrite = 1;*/
    }
   
    
    if (FD_ISSET(sockfd, &fds)){
      my_zero(outbuffer,IN_BUFFSIZE);
      n = read(sockfd,outbuffer,IN_BUFFSIZE - 1);
      outbuffer[n] = 0;
      if (n < 0) {
	my_exit(-1);
	return errorstr("whao, couldn't read from server");
      }else if(n == 0){
	alive = 0;
	my_exit(-1);
	return errorstr("Connection Terminated by Server.");
      }
      if(!(outbuffer[0] == '/' &&
	   my_strcmp("/exit", outbuffer) == 0)){
	recieveMessage(outbuffer), redraw();
      }
    }
    
        
    if(alive && outbuffer[0] == '/'){
      if(my_strcmp("/exit", outbuffer) == 0){
	my_exit(0);
	errorstr("Connection Terminated by Server.");
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
