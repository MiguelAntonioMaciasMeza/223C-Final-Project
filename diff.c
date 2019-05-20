//
//  diff_02.c
//  diff
//
//  Created by William McCarthy on 4/29/19.
//  Modified by Miguel Antonio Macias on 5/19/19
//  Copyright © 2019 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGC_ERROR 1
#define TOOMANYFILES_ERROR 2
#define CONFLICTING_OUTPUT_OPTIONS 3

#define MAXSTRINGS 1024
#define MAXPARAS 4096

#define HASHLEN 200

#include "para.c"
#include "util.c"


void version(void) {
  printf("\n\n\ndiff (CSUF diffutils) 1.0.0\n");
  printf("Copyright (C) 2014 CSUF\n");
  printf("This program comes with NO WARRANTY, to the extent permitted by law.\n");
  printf("You may redistribute copies of this program\n");
  printf("under the terms of the GNU General Public License.\n");
  printf("For more information about these matters, see the file named COPYING.\n");
  printf("Written by Dr. William McCarthy, Tony Stark (Miguel), and Dr. Steven Strange\n");
}

void todo_list(void) {
  printf("\n\n\nTODO: check line by line in a paragraph, using '|' for differences");
  printf("\nTODO: this starter code does not yet handle printing all of fin1's paragraphs.");
  printf("\nTODO: handle the rest of diff's options\n");
}

char buf[BUFLEN];
char *strings1[MAXSTRINGS], *strings2[MAXSTRINGS];
int showversion = 0, showbrief = 0, ignorecase = 0, report_identical = 0, showsidebyside = 0;
int showleftcolumn = 0, showunified = 0, showcontext = 0, suppresscommon = 0, diffnormal = 0;

int count1 = 0, count2 = 0;


void loadfiles(const char* filename1, const char* filename2) {
  memset(buf, 0, sizeof(buf));
  memset(strings1, 0, sizeof(strings1));
  memset(strings2, 0, sizeof(strings2));
  
  FILE *fin1 = openfile(filename1, "r");
  FILE *fin2 = openfile(filename2, "r");
  
  while (!feof(fin1) && fgets(buf, BUFLEN, fin1) != NULL) {
    strings1[count1++] = strdup(buf); 
  }

  fclose(fin1);

  while (!feof(fin2) && fgets(buf, BUFLEN, fin2) != NULL) { 
    strings2[count2++] = strdup(buf); 
  }  

  fclose(fin2);
}

void print_option(const char* name, int value) { 
  printf("%17s: %s\n", name, yesorno(value)); 
}

void diff_output_conflict_error(void) {
  fprintf(stderr, "diff: conflicting output style options\n");
  fprintf(stderr, "diff: Try `diff --help' for more information.)\n");
  exit(CONFLICTING_OUTPUT_OPTIONS);
}

void setoption(const char* arg, const char* s, const char* t, int* value) {
  if ((strcmp(arg, s) == 0) || ((t != NULL && strcmp(arg, t) == 0))) {
    *value = 1;
  }
}

void showoptions(const char* file1, const char* file2) {
  printf("diff options...\n");
  print_option("diffnormal", diffnormal);
  print_option("show_version", showversion);
  print_option("show_brief", showbrief);
  print_option("ignorecase", ignorecase);
  print_option("report_identical", report_identical);
  print_option("show_sidebyside", showsidebyside);
  print_option("show_leftcolumn", showleftcolumn);
  print_option("suppresscommon", suppresscommon);
  print_option("showcontext", showcontext);
  print_option("show_unified", showunified);
  
  printf("file1: %s,  file2: %s\n\n\n", file1, file2);
  
  printline();
}


void init_options_files(int argc, const char* argv[]) {
  int cnt = 0;
  const char* files[2] = { NULL, NULL };
  //printf("%i \n", argc);
  while (argc-- > 0) {
    const char* arg = *argv;
    //printf("%s \n", arg);
    setoption(arg, "-v",       "--version",                  &showversion);       //DONE
    setoption(arg, "-q",       "--brief",                    &showbrief);         //DONE
    setoption(arg, "-i",       "--ignore-case",              &ignorecase);        //DONE
    setoption(arg, "-s",       "--report-identical-files",   &report_identical);  //DONE
    setoption(arg, "--normal", NULL,                         &diffnormal);        //DONE
    setoption(arg, "-y",       "--side-by-side",             &showsidebyside);    //DONE
    setoption(arg, "--left-column", NULL,                    &showleftcolumn);    //DONE
    setoption(arg, "--suppress-common-lines", NULL,          &suppresscommon);    //DONE
    setoption(arg, "-c",       "--context",                  &showcontext);       //  TODO
    setoption(arg, "-u",       "--showunified",                &showunified);     //DONE
    if (arg[0] != '-') {
      if (cnt == 2) {
        fprintf(stderr, "apologies, this version of diff only handles two files\n");
        fprintf(stderr, "Usage: ./diff [options] file1 file2\n");
        exit(TOOMANYFILES_ERROR);
      } 
      
      else { 
        files[cnt++] = arg; 
      }
    }
    ++argv;   // DEBUG only;  move increment up to top of switch at release
  }

  if (!showcontext && !showunified && !showsidebyside && !showleftcolumn) {
    diffnormal = 1;
  }
  
  if (showversion) { version();  exit(0); }
  
  if (((showsidebyside || showleftcolumn) && (diffnormal || showcontext || showunified)) || (showcontext && showunified) || (diffnormal && (showcontext || showunified))) {
    diff_output_conflict_error();
  }
  
 // showoptions(files[0], files[1]);
  loadfiles(files[0], files[1]);
}


int main(int argc, const char * argv[]) {
  init_options_files(--argc, ++argv);
  
  int flags_arr[6];
  flags_arr[0] = diffnormal;
  flags_arr[1] = showsidebyside;
  flags_arr[2] = showunified;
  flags_arr[3] = showcontext;
  flags_arr[4] = suppresscommon;
  flags_arr[5] = showleftcolumn;

//  para_printfile(strings1, count1, printleft);
//  para_printfile(strings2, count2, printright);
  
  para* p = para_first(strings1, count1);
  para* q = para_first(strings2, count2);
  int foundmatch = 0;
  int count = 0;  //Keeps tracks of amount of times files are diffrent 
  int is_same = 0;
  int is_differnt = 0;
  
  para* qlast = q;

  while (p != NULL) {
    
    qlast = q;
    foundmatch = 0;

    while (q != NULL && (foundmatch = para_equal(p, q)) == 0) {
      q = para_next(q);
    }

    q = qlast;
    if (foundmatch) {
      while ((foundmatch = para_equal(p, q)) == 0) {

        if (!report_identical &&!showbrief)
          para_print(q, printright, flags_arr);
        q = para_next(q);
        qlast = q;
      }
      
  

      if (showsidebyside || showunified) {
        if (!suppresscommon) {
          if (!report_identical && !showbrief) {
            
            para_print(q, printboth, flags_arr);
          }
        }
      }

      p = para_next(p);
      q = para_next(q);
      if(p == NULL  && report_identical  && count == 0){
          printf("The files are the same \n");
      }

      
    }else{
      if (showbrief) {
        printf("The files are different\n");
        exit(0);
      }if(report_identical){
          count = 1;
          para_print(p, printright, flags_arr);
          para_print(q, printleft, flags_arr);
       
      }
      if (!report_identical && !showbrief){
        count = 1;
        para_print(p, printleft, flags_arr);
      }
      p = para_next(p);
    }

  }
 

  while (q != NULL) {
    if (!report_identical && !showbrief)
      para_print(q, printright, flags_arr);
    q = para_next(q);
  }
  return 0;
}
