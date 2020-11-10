#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

int end(int x, int y, char keyword[]){
	int result = strcmp(keyword, "END");
	if(result == 0) printf("[OUTPUT] %d\n", x);
	return 0;
}
int my_div(int x, int y, char keyword[]){
	int result = strcmp(keyword, "DIV");
	if(result == 0) {
	  // Calculate sign of divisor i.e.,
	  // sign will be negative only iff
	  // either one of them is negative
	  // otherwise it will be positive
	  int sign = ((x < 0) ^ (y < 0)) ? -1 : 1;

	  // Update both divisor and
	  // dividend positive
	  x = abs(x);
	  y = abs(y);

	  // Initialize the quotient
	  int quotient = 0;
	  while (x >= y) {
	    x -= y;
	    ++quotient;
	  }

	  return sign * quotient;
    }

	return end(x, y, keyword);
}
int mul(int x, int y, char keyword[]){
	int result = strcmp(keyword, "MUL");
	if(result == 0){
	   int res = 0;  // initialize result

	    // While second number doesn't become 1
	    while (y > 0)
	    {
	         // If second number becomes odd, add the first number to result
	         if (y & 1)
	             res = res + x;

	         // Double the first number and halve the second number
	         x = x << 1;
	         y = y >> 1;
	     }
	     return res;
	}
	return my_div(x, y, keyword);
}

int sub(int x, int y, char keyword[]){
	int result = strcmp(keyword, "SUB");
	if(result == 0) {
	    if (y == 0)
	        return x;
	    return sub(x ^ y, (~x & y) << 1, keyword);
    }
    return mul(x, y, keyword);
}

int add(int x, int y, char keyword[]){
	int result = strcmp(keyword, "ADD");
	if(result == 0) return x|y;
	return sub(x, y, keyword);
}

int start(int x, int y, char keyword[]){
	int result = strcmp(keyword, "START");
	if(result == 0) return y;
	return add(x, y, keyword);

}
// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.

void trim(char *str)
{
    char *start, *end;

    /* Find first non-whitespace */
    for (start = str; *start; start++)
    {
        if (!isspace((unsigned char)start[0]))
            break;
    }

    /* Find start of last all-whitespace */
    for (end = start + strlen(start); end > start + 1; end--)
    {
        if (!isspace((unsigned char)end[-1]))
            break;
    }

    *end = 0; /* Truncate last whitespace */

    /* Shift from "start" to the beginning of the string */
    if (start > str)
        memmove(str, start, (end - start) + 1);
}
int parseNumber(char* ptr)
{
	regex_t reg;
	regmatch_t refs[1]; //as in, the maximum number of data you want to extract
	regcomp(&reg, "[0-9]", 0); //replace 0 with REG_EXTENDED if desired
	regexec(&reg, ptr, 1, refs, 0);
	regfree(&reg);

	return atoi(ptr+refs[0].rm_so);
}
int main(int argc, char **argv) {
  int x = 0;
  int y = 0;
  char text[] = "START 9; ADD 1; MUL 9; SUB 2; DIV 5; END 99;";
  char delim[] = ";";

	printf("[INPUT] %s\n", text);

	char *ptr = strtok(text, delim);

	while(ptr != NULL)
	{
		trim(ptr);
		char keyword[10];
		sscanf(ptr, "%[A-Z]", keyword);
		y = parseNumber(ptr);
		x = start(x, y, keyword);
		ptr = strtok(NULL, delim);
	}

  return 1;
}
