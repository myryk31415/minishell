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
	$HOME
	stop
- cat <<"stop"
	$HOME
	stop
- cat <<'stop'
	$HOME
	stop
- cat <<"st"op
	$HOME
	stop
- cat <<st'op'
	$HOME
	stop
