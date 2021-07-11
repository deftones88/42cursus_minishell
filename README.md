# minishell

Things to be done

1. cat | cat | ls -al | cat
2. '\\' - what to do with this?
3. leaks (currently 102 leaks present)
4. one argument problem : exits when pipe command is not used beforehand (exit code from waitpid fixed to 0 -> sigint)
