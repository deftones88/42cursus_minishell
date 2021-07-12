# minishell

Things to be done

1. test cases :  
	- cat | cat | ls -al | cat
 	- echo -n “test\’\\\”\|”
	- echo echo -n “test\’\\\”\|” -n test option
	- '"pwd"' or "'pwd'" (should be command not found)
2. leaks (currently 102 leaks present)
		- suspected to be mainly from parse_key & parse_value
