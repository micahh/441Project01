#include <stdio.h>
#include "joblist.h"
#include "executor.h"

/********************* parse.c *******************************************
 * Author: Charles Goetzman and Micah Heyer
 * Date: 09/18/12
 * Description: These functions handle starting jobs and keeping track of job
				numbers.
 *
 ************************************************************************/


static uint32_t job_counter = 0;
static uint32_t jobs_started = 0;
static uint32_t jobs_background = 0;

/* Returns the next available job number.
 * The first job nubmer is always 1.
 */
uint32_t get_next_job_number()
{
	return ++job_counter;
}

int list_jobs()
{
	//Print out the 'jobs' prompt for Part 1 of homework
	uint32_t jobnum = get_next_job_number();
	printf("Job %3d^: <jobs>\n",jobnum);
	// for part 2 will call into joblistc's list_jobs function
	return 0;
}
int exit_notify()
{
	// Print out the 'exit' prompt for Part 1 of homework
	uint32_t jobnum = get_next_job_number();
	printf("Job %3d^: <exit>\n",jobnum);
	printf("Total number of jobs: %d\n",jobs_started);
	printf("Total number of jobs in background: %d\n",jobs_background);
	// for part 2 will call into joblistc's list_jobs also
	return 0;
}


int start_job(job_t *job)
{


	/* Assign a job number to job be for we start it */
	job->job_n = get_next_job_number();
	job->job_state = RUNNING;
	jobs_started++;

	/* print out job information for part 1*/
	/* conditionally display a * to indicate a job is executed in parallel */
	char parallel_char = ' ';
	if(job->job_type == PARALLEL)
	{
		parallel_char = '*';
		jobs_background++;
	}

	printf("Job %3d%c: <%s> ",job->job_n,parallel_char,job->prog_name);
	for(uint32_t i = 0;i < job->size_params;++i)
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
