
#include "parse.h"
#include "joblist.h"
#include "executor.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/********************* parse.c *******************************************
 * Author: Charles Goetzman
 * Date: 09/17/12
 * Description: This file handles reading input strings and convering them
 *			into job_t structures. It also identifies the special commands
 *			'exit' and 'jobs'.
 *
 ************************************************************************/

const unsigned int MAX_INPUT_LENGTH = 4096; 

int process_statement(char *start, char *end);
char *copy_string(char *start, char *end);
char **tokenize_string(char *buff, uint32_t *num_params);
jobtype_t get_jobtype(char **params, uint32_t *num_params);

// Parses and issues the jobs for a single line from a given file handle.
// Returns 0 if not EOF, or -1 on EOF
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
				if (process_statement(start,end))
					return -1;
				// Done
				break;
			} 
			else if (*end == ';' || *end == '&') // separator
			{
				// Process line
				if (process_statement(start,end))
					return -1;
				// Move to next portion
				start = end = (end + 1);
			}
			else
				end++;
		}
		
	}
	else
	{
		// end of file or input problem
		exit_notify();	// dsplay current job status
		return -1;
	}

	return 0;
}

// Performs all the processing on a substring that begins
// at 'start' and finishes at 'end', the original char
// array is not modified.
int process_statement(char *start, char *end)
{
	int exit = 0; // This will be the return value
	
	// Isolate the command and copy it to a string
	char *buff = copy_string(start,end);
	if (buff == NULL) return -1;
	
	// buff is now a valid string containing a single 'command'
	
	// tokenize the command
	uint32_t num_params = 0;
	char **params = NULL;
	params = tokenize_string(buff, &num_params);
	
	// check there is at least one non-empty parameter
	
	jobtype_t jobtype = get_jobtype(params,&num_params);
	
	// check for special instructions (exit, jobs)
	if (num_params > 0)
	{
		if (!strcmp(params[0],"jobs"))
		{
			// Display jobs list?
			list_jobs();
		}
		else if (!strcmp(params[0],"exit"))
		{
			// Exit code
			exit_notify();
			exit = 1;
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
	
	
	return exit;
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


// Tokenizes a string, allocs the 2d string array. The individual tokens
// are pointers into the original tokenized string, so no new storage is
// created for the tokens themselves, only the token array.
char **tokenize_string(char *buff, uint32_t *num_params)
{
	char *temp_str = NULL;
	char **params = NULL;
	
	for(temp_str = strtok(buff, " \t\n\0"); temp_str != NULL; temp_str = strtok(NULL, " \t\n\0"))
	{
		params = (char**)realloc(params, (sizeof(char*) * ((*num_params) + 1)));
		if (params == NULL)
		{
			fprintf(stderr, "Fatal error allocating storage while parsing.\n");
			exit(1);
		}
		params[*num_params] = temp_str;
		(*num_params)++;
	}
	return params;
}

// Analyzes the tokens to find the jobtype of the command
// by looking for & or ; symbols at the end. WILL modify
// the num_params value, and/or erase the symbols from params
// as necessary. 
jobtype_t get_jobtype(char **params, uint32_t *num_params)
{
	jobtype_t jobtype = 0;
	// If there are no parameters, just return default jobtype
	if (*num_params < 1) return jobtype; 
		
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
