
#include "parse.h"
#include <stdlib.h>
#include <string.h>

const unsigned int MAX_INPUT_LENGTH = 4096; 
const unsigned int MAX_NUM_PARAMS = 2048; 

int process_statement(char *start, char *end);

int parse_line(FILE *fp)
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
	// Isolate the command and copy it to a string
	if (start > end) return -1; // Empty
	
	char *buff = NULL;
	buff = malloc(  (end - start) + 2 );
	
	memcpy(buff,start,(end - start) + 1);
	buff[ (end - start) + 1] = '\0';
	
	// buff is now a valid string containing a single 'command'
	printf("[%s]\n",buff);
	
	// tokenize the command
	char *temp_str = NULL;
	int num_params = 0;
	char **params = NULL;

	for(temp_str = strtok(buff, " \t"); temp_str != NULL; temp_str = strtok(NULL, " \t"))
	{
		params = (char**)realloc(params, (sizeof(char*) * (num_params + 1)));
		if (params == NULL)
		{
			fprintf(stderr, "Fatal error allocating storage while parsing.");
			// TODO: exit program...
		}
		params[num_params] = strdup(temp_str);
		num_params++;
	}
	
	
	printf("%d parameters tokenized.\n",num_params);
	
	for(int i = 0; i < num_params; i++)
	{
		printf("%d) %s\n",i,params[i]);
	}
	
	// check there is at least one non-empty parameter
	
	// check for special instructions (exit, jobs)
	
	
	// Find number of params, check for &
	
	
	// Build job
	
	
	// Submit job
	
	
	
	if (params != NULL)
	{
		free(params);
		params = NULL;
	}
	
	if (buff != NULL)
	{
		free(buff);
		buff = NULL;
	}
	
	
	return 0;
}