#include "minishell.h"

void sig_handler(int sig)
{
  printf("sigint : %d\n", sig);
  // rl_replace_line("\n", 0);
}
