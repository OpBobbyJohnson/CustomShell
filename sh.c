#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"

int sh(int argc, char **argv, char **envp)
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char *));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid);   /* get passwd info */
  homedir = password_entry->pw_dir; /* Home directory to start
						  out with*/

  if ((pwd = getcwd(NULL, PATH_MAX + 1)) == NULL)
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' ';
  prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();

  while (go)
  {
    char buffer[BUFSIZ];
    int len;
    /* print your prompt */
    printf("%s:-->", homedir);
    if (fgets(buffer, BUFSIZ, stdin) != NULL)
    {
      len = (int)strlen(buffer);
      printf("length [%d] of string %s", len, buffer); // notice the length!
                                                       /* get command line and process */
      // Handle exit command
      if (strcmp(buffer, "exit\n") == 0)
      {
        printf("Exit");
      }
      // Handle exec command
      else if (strcmp(buffer,"exec\n") == 0) {
        printf("Exec");
      }
      // Handle which command
      else if (strcmp(buffer,"which\n") == 0) {
        printf("Which");
      }
      // Handle where command 
      else if (strcmp(buffer,"where\n") == 0) {
        printf("Where");
      }
      /* check for each built in command and implement */

      /*  else  program to exec */
      // {
      /* find it */
      /* do fork(), execve() and waitpid() */

      /* else */
      // aww shit command not found :(
      else
      {
        fprintf(stderr, "%s: Command not found.\n", args[0]);
      }
      printf("\n");
    }
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist)
{
  /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */

} /* which() */

char *where(char *command, struct pathelement *pathlist)
{
  /* similarly loop through finding all locations of command */
} /* where() */

void list(char *dir)
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */
