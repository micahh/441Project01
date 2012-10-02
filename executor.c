#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
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


/* list_jobs
 * Description: Prints a summery of all jobs currently running in the background and those that have finished after the
 * 				last invocation of this function.
 * Parameters: None
 * Returns: 0
 */
int list_jobs()
{
	//Print out the 'jobs' prompt for Part 1 of homework
	//uint32_t jobnum = get_next_job_number();
	//printf("Job %3d^: <jobs>\n",jobnum);
	// for part 2 will call into joblistc's list_jobs function
	update_job_list_state();
	print_job_list();
	clean_job_list();

	return 0;
}

/* list_jobs
 * Description: Prints a summery of all jobs that have been started since the time this program was started. The total jobs number is the count
 * 				of every job started since the program began. The background job count is the number of jobs started in background regardless
 * 				of there current state.
 * Parameters: None
 * Returns: 0
 */
int exit_notify()
{
	printf("Total number of jobs: %d\n",jobs_started);
	printf("Total number of jobs in background: %d\n",jobs_background);
	// for part 2 will call into joblistc's list_jobs also
	return 0;
}

/* execute_job
 * 
 * Description: Allocates argument list and invokes the execvp system call
 * Parameters: 'job' - the job information structure to use to create new program.
 * Return: returns the error code from execvp on error, otherwise this function does not return.
 *
 */

int execute_job(job_t* job)
{
	int argc = job->size_params + 2; //number of parameters plus 1 for program name and plus 1 for NULL terminating argument.
	char** args = (char**)malloc(sizeof(char**)*argc);
	char* prog_name = strdup(job->prog_name);
	args[0] = strdup(job->prog_name);
	for(uint32_t i = 0; i < job->size_params ; ++i)
	{
		args[i+1] = strdup(job->params[i]);
	}
	args[argc-1] = NULL;

	return execvp(prog_name,args);
}


/* start_job
 * 
 * Description: This function spawns a new child process and executes the command described in 'job'.
 * Parameters: 'job' a job information structure that describes the command to execute.
 * Returns: 0
 */
int start_job(job_t *job)
{


	/* Assign a job number to job be for we start it */
	job->job_n = get_next_job_number();
	jobs_started++;
	// For PROJECT PART 2:

	// Check if the job is Parallel or Sequential
	int parallel = (job->job_type == PARALLEL);

	// If serial:
	// Simply execute the job immediately as-is, wait() on process, then free it (never used again)
	pid_t c_pid = fork();
	if(c_pid < 0)
	{
		fprintf(stderr, "Error: failed to fork.\n");
		return -1;
	}
	else if(c_pid == 0) // child process
	{
		int exec_err = execute_job(job);
		if(errno == ENOENT) /*file not found errors */
		{
			fprintf(stderr,"Error: command not found '%s'.\n", job->prog_name);
			exit(0);
		}
		/* print any other errors errors */
		fprintf(stderr, "Error: could not execute command '%s'. Error code %d.\n", job->prog_name,exec_err);
		exit(0);
	}
	else if(c_pid > 0)
	{
		int status = 0;
		if(!parallel)
		{
			waitpid(c_pid,&status,0);
			if(status != 0)
			{
				fprintf(stderr,"Command %s terminated with value of %d.\n",job->prog_name,status);
				return 0;
			}
			free_job(job);
		}
		else
		{
			jobs_background++;
			job->pid = c_pid;
			job->job_state = RUNNING;
			add_job(job);
		}
	}
	
	return 0;
}

