# Shell

A simple shell for Linux implemented in C.  
Shell supports external linux commands, such as:  
  * cat  
  * grep  
  * ls  
  * pwd  
Shell supports internal commands (builtins), such as:  
  * help  
  * cd  
  * exit  
Shell supports quotes escaping, simple two commands piping with |.  
Ctrl-c does not terminate the shell, but a command running in it.  
Shell supports commands history, available with arrows.  
Shell supports tab completion.  

## Getting Started
```
git clone "this repo" shell
cd shell
make
./shell
```

### Prerequisites
```
gcc
```

## Authors

* **Andrii Liashenko**