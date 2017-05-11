/* Header for libmy assignment. */ 
#ifndef _MY_H_ 
#define _MY_H_

#ifndef NULL 
#define NULL ((void*)0) 
#endif

#include <unistd.h> 
#include <stdlib.h>

void my_char(char); 
void my_str(const char*); 
void my_int(int); 
void my_num_base(int, const char* base); 
void my_alpha(); 
void my_digits();
int my_strlen(const char*); 
int my_revstr(char*); 
int my_strindex(const char*, char); 
int my_strrindex(const char*, char);

char *my_strfind(char*, char);
char *my_strrfind(char*, char);
int my_strcmp(const char*, const char*);
int my_strncmp(const char*, const char*, int);
int my_strkcmp(const char*, const char*, int);
char *my_strcpy(char*dst, const char*src);
char *my_strncpy(char*, const char*, int); 
char *my_strcat(char*, const char*); 
char *my_strdup(const char*); 
char *my_strconcat(const char*, const char*); 
char *my_strnconcat(const char*, const char*, int); 
int my_atoi(const char*); 
char *my_vect2str(char **x);
char** my_str2vect(char*);
#endif
