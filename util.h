//
//  util.h
//  diff
//
//  Created by William McCarthy on 5/9/19.
//  Modified by Miguel Antonio Macias on 5/19/19
//  Copyright © 2019 William McCarthy. All rights reserved.
//

#ifndef util_h
#define util_h

#include <stdio.h>
#define BUFLEN 256

char* yesorno(int condition);
FILE* openfile(const char* filename, const char* openflags);
void printleft(const char* left, int flags_arr[]);
void printright(const char* right, int flags_arr[]);
void printboth(const char* left_right, int flags_arr[]);
void printline(void);
int newlinecount(char* text);
#endif /* util_h */
