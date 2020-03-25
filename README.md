# unixShell
A simple shell program for unix-like system that supports Linux shell commands in addition to the internal commands. This shell can generate as much processes are needed to handle as user's input.
There is only one rule in this shell is prioritizing the internal command over built-in command.
This shell able to execute multiple command chain with I/O redirection no problems.

Notice: NEED TO MANUALLY CONFIGURE PATH

Built-in command:
    quit: quit shell
    path: show path of shell
        supporting flag: + /pathToAdd 
                 - /pathToRemove
        (only add and remove 1 path at a time)
    cd: change directory to /home, if no directory include 
        if /home does not exist, a critize line will be print out
