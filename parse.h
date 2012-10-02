#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>

// Parses and issues the jobs for a single line from a given file handle.
// Returns 0 if not EOF, or -1 on EOF
int parse_line(FILE *fp);

#endif