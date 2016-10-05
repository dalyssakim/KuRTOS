#ifndef _TYPE_H
#define _TYPE_H

#define MAXTASKNUM	6
#define TRUE		1
#define FALSE		0

typedef unsigned char BOOLEAN;
typedef unsigned char INT8U;
typedef signed int INT8S;
typedef unsigned int INT16U;
typedef signed int INT16S;
typedef unsigned long INT32U;
typedef signed long INT32S; 
typedef INT16S	TID;

typedef INT16U OS_STK;
extern INT16U IDLE_TID;

#endif