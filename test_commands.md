## Test cases

### Builtins
#### cd
- cd
- cd ..
- cd -
- cd ~
- cd ~/Documents
- cd ~/Documents/
- cd /run
- cd /run/

#### pwd
- pwd

#### export
- export test=test && echo $test

#### unset
- unset USER && echo $USER

#### exit
- exit

### Pipes
- echo a | cat -e
- echo a | cat | cat | cat -e
- echo a | cat >test | cat -e

### Variable expansion
- echo $HOME
- echo \$HOME$
- echo "$HOME"
- echo '$HOME'
- echo $"HOME"
- echo $'HOME
- echo $
- echo $$
- echo $&& echo a
- echo $ && echo a
- echo $HOME&& echo a
- echo a$HOME
- echo a"$HOME"b
- echo "a$HOME"
- echo $notfound
- echo "$""he"
- echo '$''hi'
- echo \$HOME$HOME
- echo \$HOME $HOME
- cat <<stop
	>$HOME
	>stop
- cat <<"stop"
	>$HOME
	>stop
- cat <<'stop'
	>$HOME
	>stop
- cat <<"st"op
	>$HOME
	>stop
- cat <<st'op'
	>$HOME
	>stop

### Redirects

- cat <input
- <input cat
- cat test >output
- >output cat test
- cat test <<heredoc
- cat test >output && cat test >>output
- cat test <input
- cat <input \>output
- cat <input <input2
- cat >output >output2
-	(cat <<he) <notfound
	>jdfsa
	>he

### Syntax errors
- ()
- (())
- ()()
- &&
- |
- echo a &&
- && echo a
- echo a && && echo b
- cat >
- cat > >test
- echo a |
- | echo a
- echo a | | echo b
- echo a | && echo b
