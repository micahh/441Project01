
#include "parse.h"
#include "joblist.h"
#include "executor.h"
#include <stdlib.h>
#include <string.h>

const unsigned int MAX_INPUT_LENGTH = 4096; 
const unsigned int MAX_NUM_PARAMS = 2048; 

int process_statement(char *start, char *end);
char *copy_string(char *start, char *end);
char **tokenize_string(char *buff, int *num_params);
jobtype_t get_jobtype(char **params, int *num_params);

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
	char *buff = copy_string(start,end);
	if (buff == NULL) return -1;
	
	// buff is now a valid string containing a single 'command'
	//printf("[%s]\n",buff); // DEBUG ONLY
	
	// tokenize the command
	int num_params = 0;
	char **params = NULL;
	params = tokenize_string(buff, &num_params);
	
	
	//printf("%d parameters tokenized.\n",num_params); // DEBUG ONLY
	
	// check there is at least one non-empty parameter
	
	
	if (num_params > 0)
	{
		jobtype_t jobtype = get_jobtype(params,&num_params);
		
		/*
		for(int i = 0; i < num_params; i++) // DEBUG ONLY
		{
			printf("%d) %s\n",i,params[i]);
		}
		if (jobtype == SEQUENTIAL) printf("SEQUENTIAL\n"); // DEBUG ONLY
		if (jobtype == PARALLEL) printf("PARALLEL\n"); // DEBUG ONLY
		*/
	
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
				exit(0);
			}
			else
			{
				// Build Job
				job_t *job = malloc(sizeof(job_t));
				job->job_state = WAITING;
				job->job_type = jobtype;
				job->pid = 0; // necessary?
				job->prog_name = strdup(params[0]);
				if (num_params > 1)
				{
					job->params = malloc(sizeof(char *) * (num_params - 1));
					for(int i = 1; i < num_params; i++)
					{
						job->params[i - 1] = strdup(params[i]);
					}
					job->size_params = num_params - 1;
				}
				else
				{
					job->params = NULL;
					job->size_params = 0;
				}
				job->job_n = -1; // Will be filled in by start method...
				
				// Submit job
				start_job(job);
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

// Copies a portion of memory from start to end into a new string
// Length will be increased by 1 for a null termination
char *copy_string(char *start, char *end)
{
	if (start > end) return NULL; // Empty
	
	char *buff = NULL;
	buff = malloc(  (end - start) + 2 );
	
	memcpy(buff,start,(end - start) + 1);
	buff[ (end - start) + 1] = '\0';
	
	return buff;
}


// Tokenizes a string, calling strdup (and thus malloc) for each
// token and also allocs the 2d string array.
char **tokenize_string(char *buff, int *num_params)
{
	char *temp_str = NULL;
	char **params = NULL;
	
	for(temp_str = strtok(buff, " \t"); temp_str != NULL; temp_str = strtok(NULL, " \t"))
	{
		params = (char**)realloc(params, (sizeof(char*) * ((*num_params) + 1)));
		if (params == NULL)
		{
			fprintf(stderr, "Fatal error allocating storage while parsing.");
			// TODO: exit program...
		}
		// TODO: Can we eliminate strdup here, and duplicate INTO the jobs?
		//params[*num_params] = strdup(temp_str);
		params[*num_params] = temp_str;
		(*num_params)++;
	}
	return params;
}

// Analyzes the tokens to find the jobtype of the command
// by looking for & or ; symbols at the end. WILL modify
// the num_params value, and/or erase the symbols from params
// as necessary. 
jobtype_t get_jobtype(char **params, int *num_params)
{
	jobtype_t jobtype;
		
		if (strlen(params[(*num_params) - 1]) == 1)
		{
			// Last parameter is a single character, see if it was & or ;
			// and if so, remove that parameter and set jobtype
			if (*params[(*num_params) - 1] == '&')
			{
				jobtype = PARALLEL;
				(*num_params)--;
			}
			else if (*params[(*num_params) - 1] == ';')
			{
				jobtype = SEQUENTIAL;
				(*num_params)--;
			}
			else
				jobtype = SEQUENTIAL;
		}
		else
		{
			// See if last parameter ENDS with & or ;
			// and if so, cut it off and set jobtype
			int len = strlen(params[(*num_params) - 1]);
			if (params[(*num_params) - 1][len - 1] == '&')
			{
				params[(*num_params) - 1][len - 1] = '\0';
				jobtype = PARALLEL;
			}
			else if (params[(*num_params) - 1][len - 1] == ';')
			{
				params[(*num_params) - 1][len - 1] = '\0';
				jobtype = SEQUENTIAL;
			}
			else
				jobtype = SEQUENTIAL;
		}
	return jobtype;
}
