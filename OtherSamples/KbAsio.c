/* Programmer: Nagesh Nanjundachari */
/* Demo ASIO using KB Hit */
/* A bug found in Terminal-Session 2nd time run */
/* Open new terminal-session for 2nd time run */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define STD_IN  0
#define STD_OUT 1

void handle_sig (int sigNo)
{

  if (sigNo == SIGIO)
    write (STD_OUT, "KB_HIT", 6);
  if (sigNo == SIGTERM || sigNo == SIGINT)
    write (STD_OUT, "Process recvd term or inter", 32);
}

struct sigaction sig_act;

int main ()
{

  sig_act.sa_handler = handle_sig;
  sig_act.sa_flags = 0;

  sigaction (SIGIO, &sig_act, NULL);
  sigaction (SIGINT, &sig_act, NULL);
  fcntl (STD_IN, F_SETFL, O_ASYNC);
  fprintf (stdout, "%s\n", "Enter Key");
  getchar ();
  fprintf (stdout, "%s\n", "Terminate or Interrupt the process");
  while (1)
    sleep (2);
}
