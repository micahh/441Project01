
#include "parse.h"
#include "joblist.h"
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
	printf("[%s]\n",buff); // DEBUG ONLY
	
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
	
	
	printf("%d parameters tokenized.\n",num_params); // DEBUG ONLY
	
	// check there is at least one non-empty parameter
	
	
	if (num_params > 0)
	{
		jobtype_t jobtype;
		
		if (strlen(params[num_params - 1]) == 1)
		{
			// Last parameter is a single character, see if it was & or ;
			// and if so, remove that parameter and set jobtype
			if (*params[num_params - 1] == '&')
			{
				jobtype = PARALLEL;
				num_params--;
			}
			else if (*params[num_params - 1] == ';')
			{
				jobtype = SEQUENTIAL;
				num_params--;
			}
			else
				jobtype = SEQUENTIAL;
		}
		else
		{
			// See if last parameter ENDS with & or ;
			// and if so, cut it off and set jobtype
			int len = strlen(params[num_params - 1]);
			if (params[num_params - 1][len - 1] == '&')
			{
				params[num_params - 1][len - 1] = '\0';
				jobtype = PARALLEL;
			}
			else if (params[num_params - 1][len - 1] == ';')
			{
				params[num_params - 1][len - 1] = '\0';
				jobtype = SEQUENTIAL;
			}
			else
				jobtype = SEQUENTIAL;
		}
			
		for(int i = 0; i < num_params; i++) // DEBUG ONLY
		{
			printf("%d) %s\n",i,params[i]);
		}
		if (jobtype == SEQUENTIAL) printf("SEQUENTIAL\n"); // DEBUG ONLY
		if (jobtype == PARALLEL) printf("PARALLEL\n"); // DEBUG ONLY
	
	
		// check for special instructions (exit, jobs)
		if (num_params > 0)
		{
			if (!strcmp(params[0],"jobs"))
			{
				// Display jobs list?
			}
			else if (!strcmp(params[0],"exit"))
			{
				// Exit code
			}
			else
			{
				// Build Job
				
				
				
				// Submit job
			
			
			}
		}
	}
	
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