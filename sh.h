#include <stdbool.h> 
#include "get_path.h"

int pid;
int sh( int argc, char **argv, char **envp);
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void list ( char *dir );
int cd ( char *dir );
// void exit();
void printenv(char **envp);
bool startsWith(const char *pre, const char *str);
#define PROMPTMAX 32
#define MAXARGS 10
