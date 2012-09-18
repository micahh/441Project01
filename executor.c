#include <stdio.h>
#include "joblist.h"
#include "executor.h"

static int job_counter = 0;

/* Returns the next available job number.
 * The first job nubmer is always 1.
 */
int get_next_job_number()
{
	return ++job_counter;
}

int start_job(job_t *job)
{


	/* Assign a job number to job be for we start it */
	job->job_n = get_next_job_number();
	job->job_state = RUNNING;

	/* print out job information for part 1*/
	/* conditionally display a * to indicate a job is executed in parallel */
	char parallel_char = ' ';
	if(job->job_type == PARALLEL)
		parallel_char = '*';

	printf("Job %d%c: <%s> ",job->job_n,parallel_char,job->prog_name);
	for(size_t i = 0;i < job->size_params;++i)
	{
		printf("[%s] ",job->params[i]);
	}
	printf("\n");

	/* clean up job */
	free_job(job);

	
	/*
	// For PROJECT PART 2:
	
	

	// Check if the job is Parallel or Sequential
	
	// If serial:
	// Simply execute the job immediately as-is, wait() on process, then free it (never used again)
	
	
	// If parallel:
	// Start the job immediately, add it to the active joblist (don't free it)
	*/
	
	return 0;
}
