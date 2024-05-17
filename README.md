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
- **Input:** We read input using `readline` to display the prompt and get the input unless `stdin` is not a tty, in which case we use `get_next_line` from [libft](https://github.com/myryk31415/libft).
- **Tokenization:** The input string gets tokenized into a doubly linked list made up of these tokens:
  - `T_WORD`
  - `T_SEPARATOR`
  - `T_PIPE`
  - `T_AND`
  - `T_OR`
  - `T_LPAREN`
  - `T_RPAREN`
  - `T_REDIR_IN`
  - `T_REDIR_OUT`
  - `T_REDIR_APPEND`
  - `T_REDIR_HEREDOC`
- **Recursive decent:** The token list is recursively converted into a binary tree by splitting it into smaller parts and removing already used tokens.
- The tree consists of the following nodes, each with a `type` variable to identify it:
  - **Basic Binary Node**: With two pointers to the next nodes, used for `AND`, `OR`, and `PIPE`.
  - **Redirect Node**: With one pointer to a new subtree, used for storing redirect information outside of brackets and starting a new subshell. Example: `cd .. && pwd` vs `(cd ..) && pwd`.
  - **Command Node**: With a double character array for the command arguments and another array for redirects specific to this command.
- **Post-Parsing Resolution**: After the tree is parsed, all heredoc redirects are resolved. This ensures that user input for heredoc is not prematurely read, particularly in cases where the overall command syntax is incorrect.

### Execution
#### Overview
Once the input has been parsed into a binary tree structure, the shell enters the execution phase. This phase involves traversing the binary tree, handling each type of node appropriately to execute commands, handle logical operators, manage pipelines, and handle redirections. The following sections outline the steps involved in executing the parsed command tree.

#### Node Types and Their Execution

##### Command Node Execution
- **Description**: Command nodes contain the basic commands and their arguments.
- **Execution Steps**:
  1. **Forking**: The shell forks a new process using `fork()`.
  2. **Redirections**: Before executing the command, the shell sets up input/output redirections based on the redirections list associated with the node.
  3. **Executing**: The child process uses `execve()` to execute the command with the provided arguments.
  4. **Waiting**: The parent process waits for the child process to complete using `waitpid()`.

##### Binary Operator Nodes (`AND`, `OR`, `PIPE`)
- **Description**: These nodes handle logical operators (`&&`, `||`) and pipelines (`|`).
- **Execution Steps**:
  - **AND (`&&`)**:
    1. Execute the left subtree.
    2. If the left subtree succeeds (returns 0), execute the right subtree.
  - **OR (`||`)**:
    1. Execute the left subtree.
    2. If the left subtree fails (returns non-zero), execute the right subtree.
  - **PIPE (`|`)**:
    1. Create a pipe using `pipe()`.
    2. Fork two processes:
       - The first process redirects its stdout to the write end of the pipe and executes the left subtree.
       - The second process redirects its stdin to the read end of the pipe and executes the right subtree.
    3. Close the pipe and wait for both processes to complete.

##### Redirection Nodes
- **Description**: These nodes handle input (`<`), output (`>`), append (`>>`), and heredoc (`<<`) redirections.
- **Execution Steps**:
  1. **Input Redirection (`<`)**:
     - Open the input file and redirect stdin to this file.
  2. **Output Redirection (`>`)**:
     - Open/Create the output file and redirect stdout to this file.
  3. **Append Redirection (`>>`)**:
     - Open/Create the output file in append mode and redirect stdout to this file.
  4. **Heredoc Redirection (`<<`)**:
     - Read input until a specified delimiter is encountered and use this input as stdin for the command.

#### Error Handling
The shell must handle various errors , including:
- Command not found
- Permission denied
- File not found for redirections
- Syntax errors in the command tree

#### Execution Flow
1. **Traverse the Tree**: The shell begins by traversing the root of the binary tree.
2. **Execute Nodes**: Based on the type of node encountered, the appropriate execution function is called.
3. **Handle Operators and Redirections**: Logical operators and redirections are handled according to their semantics.
4. **Monitor Processes**: The shell monitors the execution of commands and handles process termination and exit statuses.
ficiently.

## Tester
The tester is a modivied version of [this](https://github.com/zstenger93/42_minishell_tester) one, so props to [@zstenger93](https://github.com/zstenger93). We added some tests, commented some out that are not in the scope of the subject and rewrote the valgrind test so it also checks for "still reachable" and added 'valgrand bonus' and 'valgrind all' tests.

For more infos have a look at the `README.md` in the `mstest` folder.
You can either copy the whole mstest folder into your project or change the `MINISHELL_PATH` at the top of `tester.sh`.
To execute go into the project folder and type `bash ./mstest/tester.sh`.

