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
        
Sample Runs:
        Shell> path
        Path is empty!
        Shell> path + /usr
        Shell> path + /usr/etc
        Shell> path + /usr/bin
        Shell> path
        /usr:/usr/etc:/usr/bin:
        Shell> path - /usr/bin
        Shell> path
        /usr:/usr/etc:
        Shell> /path + /usr/bin
        ERROR: No such file or directory
        Shell> path + /usr/bin
        Shell> path - /usr
        Shell> path
        /usr/etc:/usr/bin:
        Shell> path + /usr
        Shell> ls
        demoProgram  Makefile  README.txt  shell  shell.c  shell.o
        Shell> pwd
        /home/zodser/Documents/cis340/project_5
        Shell> cd
        Shell> pwd
        /home
        Shell> cd /Something
        ERROR: No such file or directory
        Shell> rip 
        ERROR: Command not found!
        Shell> quit
        Pipepline run:
            input.txt
                Yolo
                Hello
                Bye
                heehah
                HeeHah1
                HeeHah2
                HeeHah1
                HeeHah2
                Heehah3
                HeeHah2
                Heehah3
                HeeHah2
                Heehah3
                HeeHah2
                Heehah3
                HeeHah2
                Heehah3
                HeeHah2
                Heehah3
            Shell> path + /bin
            Shell> path
            /bin:
            Shell> ls -l -a
            total 84
            drwxr-xr-x 3 zodser zodser  4096 Dec  3 18:29 .
            drwxr-xr-x 9 zodser zodser  4096 Nov 24 09:07 ..
            drwxr-xr-x 2 zodser zodser  4096 Nov 23 22:34 demoProgram
            -rw-r--r-- 1 zodser zodser   142 Dec  3 18:29 input.txt
            -rw-r--r-- 1 zodser zodser   184 Nov  3 00:09 Makefile
            -rw-r--r-- 1 zodser zodser   160 Dec  1 21:50 output.txt
            -rw-r--r-- 1 zodser zodser  1354 Nov 22 00:54 README.txt
            -rwxr-xr-x 1 zodser zodser 26824 Nov 26 16:51 shell
            -rw-r--r-- 1 zodser zodser  6729 Nov 26 16:51 shell.c
            -rw-r--r-- 1 zodser zodser 17504 Nov 26 16:51 shell.o
            Shell> ls 
            demoProgram  Makefile     README.txt  shell.c
            input.txt    output.txt  shell         shell.o
            Shell> ls -l -a | grep shell
            -rwxr-xr-x 1 zodser zodser 26824 Nov 26 16:51 shell
            -rw-r--r-- 1 zodser zodser  6729 Nov 26 16:51 shell.c
            -rw-r--r-- 1 zodser zodser 17504 Nov 26 16:51 shell.o
            Shell> grep heehah < input.txt
            heehah
            Shell> grep HeeHah < input .txt
            input: No such file or directory
            Shell> grep HeeHah < input.txt
            HeeHah1
            HeeHah2
            HeeHah1
            HeeHah2
            HeeHah2
            HeeHah2
            HeeHah2
            HeeHah2
            HeeHah2
            Shell> grep HeeHah < input.txt | grep HeeHah2
            HeeHah2
            HeeHah2
            HeeHah2
            HeeHah2
            HeeHah2
            HeeHah2
            HeeHah2
            Shell> grep HeeHah < input.txt | grep HeeHah2 | wc
                  7       7      56
            Shell> grep HeeHah < input.txt | grep HeeHah2 | wc > output.txt
            Shell> exit
            output.txt
                      7       7      56
