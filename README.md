# Minishell
This project is our own little shell written completely in C.
While it is not as feature complete as minishell or zsh, it provides most features typically used.

## Features
- executing commands
- command history
- pipes: "|"
- operators with priority: "&&", "||", "(", ")"
- redirections: "<", "<<", ">", ">>"
- environment variables: "$"
- quotation marks
- signals: ctrl-C, ctrl-D
- built-ins: cd, pwd, echo, env, export, unset, exit

## Installation
- clone repository
- open folder in terminal
- run "make init_submodules"
- run "make"
- run "./minishell"
A minishell prompt should appear, you can now use the terminal as usual,
to return back to your original shell press ctrl-D or run "exit".
