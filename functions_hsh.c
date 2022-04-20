#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stddef.h>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
extern char **environ;
/*
 * Function Declarations for builtin shell commands:
 */
int hsh_cd(char **args);
int hsh_help(char **args);
int hsh_exit(char **args);
int hsh_env(char **args);
/*
 * List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
	"cd",
	"help",
	"exit",
	"env"
};

int (*builtin_func[]) (char **) = {
	&hsh_cd,
	&hsh_help,
	&hsh_exit,
	&hsh_env
};


int hsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);

}

/*
 * Builtin function implementations.
*/
int hsh_cd(char **args)
{

    if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;

}

int hsh_help(char **args)
{
	int i;
	if (args[1] == NULL){
	}
  	printf("simple shell\n");
  	printf("Type program names and arguments, and hit enter.\n");
  	printf("The following are built in:\n");

  	for (i = 0; i < hsh_num_builtins(); i++) {
    		printf("  %s\n", builtin_str[i]);
  	}

  	printf("Use the man command for information on other programs.\n");
  	return 1;
	
}

int hsh_exit(char **args)
{
	if (args[1] == NULL){
		printf("exit of the shell");
        }


	return 0;

}

int hsh_env(char **args)
{
	char **s = environ;
	if (args[1] == NULL){
        	printf("exit of the shell");
        }


  	for (; *s; s++) {
    	printf("%s\n", *s);
  	}

  	return 0;
}

char *hsh_read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0; /* have getline allocate a buffer for us */

  if (getline(&line,&bufsize, stdin) == -1){
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  /* We recieved an EOF*/
    } else  {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

	return (line);
}

char **hsh_split_line(char *line)
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return (tokens);
}

int hsh_launch(char **args)
{
	pid_t pid;
	int status;

	if (args[0] == NULL) {
    		/* An empty command was entered.*/
    		return 1;
	}


  	pid = fork();
  	if (pid == 0) {
    	/* Child process*/
    	if (execvp(args[0], args) == -1) {
      	perror("lsh");
    	}
    	exit(EXIT_FAILURE);
  	} else if (pid < 0) {
    	/* Error forking*/
    	perror("lsh");
  	} else {
    	/* Parent process*/
    	
      		waitpid(pid, &status, WUNTRACED);
    	
  	}

	return (1);
}

int hsh_execute(char **args)
{
	int i;

  if (args[0] == NULL) {
    /* An empty command was entered.*/
    return 1;
  }

  for (i = 0; i < hsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

	return (hsh_launch(args));
}

void hsh_loop(void)
{
	char *line;
	char **args;
	int status;

  do {
    printf("#cisfun$");
    line = hsh_read_line();
    args = hsh_split_line(line);
    status = hsh_execute(args);

	free(line);
	free(args);
	} while (status);
}
