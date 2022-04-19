#ifndef FILE_MAIN
#define FILE_MAIN

int hsh_cd(char **args);
int hsh_help(char **args);
int hsh_exit(char **args);
char *hsh_read_line(void);
char **hsh_split_line(char *line);
int hsh_launch(char **args);
int hsh_execute(char **args);
void hsh_loop(void);

#endif
