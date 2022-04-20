/**
 * @lsh_loop: is a function
 *
 */

void lsh_loop(void)
{
	char *line;
	char **args;
	int status;

	do {
		printf("cisfun!>>");
		line = hsh_read_line();
		args = hsh_split_line(line);
		status = hsh_execute(args);

		free(line);
		free(args);
	} while (status);
}
