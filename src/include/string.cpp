#include "string.h"

char * rIntToAscii(int num, char * result) {
    if (num == 0) {
        return result;
    }
    else {
         result=rIntToAscii( num/10, result );
         *result = '0'+num%10;
         return result+1;
    }
}

void itoa(int num, char * output) {
  if (num==0){
    *output++='0';
    *output='\0';
  }
    else
    {
      if(num<0){
        *output++='-';
        num=-num;
      }
      *rIntToAscii(num, output) = '\0';
    }
}

char * rIntToAscii(unsigned int num, char * result) {
    if (num == 0) {
        return result;
    }
    else {
         result=rIntToAscii( num/10, result );
         *result = '0'+num%10;
         return result+1;
    }
}

void itoa(unsigned int num, char * output) {
  if (num==0){
    *output++='0';
    *output='\0';
  }
    else
    {
      if(num<0){
        *output++='-';
        num=-num;
      }
      *rIntToAscii(num, output) = '\0';
    }
}

static void
itoa (char *buf, int base, int d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;

  /* If %d is specified and D is minus, put ‘-’ in the head. */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /* Divide UD by DIVISOR until UD == 0. */
  do
    {
      int remainder = ud % divisor;

      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /* Terminate BUF. */
  *p = 0;

  /* Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}

int len(char * str)
{
    int count = 0;

    while (str[count] != '\0')
        count++;

    return count;
}

bool strcmp(char * str1, char * str2) {
    int total_length = len(str1);
    int slength = len(str2);

    if (total_length < slength || total_length > slength) {
        return false;
    }

    for (int z = 0; z < total_length; z++) {
        if (str1[z] != str2[z]) {
            return false;
        }
    }

    return true;
}

bool charcmp(char str1, char str2) {
    if (str1 == str2)
        return true;

    return false;
}

char * append(char * orig, char * anew)
{
    for (int z = len(orig); z < len(orig) + len(anew); z++)
    {
        orig[z] = anew[z - len(orig)];
    }

    return orig;
}

bool startswith(char * words, char * start)
{
    int cl = len(start);
    int length = len(words);

    if (cl > length)
    {
        return false;
    }

    for (int z = 0; z < cl; z++)
    {
        if (words[z] != start[z])
        {
            return false;
        }
    }

    return true;
}

char * strcpy(char * d, char * s) // destination, source
{
    while (*d++ = *s++);
    return d;
}

char * strcat(char * d, char * s)
{
    strcat(d + len(d), s);
    return d;
}

void clears(char d[128])
{
    for (int z = 0; z < 128; z++)
        d[z] = ' ';
}

char * strip(char * s)
{
    char * r;
    int rs;
    for (int z = 0; z < len(s); z++)
    {
        if (s[z] != ' ')
        {
            r[rs] = s[z];
            rs++;
        }
    }
}

bool strcheckempty(char * s)
{
    for (int z = 0; z < len(s); z++)
        if (s[z] != ' ')
            return false;

    return true;
}

void hextoa(uint8_t in, char * out)
{
    char index[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}; //We use this to convert the number 0-15 into a character.
    out[0] = '0';
    out[1] = 'x';
    out[2] = index[(in & 0xF0) >> 4];
    out[3] = index[(in & 0x0F) >> 0];
}
