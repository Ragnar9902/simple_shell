#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "main.h"

/**
 * @main:
 *
 */


int main(void)
{
	char *line;
	char **args;
	int status;

	do {
		printf("> ");
		line = hsh_read_line();
		args = hsh_split_line(line);
		status = hsh_execute(args);

		free(line);
		free(args);
	} while (status);

	return (1);
}
