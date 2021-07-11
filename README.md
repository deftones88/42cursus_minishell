# minishell

Things to be done

1. test cases :  
	- cat | cat | ls -al | cat
	- cat << "$_"  (or any other environment variable)
 	- echo -n “test\’\\\”\|”
	- echo echo -n “test\’\\\”\|” -n test option
	- echo hi       "hello""bye"    | cat -e
	- '"pwd"' or "'pwd'" (should be command not found)
2. weird space at the end of string
	- echo "hi" | cat -e (gives 'hi $') 
3. '\\' - what to do with this?
4. leaks (currently 102 leaks present)
		- suspected to be from parse_key & parse_value
