#include <stdlib.h>
#include <stdio.h>
#include "joblist.h"

static job_t root = {RUNNING,BUILT_IN,0,NULL,NULL,0,0,NULL};
static size_t job_list_size = 0;
static int job_counter = 0;



int add_job(job_t* job){
	if(NULL == job) return -1;
	job_t* tail = &root;
	for(; tail->next != NULL; tail = tail->next);
	tail->next = job;
	++job_list_size;

	return 0;
}

/* removes the job 'job' from the list and frees the memory pointed to it. Previous must be the
 * job immediately before 'job'. */
int remove_job(job_t* prev,job_t* job){
	if(NULL == job || NULL == prev || prev->next != job) return -1; 
	prev->next = job->next;
			/*de-allocate job */
			job->next = NULL;
			if(job->prog_name != NULL)free(job->prog_name);
			job->prog_name = NULL;

			if(job->params != NULL){
				for(size_t i = 0; i < job->size_params; ++i){
					if(job->params[i] != NULL) free(job->params[i]);
					job->params[i] = NULL;
				}
				free(job->params);
				job->params = NULL;
			}
			free(job);
			--job_list_size;
			return 0;
	return -1; // object not found
}

int update_job_list_state(){
	for(job_t* j = &root; j->next != NULL; j = j->next){
		j->job_state = DONE;
	}
	return 0;
}

int clean_job_list(){
	for(job_t* j = &root; j->next != NULL;){
		if(j->job_state == DONE){
			int error = remove_job(j,j->next);
			if(error) return error;
			continue;
		}
	   	j = j->next;
	}

	return 0;
}

void print_job_list(){
	for(job_t* j = &root; j->next != NULL; j = j->next){
		job_t* cur = j->next;
		if(cur->job_type == PARALLEL)printf("Job %d*: <%s> ",cur->job_n,cur->prog_name);
		else printf("Job %d : <%s> ",cur->job_n,cur->prog_name);
		for(size_t i = 0;i < cur->size_params;++i){
			printf("[%s] ",cur->params[i]);
		}
		printf("\n");
	}
}

int get_next_job_number(){
	return ++job_counter;
}