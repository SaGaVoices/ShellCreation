# Creating a Shell in C 

The given repository contains the code of how to make your own shell in C and run it on linux terminal.

## The constructed shell supports following internal commands:
- clr
- pause
- help
- quit
- history
- cd
- pwd
- dir
- environ
- echo
- myshell <batchfile> (where <batchfile> is a textfile containing the internal commands to be ran on the shell)


## How to run it on your terminal?

- Download the libreadline-dev library if your system doesn't have. You can use the command: 'sudo apt-get install libreadline-dev' for that.
- Compile the file using the following command: 'gcc shell.c -lreadine'
- Run the a.out file (./a.out)
