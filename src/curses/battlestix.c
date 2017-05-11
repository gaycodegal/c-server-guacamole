#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define IOMAP_S 1024
#define BUFFERSIZE 1000
#define SEGMENT_SIZE 32

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
char * bufptr = buffer;
int MAX_X;
int MAX_Y;
short colors_inverse = 1, colors_custom = 2;
int BOX_HEIGHT = 3, DISPLAY_HEIGHT;
Prompt PROMPT;
int writePrompt(int);
Node * __DONT_TOUCH_1__ = NULL;
Node * __DONT_TOUCH_2__ = NULL;
Node ** box_queue = &__DONT_TOUCH_1__;
Node ** display_queue = &__DONT_TOUCH_2__;


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

int fallbackfn(int key){
  if(key&1){
    attron(COLOR_PAIR(colors_custom));
  }else{
    attroff(COLOR_PAIR(colors_custom));
  }
  if((key >= 97 && key <= 122) || (key >= 65 && key <= 90)){
    addch(key);
  }
  *(bufptr++) = (char)key;
  return 0;
}

int enterfn(int key){
  char c;
  int x, y, i;
  int delta, didsrc = 0;
  getyx(stdscr, y, x);
  /*insertln();*/
  y++;
  /*int moveby = ;*/
  
  delta = y - lastlinestart;
  if(delta < 1)
    delta = 1;
  if(y + delta >= MAX_Y){
    didsrc = 1;
    for(i = 0; i < delta; ++i){
      addch('\n');
      scroll(stdscr);
    }
  }
  x++;
  move(y,0);
  *bufptr = 0;
  bufptr = buffer;
  while((c=*(bufptr++))){
    addch(c);
  }
  bufptr = buffer;
  getyx(stdscr, y, x);
  /*insertln();*/
  y++;
  lastlinestart = y;
  x++;
  if(didsrc){
    addch('\n');
    scroll(stdscr);
    
  }
  move(y,0);
  /*refresh();*/
  /*addch(90);*/
  return 0;
}

int leftArr(int key){
  int x, y;
  getyx(stdscr, y, x);
  move(y,--x);
  return 0;
}

int rightArr(int key){
  int x, y;
  getyx(stdscr, y, x);
  move(y,++x);
  return 0;
}

int upArr(int key){
  int x, y;
  getyx(stdscr, y, x);
  move(--y,x);
  return 0;
}

int downArr(int key){
  int x, y;
  getyx(stdscr, y, x);
  move(++y,x);
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

void redrawBox(){
  blankInRect(0, MAX_Y - BOX_HEIGHT, MAX_X, MAX_Y, ' ');
}

void redrawScreen(){
  blankInRect(0, 0, MAX_X, MAX_Y - BOX_HEIGHT - 1, ' ');
  displayMessages();
}

int resize(int key){
  getmaxyx(stdscr, MAX_Y, MAX_X);
  redrawBox();
  writePrompt(0);
  redrawScreen();
  return 0;
}

int writePrompt(int key){
  char * text = PROMPT.message;
  int x = 0, y;
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
  /*for(y = 0; y < MAX_Y; ++y){*/  
  attroff(COLOR_PAIR(colors_inverse));  
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
  
  for(p = text + len - moveBy; p >= text; p -= moveBy){
    pushMessage(newMessageSegment(paginate, p));
    paginate = 0;
    PROMPT_DISPLAY(p);
  }
  len = moveBy - (text - p);
  if(len != 0)
    pushMessage(newMessageSegment2(paginate, text, len));
}

int main(int argc, char ** argv){  
  int x = 0, i;
  int arrs[5];
  int c = 10;
  mapping fallback = &fallbackfn;
  mapping chosen = fallback;

  recieveMessage("*");
  recieveMessage("*");
  recieveMessage("On the road to Shambala");
  recieveMessage("Everyone is lucky, everyone is so kind");
  recieveMessage("On the road to Shambala");
  recieveMessage("Everyone is helpful, everyone is kind");
  
  recieveMessage("With the rain in Shambala");
  recieveMessage("Wash away my sorrow, wash away my shame");
  recieveMessage("With the rain in Shambala");
  recieveMessage("Wash away my troubles, wash away my pain");




  fallback(1);
  /*  struct _win_st *win;*/
  arrs[4] = 0;

  for(i = 0; i < IOMAP_S; ++i){
    map[i] = NULL;
  }

  map[23] = &quit;
  map[10] = &enterfn;

  /*arrs*/
  map[258] = &downArr;
  map[259] = &upArr;
  map[260] = &leftArr;
  map[261] = &rightArr;

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
  scrollok(stdscr, TRUE);
  /*win = newwin(0,0,LINES,COLS);*/
  keypad(stdscr, TRUE);
  noecho();
  erase();
  resize(0);
  /*fillInRect(2, 2, 4, 4, 'p', colors_custom);*/
    
  while(c > 0){
    /*beep();*/
    /*move(x/80, x%80);*/
    /*timeout(-1);*/
    c = getch();
    arrs[x%4] = (int)c;
    ++x;
    chosen = map[c];
    if(chosen == NULL)
      chosen = fallback;
    if(chosen(c)){
      break;
    }
    
    /*beep();*/
  }
  /*delwin(win);*/
  echo();
  endwin();
  printf("%i %i %i %i\n", arrs[0], arrs[1], arrs[2], arrs[3]);
  return 0;
}

