# Shell
A simple shell for Linux implemented in C.  

Shell supports external linux commands, such as:  
* cat
* grep
* ls
* pwd
* ...
  
Shell supports internal commands (builtins), such as:  
* help
* cd
* exit

## Features

- Quotes escaping
- Simple commands piping  
- Commands history
- Tab completion
- Ctrl-c does not terminate the shell, but terminates a command currently running

## Getting Started
```
git clone https://github.com/liashenko/shell.git shell
cd shell
make
./shell
```

## TODO

* output/input redirection
* multiple commands piping
* globbing
* multiple commands execution

## Authors

* **Andrii Liashenko**
