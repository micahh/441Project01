

#include "parse.h"

int main(int argc, char **args)
{
	while(1)
	{
		printf("SHELL$");
		
		parse_line(stdin);
	}
}