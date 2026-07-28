#ifndef OPERATIONS_H

#include <stdio.h>

int encode(char* inputpath, char* outputpath);
int decode(char* inputpath, char* outputpath);

//usado somente para debug
int __decode(char* inputpath, char* outputpath);
int __encode(char* inputpath, char* outputpath);

#define OPERATIONS_H
#endif