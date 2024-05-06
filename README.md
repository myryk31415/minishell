# Minishell
This project is our own (@antonweizmann) little shell written completely in C.
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
