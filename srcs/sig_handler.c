#include "sig_handler.h"

void sig_handler(int sig)
{
  printf("sigint : %d\n", sig);
  // rl_replace_line("\n", 0);
  // rl_redisplay();
  // rl_on_new_line();
}
