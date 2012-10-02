#ifndef JOBLIST_H
#define JOBLIST_H

#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>

/********************* joblist.h *****************************
 * Author: Micah Heyer
 * Date: 09/16/12
 * Description: This file contains the data types and support 
 * 				functions for the job list data structure.
 * 				This structure is used for managing the global 
 * 				list of jobs currently being processed by the shell.
 * 				It is implemented as a linked list.
 *
 *
 *************************************************************/

typedef enum 
{
	RUNNING = 1,
	WAITING = 2,
	DONE = 3
} jobstate_t;

typedef enum 
{
	SEQUENTIAL = 0,
	PARALLEL = 1,
	BUILT_IN
} jobtype_t;

typedef struct job_t
{
	jobstate_t job_state;
	jobtype_t  job_type;
	pid_t pid;						// pid associated
	char* prog_name;				// Name of program
	char** params;					// array of string parameters for job 
	uint32_t size_params; 			// size of the parameter array
	uint32_t job_n;					// the job number
	struct job_t* next;				// next job in job list
} job_t;

//add a job to the job list.
int add_job(job_t* job); 		

// updates the state of each job in the job list.
int update_job_list_state();		

// print out job information for every job in the job list.
void print_job_list();				

/* de-allocates the job referenced by 'job'.
 * Note: this function doesn't remove the job from the job list.
 * 		 It is assumed to not belong to the joblist. remove_job 
 * 		 should be used instead to remove  a job from the job list.
 */
int free_job();						

/* removes the job 'job' from the list and frees the memory pointed to it. Previous must be the
 * job immediately before 'job'.
 * Returns non-zero if job couldn't be removed.
 */
int remove_job(job_t* prev,job_t* job);

// remove all completed jobs from job list. Jobs are deleted (de-allocated) when removed.
int clean_job_list();		



#endif
