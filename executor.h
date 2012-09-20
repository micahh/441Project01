#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdio.h>
#include "joblist.h"

int start_job(job_t *job);
int list_jobs();
int exit_notify();

#endif