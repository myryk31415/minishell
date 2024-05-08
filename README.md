# Minishell
This project is our [(@antonweizmann)](https://github.com/antonweizmann) own little shell written completely in C.
While it is not as feature complete as bash or zsh, it provides most features typically used.

## Installation
- clone repository
- open folder in terminal
- run "make init_submodules"
- run "make"
- run "./minishell"
A minishell prompt should appear, you can now use the terminal as usual,
to return back to your original shell press ctrl-D or run "exit".

## Features
- executing commands
- command history
- pipes: "|"
- operators with priority: "&&", "||", "(", ")"
- redirections: "<", "<<", ">", ">>"
- environment variables: "$"
- quotation marks
- signals: ctrl-C, ctrl-D, ctrl-\
- built-ins: cd, pwd, echo, env, export, unset, exit

## Structure of the shell
### Parsing
- We read input using readline to display the prompt and get the input unless stdin is not a tty in which case we use get_next_line from [libft](https://github.com/myryk31415/libft).
- The input string gets tokenized into a double linked list made up of these tokens:
```
	T_WORD, T_SEPARATOR, T_PIPE,
	T_AND, T_OR, T_LPAREN, T_RPAREN,
	T_REDIR_IN, T_REDIR_OUT, T_REDIR_APPEND, T_REDIR_HEREDOC
```
- The token list is recursivly converted into a binary tree by splitting it in smaller parts and removing already used tokens.
- The tree consists of following nodes, each pointer to the next node has a variable `type` as well to identify it:
  - A basic binary node, with two pointers to the next nodes, for `AND`, `OR` and `PIPE`
  - A redirect node, with one pointer to a new subtree, for storing redirect information outside of brackets and starting a new subshell - `cd .. && pwd` vs  `(cd ..) && pwd`
  - A command node with a double character array for the command arguments and another one for redirects for this specific command
- All heredoc redirects get resolved after parsing the tree to avoid issues where input from the user was read even though the syntax of the command was incorrect.
### Execution

## Tester
The tester is a modivied version of [this](https://github.com/zstenger93/42_minishell_tester) one, so props to [@zstenger93](https://github.com/zstenger93). We added some tests, commented some out that are not in the scope of the subject and rewrote the valgrind test so it also checks for "still reachable" and added 'valgrand bonus' and 'valgrind all' tests.

For more infos have a look at the `README.md` in the `mstest` folder.
You can either copy the whole mstest folder into your project or change the `MINISHELL_PATH` at the top of `tester.sh`.
To execute go into the project folder and type `bash ./mstest/tester.sh`.

