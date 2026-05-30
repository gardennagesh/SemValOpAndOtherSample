/* Programmer: Nagesh Nanjundachari */
/* Demo for 2D-array of Pointer using execvp */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX_OPT 10

char *cmd[][MAX_OPT] = { 
                    //Indexed as 0
                    {"df", "-h", NULL},  
                    //Indexed as 1.
                    {"ls", "-l", "-r", "-t", NULL},
                    //Indexed as N.
                    {"MyApp", "MyOpts", NULL},
                  };

int main (int argc, char *argv[])
{
  int pfd[2];
  char buf[2] = { 0 };
  pipe (pfd);

  if (!fork ())
    {
      close(pfd[1]);
      read (pfd[0], buf, 1);
      printf ("\n\n.....Child proc recvd index=%d from parent proc.....\n\n", atoi (buf));
      execvp (cmd[atoi (buf)][0], cmd[atoi (buf)]);
    }
  else {
      close(pfd[0]);
      // No chk made for argv val. So just 0 or 1.
      write (pfd[1], argv[1], 1);
  }
}
