#ifndef JOBLIST_H
#define JOBLIST_H

#include <sys/types.h>

typedef enum {
	RUNNING = 1,
	WAITING = 2,
	DONE = 0
} jobstate_t;

typedef enum {
	SEQUENTIAL = 0,
	PARALLEL = 1,
	BUILT-IN
} jobtype_t;

typedef struct{
	jobstate_t job_state;
	jobtype_t  job_type;
	pid_t pid;			// pid associated
	char* prog_name;	// Name of program
	char** params;		// array of string parameters for job 
	size_t size_params; // size of the parameter array
	job_t* next;		// next job in job list
} job_t;

job_t* create_job();
void add_job(job_t* job); //add a job to the job list.
void remove_job(job_t* job); // remove job from job list.
void update_job_state();	// updates the state of each job in the job list.
void print_jobs();			// print out job information for every job in the job list.
void clean_job_list();		// remove all completed jobs from job list.


#endif
