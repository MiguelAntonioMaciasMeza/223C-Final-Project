//Modified by Miguel Antonio Macias on 5/19/19

#include <string.h>
#include <stdlib.h>
#include "util.h"

#define BUFLEN 256


int at_line = 0;

char* yesorno(int condition) { return condition == 0 ? "no" : "YES"; }

FILE* openfile(const char* filename, const char* openflags) {
  FILE* f;
  if ((f = fopen(filename, openflags)) == NULL) {  printf("can't open '%s'\n", filename);  exit(1); }
  return f;
}

void printline(void) {
  for (int i = 0; i < 10; ++i) { printf("=========="); }
  printf("\n");
}


void printleft(const char* left, int flags_arr[]) {
  char buf[BUFLEN];
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;

  // Default print
  if (flags_arr[0] == 1) {

    printf("< %s", left);
    return;
  }

  // Unified print
  if (flags_arr[2] == 1) {
    printf("- %s", left);
    return;
  }

  for (j = 0; j <= 48 - len ; ++j) { 
    buf[len + j] = ' '; 
  }

  buf[len + j++] = '<';
  buf[len + j++] = '\0';
  printf("%s\n", buf);
}

void printright(const char* right, int flags_arr[]) {
  if (right == NULL) { 
    return; 
  }

  // Default print
  if (flags_arr[0] == 1) {
    printf("> %s", right);
    return;
  }
  // Unified print
  if (flags_arr[2] == 1) {
   printf("+ %s", right);
    return;
  }

  if (flags_arr[1] == 1)
    printf("%50s %s", ">", right);
}

void printboth(const char* left_right, int flags_arr[]) {
  //Unified Printing
  if (flags_arr[2] == 1) {
    printf("  %s", left_right);
    return;
  }

  char buf[BUFLEN];
  size_t len = strlen(left_right);
  if (len > 0) {
    strncpy(buf, left_right, len);
  }
  buf[len - 1] = '\0';

  //Left-Column print
  if (flags_arr[5]) {
    char *lp = (char *)left_right;
    
    while (*lp != '\n') {
      lp++;
    }
    *lp = '\0';

    printf("%-50s", left_right);
    
    //Left-Column print
    if (flags_arr[5]) {
      printf("(");    
    }

    printf("\n");
  } else {
    printf("%-50s %s", buf, left_right);
  }
}

int newlinecount(char* text) {
  int len = strlen(text);
  int count = 0;
  for (int i = 0; i < len; i++) {
    if (text[i] == '\n')
      count++;
  }

  return count;
}
