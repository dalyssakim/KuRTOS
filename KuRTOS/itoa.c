#include "itoa.h"

static char t[ITOA_BUF_SIZE];  // private storage for these functions. Not reentrant.

char *itoa (int i)
{
  char *s = t + ITOA_BUF_SIZE - 1;
  int j = i;
  
	i = i<0? -i : i;
  *s-- = 0;
  do {
		*s-- = (char) (i % 10 + '0');
    } while ((i = i / 10));
  if (j < 0)
		*s-- = '-';
  return ++s;
}

char * utoa(unsigned int i)
{
  char *s = t + ITOA_BUF_SIZE - 1;
  unsigned int j = i;
  
  *s-- = 0;
  do {
		*s-- = (char) (i % 10 + '0');
    } while ((i = i / 10));

  return ++s;
}