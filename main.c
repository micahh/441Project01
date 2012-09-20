
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "parse.h"

const char* DEFAULT_SHELL_NAME="my_shell";

int main(int argc,char** argv)
{
	

	/*check to see if there are any input files to run in batch mode */
	if(argc > 1)
	{
		for(uint32_t i = 1;i < argc; ++i)
		{
			FILE* inputf = fopen(argv[i],"r");
			if(inputf == NULL)
			{
				fprintf(stderr, "Error: could not open file '%s'.\n",argv[i]);
				continue;
			}

			while(!parse_line(inputf));

			fclose(inputf);
		}
	}
	else //interactive mode
	{
		const char* shell_name = getenv("USER");
		if(NULL == shell_name) shell_name = DEFAULT_SHELL_NAME;
		do
		{
			printf("%s$ ",shell_name);
		}while(!parse_line(stdin));

	}

	return 0;
}
