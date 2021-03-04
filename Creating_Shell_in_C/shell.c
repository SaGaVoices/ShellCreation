//Creating a Shell in C with some listed internal commands
//1. clr
//2. pause
//3. help
//4. quit
//5. history
//6. cd
//7. dir
//8. environ
//9. echo
//10. displays shell environment as shell=<pathname>=myshell where <pathname>=myshell is full path for the shell executable
//11. the shell must be able to take its command line input from a file whenever the command line is invoked with a command line argument: myshell <batchfile>

#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h> 
#include <limits.h>
  
#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 1000 // max number of commands to be supported 
  
// Clearing the shell using escape sequences 
#define clr() printf("\033[H\033[J") 

//char *cmdsHistory[100];
//int cmdHis = 0;



  
// Greeting shell during startup 
void init_shell() 
{ 
    clr(); 
    printf("\n\n\n\n******************"
        "************************"); 
    printf("\n\n\n\t****MY SHELL****");  
    printf("\n\n\n\n*******************"
        "***********************"); 
    char* username = getenv("USER"); 
    printf("\n\n\nUSER is: @%s", username); 
    printf("\n"); 
    sleep(1); 
    clr(); 
} 


// Function to add History to a history text file
void add_History(char* str)
{
	FILE * fptr;
	fptr = fopen("history.txt", "a+");
    	//cmdsHistory[cmdHis] = str;
    	fputs(str, fptr);
    	fputs("\n", fptr);
    	fclose(fptr);
    	//cmdHis++;
}
  
// Function to take input 
int takeInput(char* str) 
{ 
    char* buf; 
  
    buf = readline(">>> "); 
    if (strlen(buf) != 0) { 
        add_history(buf); 
        strcpy(str, buf);
        add_History(buf);  
        return 0; 
    } else{
        return 1; 
    } 
} 

/**int takeRecInput(char* str, char * line)
{
	char * buf;
	
	buf = line;
	if (strlen(buf) != 0) { 
        	add_history(buf); 
        	strcpy(str, buf);
        	add_History(buf); 
        	printf("%s", str); 
        	return 0; 
    	} else{
        	return 1; 
    	} 
	
}**/


//Function to pause
void pauseStop(){
while (getchar() != '\n') ;
}

 
 
// Function to print Current Directory. 
void printDir() 
{ 
    char cwd[PATH_MAX]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("\nshell=<%s>=myshell", cwd); 
} 
  
// Function where the system command is executed 
void execArgs(char** parsed) 
{ 
    // Forking a child 
    int i, j;
    i = 0;
    j = -1;
    pid_t pid = fork();  
  
    if (pid == j) { 
        printf("\nFailed forking child.."); 
        return; 
    } else if (pid == i) { 
        if (execvp(parsed[i], parsed) < i) { 
            printf("\nCould not execute command.."); 
        } 
        exit(0); 
    } else { 
        // waiting for child to terminate 
        wait(NULL);  
        return; 
    } 
} 
  
// Function where the piped system commands is executed 
void execArgsPiped(char** parsed, char** parsedpipe) 
{ 
    // 0 is read end, 1 is write end 
    int pipefd[2];  
    pid_t p1, p2; 
  
    if (pipe(pipefd) < 0) { 
        printf("\nPipe could not be initialized"); 
        return; 
    } 
    p1 = fork(); 
    if (p1 < 0) { 
        printf("\nCould not fork"); 
        return; 
    } 
  
    if (p1 == 0) { 
        // Child 1 executing.. 
        // It only needs to write at the write end 
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 
  
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command 1.."); 
            exit(0); 
        } 
    } else { 
        // Parent executing 
        p2 = fork(); 
  
        if (p2 < 0) { 
            printf("\nCould not fork"); 
            return; 
        } 
  
        // Child 2 executing.. 
        // It only needs to read at the read end 
        if (p2 == 0) { 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO); 
            close(pipefd[0]); 
            if (execvp(parsedpipe[0], parsedpipe) < 0) { 
                printf("\nCould not execute command 2.."); 
                exit(0); 
            } 
        } else { 
            // parent executing, waiting for two children 
            wait(NULL); 
            wait(NULL); 
        } 
    } 
} 


// History Writing in Shell
void writeHistory()
{
        
        FILE* file = fopen("history.txt", "r"); /* should check the result */
    	char line[256];
    	
    	int i = 0;

   	while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        	printf("History Command %d: %s", i, line);
        	i++; 
    	}
    	/* may check feof here to make a difference between eof and io failure -- network
       	timeout for instance */

    	fclose(file);
        
        
        return;
}
  
// Help command builtin 
void openHelp() 
{ 
    puts("\n***WELCOME TO MY SHELL HELP***"
        "\nList of Commands supported:"
        "\n>clr"
        "\n>pause"
        "\n>quit"
        "\n>history"
        "\n>cd"
        "\n>dir"
        "\n>environ"
        "\n>echo"
        "\n>myshell <batchfile> where <batchfile> is a txt file containing the required commands to be ran"); 
  
    return; 
} 
  
// Function to execute builtin commands 
int ownCmdHandler(char** parsed) 
{ 
    int NoOfOwnCmds = 6, i, switchOwnArg = 0; 
    char* ListOfOwnCmds[NoOfOwnCmds]; 
    char* username; 
  
    ListOfOwnCmds[0] = "quit"; 
    ListOfOwnCmds[1] = "cd"; 
    ListOfOwnCmds[2] = "help";  
    ListOfOwnCmds[3] = "clr";
    ListOfOwnCmds[4] = "history";
    ListOfOwnCmds[5] = "pause";
  
    for (i = 0; i < NoOfOwnCmds; i++) { 
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
            switchOwnArg = i + 1; 
            break; 
        } 
    } 
  
    switch (switchOwnArg) { 
    case 1: 
        printf("\nBye Bye!!\nHave a nice day!!\n"); 
        exit(0); 
    case 2: 
        chdir(parsed[1]); 
        return 1; 
    case 3: 
        openHelp(); 
        return 1; 
    case 4:
    	clr();
    	return 1;
    case 5:
        writeHistory();
        return 1;
    case 6:
    	pauseStop();
    	return 1;
    default: 
        break; 
    } 
  
    return 0; 
} 
  
// function for finding pipe 
int parsePipe(char* str, char** strpiped) 
{ 
    int i; 
    for (i = 0; i < 2; i++) { 
        strpiped[i] = strsep(&str, "|"); 
        if (strpiped[i] == NULL) 
            break; 
    } 
  
    if (strpiped[1] == NULL) 
        return 0; // returns zero if no pipe is found. 
    else { 
        return 1; 
    } 
} 
  
// function for parsing command words 
void parseSpace(char* str, char** parsed) 
{ 
    int i; 
  
    for (i = 0; i < MAXLIST; i++) { 
        parsed[i] = strsep(&str, " "); 
  
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    } 
} 
  
int processString(char* str, char** parsed, char** parsedpipe) 
{ 
  
    char* strpiped[2]; 
    int piped = 0; 
    int i, j, k;
    i = 0;
    j = 1;
  
    piped = parsePipe(str, strpiped); 
  
    if (piped) { 
        parseSpace(strpiped[i], parsed); 
        parseSpace(strpiped[j], parsedpipe); 
  
    } else { 
  
        parseSpace(str, parsed); 
    } 
  
    if (ownCmdHandler(parsed)) 
        return 0; 
    else
        return 1 + piped; 
} 
  
int main(int argc, char *argv[], char * envp[]) 
{ 
    char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
    char* parsedArgsPiped[MAXLIST]; 
    int execFlag = 0; 
    char * string;
    char * first;
    char * second;
    
    //Initialize the shell 
    init_shell(); 
  
    while (1) { 
        // print shell line 
        printDir(); 
        
        
        // take input 
        if (takeInput(inputString)) 
            continue; 
            
        // process 
        string = strdup(inputString);
        
        //Checking if the input command is environ
        if(strcmp(inputString, "environ") == 0)
        {
    		int i;
 		for (i = 0; envp[i] != NULL; i++)
    		{    
        		printf("\n%s", envp[i]);
    		}
	}
	
	//Reading the first string of input command
	first = strsep(&string," ");
	
	
	//Checking if the first string is myshell and if yes, then executing the commands written in batchfile.txt
	if(strcmp(first , "myshell") == 0){
		second = strsep(&string, " ");
		
		//printf("%s", second);
		
		FILE* file = fopen(second, "r"); /* should check the result */
    		char line[256];
    	
    		char inputString_[MAXCOM], *parsedArgs_[MAXLIST]; 
    		char* parsedArgsPiped_[MAXLIST];
    		int execFlag_ = 0; 

   		while (fgets(line, sizeof(line), file)) {
        		// process 
        		//printf("History Command: %s", line);
        		//line = line.resize(line.size() - 1);
        		int size = strlen(line); //Total size of string
			line[size-1] = '\0';
        		//if(takeRecInput(inputString_, line))
        		//	break;
        		printf("\n");
        		if(strcmp(line, "environ") == 0)
        		{
    				int i;
 				for (i = 0; envp[i] != NULL; i++)
    				{    
        				printf("\n%s", envp[i]);
    				}
			}
			
			execFlag = processString(line, 
        		parsedArgs, parsedArgsPiped); 
        		// execflag returns zero if there is no command 
        		// or it is a builtin command, 
        		// 1 if it is a simple command 
        		// 2 if it is including a pipe. 
  
        		// execute 
        		if (execFlag == 1) 
            			execArgs(parsedArgs); 
  
 		       if (execFlag == 2) 
 		           execArgsPiped(parsedArgs, parsedArgsPiped); 
 	   	}

    		fclose(file);
    	
		
	}else {
	
	
        
        execFlag = processString(inputString, 
        parsedArgs, parsedArgsPiped); 
        // execflag returns zero if there is no command 
        // or it is a builtin command, 
        // 1 if it is a simple command 
        // 2 if it is including a pipe. 
  
        // execute 
        if (execFlag == 1) 
            execArgs(parsedArgs); 
  
        if (execFlag == 2) 
            execArgsPiped(parsedArgs, parsedArgsPiped); }
    } 
    return 0; 
} 
