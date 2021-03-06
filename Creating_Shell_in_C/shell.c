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
#include <dirent.h>
  
#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 1000 // max number of commands to be supported 
  
// Clearing the shell using escape sequences 
#define clr() printf("\033[H\033[J") 


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
    	fputs(str, fptr);
    	fputs("\n", fptr);
    	fclose(fptr);
}
  
// Function to take input 
int takeInput(char* str) 
{ 
    char* buf; 
  
    buf = readline(">>> "); 
    if(buf == 0){
    printf("\n\nBye Bye!!\nHave a nice day!\n\n");
    exit(0);
    }
    if (strlen(buf) != 0) { 
        strcpy(str, buf);
        add_History(buf);  
        return 0; 
    } else{
        return 1; 
    } 
} 

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

//Printing a string without multiple spaces in between
void print_without_spaces(char* str)
{
    while(*str && *str == ' ')
        str++;
    while(*str)
    {
        if(*(str) != '\0' && (*(str) != ' ' || (*(str-1) != ' ' && *(str) == ' ')))
            putchar(*str);
        str++;
    }
    printf("\n");
}
  
// History Writing in Shell
void writeHistory()
{
        
        FILE* file = fopen("history.txt", "r"); 
    	char line[256];
    	
    	int i = 0;

   	while (fgets(line, sizeof(line), file)) {
        	printf("History Command %d: %s", i, line);
        	i++; 
    	}
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

//Function to get all the elements in a given directory
void getdirectory (char *path)
{
  DIR *dp;
  struct dirent *ep;     
  dp = opendir (path);

  if (dp != NULL)
  {
    while ((ep = readdir(dp)) != NULL) 
    if((strcmp(ep->d_name, ".") != 0) && (strcmp(ep->d_name, "..") != 0)){
        printf("%s\n", ep->d_name);}

    closedir (dp);
  }
  else
    perror ("Couldn't open the directory");
}

  
// Function to execute builtin commands 
int ownCmdHandler(char** parsed) 
{ 
    int NoOfOwnCmds = 8, i, switchOwnArg = 0; 
    char* ListOfOwnCmds[NoOfOwnCmds]; 
    char* username; 
    int tem_ind = 1;
  
    ListOfOwnCmds[0] = "quit"; 
    ListOfOwnCmds[1] = "cd"; 
    ListOfOwnCmds[2] = "help";  
    ListOfOwnCmds[3] = "clr";
    ListOfOwnCmds[4] = "history";
    ListOfOwnCmds[5] = "pause";
    ListOfOwnCmds[6] = "echo";
    ListOfOwnCmds[7] = "dir";
  
    for (i = 0; i < NoOfOwnCmds; i++) { 
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
            switchOwnArg = i + 1; 
            break; 
        } 
    } 
  
    switch (switchOwnArg) { 
    case 1: 
        printf("\nBye Bye!!\nHave a nice day!!\n\n"); 
        exit(0); 
    case 2: 
    	if(parsed[1] == NULL){
    		printf("No path given\n");
    	}
    	else{
        	if(chdir(parsed[1]) == -1){
        		perror("Dir doesn't exist!");
        	}
        } 
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
    case 7:
        while(parsed[tem_ind]!=NULL) {
             printf("%s ", parsed[tem_ind]);
             tem_ind++;
        }
        printf("\n");
        return 1;
    case 8:
	if (parsed[1] != NULL){
               getdirectory(parsed[1]);
        }
        else {
            printf("No directory specified\n");
        }
        return 0;
    default: 
        break; 
    } 
  
    return 0; 
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
  
void processString(char* str, char** parsed) 
{  
    parseSpace(str, parsed); 
    ownCmdHandler(parsed); 
    return;    
} 


int main(int argc, char *argv[], char * envp[]) 
{ 
    char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
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
    		printf("\n");
	}
	
	//Reading the first string of input command
	first = strsep(&string," ");
	
	//Checking if the first string is myshell and if yes, then executing the commands written in batchfile.txt
	if(strcmp(first , "myshell") == 0){
		second = strsep(&string, " ");
		
		FILE* file = fopen(second, "r"); /* should check the result */
    		char line[256];
    		int execFlag_ = 0; 

   		while (fgets(line, sizeof(line), file)) {
        		
        		int size = strlen(line); //Total size of string
			line[size-1] = '\0';
			char inputString[MAXCOM];
			strcpy(inputString, line);
        		printf("\n");
        		if(strcmp(line, "environ") == 0)
        		{
    				int i;
 				for (i = 0; envp[i] != NULL; i++)
    				{    
        				printf("\n%s", envp[i]);
    				}
    				printf("\n");
			}
			
			processString(inputString, 
        		parsedArgs); 
  
 	   	}

    		fclose(file);
    	
		
	}else {
		processString(inputString, 
        	parsedArgs);  
  
   	}
    } 
    return 0; 
} 
