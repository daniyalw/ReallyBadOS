#include "string.h"

int len(char * words)
{
    int length = 0;

    while (words[length] != '\0')
        length++;

    return length;
}
char * rIntToAscii(int num, char * result){
    if( num == 0 ) return result;
    else{
         result=rIntToAscii( num/10, result );
         *result = '0'+num%10;
        return result+1;
    }
}

void itoa(int num,char *output){
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

char * rIntToAscii(uint8_t num, char * result){
    if( num == 0 ) return result;
    else{
         result=rIntToAscii( num/10, result );
         *result = '0'+num%10;
        return result+1;
    }
}

void itoa(uint8_t num,char *output){
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
void atoi(char * str, int out) {
	int length = len(str);
	int i;
	int pow = 1;
	for (i = length; i > 0; --i) {
		out += (str[i-1] - 48) * pow;
		pow *= 10;
	}
}

string::string(char * str)
{
    words = str;
    length = 0;
}

void string::set(char * str)
{
    words = str;
    length = 0;

    while (words[length] != '\0')
        length++;
}

int string::size()
{
    return length;
}

char * string::get()
{
    return words;
}

void string::append(char * other)
{
    for (int z = 0; z < len(other); z++)
        words += other[z];

    length += len(other);
}

int string::startswith(char * start)
{
    int cl = len(start);

    if (cl > length)
    {
        return 1;
    }

    for (int z = 0; z < length; z++)
    {
        if (words[z] != start[z])
        {
            return 1;
        }
    }

    return 0;
}
