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
#include <stdbool.h>
#include "sh.h"
#include <dirent.h>
#include <errno.h>

char *homedir, *currdir;
int uid;
int sh(int argc, char **argv, char **envp)
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char *));
  int i, status, argsct, go = 1;
  struct passwd *password_entry;
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
  currdir = pwd;
  prompt[0] = ' ';
  prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();
  // char *testArgs[] = {(char*) 0};
  // char *tenv_args[] = {"PATH=/bin", (char*)0};
  // execve("/bin/ls",NULL,NULL);
  // perror( "execve failed" );
  // exit( EXIT_FAILURE );
  while (go)
  {
    char buffer[BUFSIZ];
    int len;
    /* print your prompt */
    printf("%s:-->", currdir);
    if (fgets(buffer, BUFSIZ, stdin) != NULL)
    {
      len = (int)strlen(buffer);
      char *command = strtok(buffer, " ");
      // strcat(command,"\0");
      // printf("%s", command);
      // printf("\n");
      // Handle exit command
      if (strcmp(command, "exit") == 0)
      {
        printf("Exit");
      }
      // Handle which command
      else if (strcmp(command, "which") == 0)
      {

        char *program = strtok(NULL, " ");
        char *ans = which(program, pathlist);
        printf("%s\n", ans);
      }
      // Handle where command
      else if (strcmp(command, "where") == 0)
      {
        char *program = strtok(NULL, " ");
        char *ans = where(program, pathlist);
        printf("%s", ans);
      }
      else if (strcmp(command, "cd\n") == 0 || strcmp(command, "cd") == 0)
      {
        char *path = strtok(NULL, " ");
        cd(path);
      }
      // else if (strcmp(command, "ls\n") == 0 || strcmp(command, "ls") == 0)
      // {
      //   char *path = strtok(NULL, " ");
      //   printf("running ls");
      //   list(path);
      // }
      else if (strcmp(command, "exit\n") == 0 || strcmp(command, "exit") == 0)
      {
        printf("Exiting...");
        exit(0);
      }

      /* check for each built in command and implement */

      /*  else  program to exec */
      // {
      /* find it */
      /* do fork(), execve() and waitpid() */

      /* else */
      // try to execute the command
      else
      {
        // strcat(command, " ");
        // strcat(command, buffer);
        // printf("%s", command);
        char *b[512];
        strcpy(b, command);
        char *p = strtok(NULL, " ");
        while (p != NULL)
        {
          // printf("%s", p);
          strcat(b, " ");
          strcat(b, p);
          p = strtok(NULL, " ");
        }
        // printf("Command is: %s\n", b);
        int res = system(b);
        // char *params[10];
        // int index = 0;
        // char *param = strtok(NULL, " ");
        // while (param != NULL)
        // {
        //   printf("%s", param);
        //   params[index] = param;
        //   index++;
        //   param = strtok(NULL, " ");
        // }.
        // fprintf(stderr, "%s: Command not found.\n", args[0]);
      }
    }
  }
  return 0;
} /* sh() */

// helper function to check if a string starts with a specific substirng
bool startsWith(const char *pre, const char *str)
{
  size_t lenpre = strlen(pre),
         lenstr = strlen(str);
  return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

char *which(char *command, struct pathelement *pathlist)
{
  // check to see if an absolute path is given
  if (startsWith("/", command) == true || startsWith("./", command) == true || startsWith("../", command) == true)
  {
    // printf("Absolute path\n");
    char path[strlen(currdir) + strlen(command) + 10];
    // copy the current directory to path so you arent modifying the current directory but the copy
    strcat(path, currdir);
    strcat(path, "/");
    strcat(path, command);
    int res = access(command, X_OK);
    if (res == 0)
    {
      return command;
    }
  }
  else
  {
    // printf("Check env path\n");
    struct pathelement *cur = pathlist;
    while (cur != NULL)
    {
      char *buf = malloc(sizeof(cur->element) + 5 + sizeof(command));
      strcpy(buf, cur->element);
      strcat(buf, "/");
      strcat(buf, command);
      // remove null character
      buf[strlen(buf) - 1] = 0;
      int res = access(buf, X_OK);
      if (res == 0)
      {
        // it exists
        return buf;
      }
      cur = cur->next;
    }
    // it doesnt exits or its not executable
  }
  return "Could not find the program you were looking for";

  /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */

} /* which() */

char *where(char *command, struct pathelement *pathlist)
{
  // check to see if an absolute path is given
  if (startsWith("./", command) == true || startsWith("../", command) == true)
  {
    char path[strlen(currdir) + strlen(command) + 10];
    // copy the current directory to path so you arent modifying the current directory but the copy
    strcat(path, currdir);
    strcat(path, "/");
    strcat(path, command);
    int res = access(command, X_OK);
    if (res == 0)
    {
      return command;
    }
  }
  else
  {
    struct pathelement *cur = pathlist;
    char *paths;
    while (cur != NULL)
    {
      char *buf = malloc(sizeof(cur->element) + 5 + sizeof(command));
      strcpy(buf, cur->element);
      strcat(buf, "/");
      strcat(buf, command);
      // remove null character
      buf[strlen(buf) - 1] = 0;
      int res = access(buf, X_OK);
      if (res == 0)
      {
        // it exists
        strcat(paths, buf);
        strcat(paths, "\n");
      }
      cur = cur->next;
      free(buf);
    }
    strcat(paths, "\0");
    return paths;
    // it doesnt exits or its not executable
  }
  return "Could not find the program you were looking for";
  /* similarly loop through finding all locations of command */
} /* where() */

// implementation of cd command
int cd(char *dir)
{
  if (dir == NULL)
  {
    // no params given, navigate to home directory
    strcpy(currdir, homedir);
    chdir(currdir);
    return 0;
  }
  else
  {
    strcat(dir,"\0");
    if (startsWith("/", dir))
    {
      char *newdir = strtok(dir, "\n");
      DIR *checkdir = opendir(newdir);
      if (checkdir)
      {
        /* Directory exists. */
        closedir(checkdir);
        strcpy(currdir, newdir);
        chdir(currdir);
        return 0;
      }
      else if (ENOENT == errno)
      {
        printf("Could not find the directory you are looking for\n");
        /* Directory does not exist. */
        return 0;
      }
      else
      {
        return 1;
        /* opendir() failed for some other reason. */
      }
    }
    else if (startsWith("../", dir))
    {
      char *copy;
      char *newPath;
      strcpy(copy, currdir);
      strtok(copy, "/");
    }
    else if(startsWith("./",dir)) {
      char *newdir = strtok(dir, "\n");
      char* tmp = malloc(sizeof(newdir) + sizeof(currdir) + 10);
      strcpy(tmp,currdir);
      memmove(newdir,newdir + 2, strlen(newdir) - 1);
      strcat(tmp,"/");
      strcat(tmp,newdir);
      DIR *checkdir = opendir(tmp);
      if (checkdir)
      {
        /* Directory exists. */
        closedir(checkdir);
        strcpy(currdir, tmp);
        chdir(currdir);
        free(tmp);
        return 0;
      }
      else if (ENOENT == errno)
      {
        free(tmp);
        printf("Could not find the directory you are looking for\n");
        /* Directory does not exist. */
        return 0;
      }
      else
      {
        free(tmp);
        return 1;
        /* opendir() failed for some other reason. */
      }
    }
    else
    {
      char *newdir = strtok(dir, "\n");
      char* tmp = malloc(sizeof(newdir) + sizeof(currdir) + 10);
      strcpy(tmp,currdir);
      strcat(tmp,"/");
      strcat(tmp,newdir);
      DIR *checkdir = opendir(tmp);
      if (checkdir)
      {
        /* Directory exists. */
        closedir(checkdir);
        strcpy(currdir, tmp);
        chdir(currdir);
        free(tmp);
        return 0;
      }
      else if (ENOENT == errno)
      {
        free(tmp);
        printf("Could not find the directory you are looking for\n");
        /* Directory does not exist. */
        return 0;
      }
      else
      {
        free(tmp);
        return 1;
        /* opendir() failed for some other reason. */
      }
    }
  }
  return 1;
}

// void exit()
// {
//   printf("%d",uid);
//   exit(0);
// }

void list(char *dir)
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
  struct dirent *ent;

  while ((ent = readdir(dir)) != NULL)
  {
    printf("%s\n", ent->d_name);
  }
  closedir(dir);
  /* list() */
}
