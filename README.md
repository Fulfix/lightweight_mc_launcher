# Installation 
First you have to clone the repo:
```
git clone https://github.com/Fulfix/lightweight_mc_launcher
```

## Configuration

Now you can edit the config.h file as you wish. This file contains the command used to launch minecraft and the index of 3 arguments the username, the uuid and the token.

Inside config.h you can find an example but it won't work
### tips

To find the command used to launch minecraft:
- First launch the game with your usual launcher
- run `cat /proc/$(pgrep java)/cmdline | tr -s '\0' ' '`
- copy the command
- create a little python script
```
import sys 
print(sys.argv)
```
- run `python3 ./path/to/the/script.py <paste the command>`
- replace `[]` by `{}`

## Compilation
inside the repo's directory execute:
```
make
```
otherwise if your on windows execute:
```
make windows
```
# TODO

- check the content of the requests
