
#include "parse.h"
#include "joblist.h"
#include <stdlib.h>

const unsigned int MAX_INPUT_LENGTH = 4096; 

int parse_file(FILE *fp)
{
	char input[MAX_INPUT_LENGTH];
	if (fgets(input,MAX_INPUT_LENGTH,fp))
	{
		char *start = input;
		char *end = input;
		while(1)
		{
			if (*end == '\0' || *end == '\n') // end of input
			{
				// Process line
				process_statement(start,end - 1); // don't include the null or newline
				// Done
				break;
			} 
			else if (*end == ';' || *end == '&') // separator
			{
				// Process line
				process_statement(start,end);
				// Move to next portion
				start = end = (end + 1);
			}
			else
				end++;
		}
		
	}
	else
		return -1;

	return 0;
}


int process_statement(char *start, char *end)
{
	if (start <= end) return -1; // Empty
	
	char *buff = NULL;
	buff = malloc(  (end - start) + 2 );
	
	memcpy(buff,start,(end - start) + 1);
	buff[ (end - start) + 1] = '\0';
	
	// buff is now a valid string
	
	
	
	
	
	if (buff != NULL)
	{
		free(buff);
		buff = NULL;
	}
	
	
	
}