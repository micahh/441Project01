#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdio.h>
#include "joblist.h"

// The publicly visible functions for executor.c

/* start_job
 * 
 * Description: This function spawns a new child process and executes the command described in 'job'.
 * Parameters: 'job' a job information structure that describes the command to execute.
 * Returns: 0
 */
int start_job(job_t *job);

/* list_jobs
 * Description: Prints a summery of all jobs currently running in the background and those that have finished after the
 * 				last invocation of this function.
 * Parameters: None
 * Returns: 0
 */
int list_jobs();

/* list_jobs
 * Description: Prints a summery of all jobs that have been started since the time this program was started. The total jobs number is the count
 * 				of every job started since the program began. The background job count is the number of jobs started in background regardless
 * 				of there current state.
 * Parameters: None
 * Returns: 0
 */
int exit_notify();

#endif
