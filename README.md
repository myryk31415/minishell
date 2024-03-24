# Minishell
This project is our own little shell written completly.
While it is not as feature complete as bash or zsh, it provides most features typically used.

## Features
- executing commands
- command history
- pipelines: "|"
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
