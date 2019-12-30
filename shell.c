#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void waitBackground();
void execute(int commandlength);
int split(char line[]);
char **command;
char input[50];

int main(int argc, char **argv)
{
    char *prompt = "type";
    command  = malloc(sizeof(char*) * 100);
    int commandlength;

    if (argc != 1 && argc != 3)
    {
        printf("some error 1\n");
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "-p") != 0)
        {
            printf("some error 2\n");
        }
        else
        {
            prompt = argv[2];
        }
    }

    while (1) 
    {
        printf("%s > ", prompt);
        fgets(input, 50, stdin);

        waitBackground();

        //Don't break on new line
        if (input[0] == '\n')
        {
            continue;
        }

        commandlength = split(input);
        //Execute
        execute(commandlength);
    }
    return 1;
}

//splits the line into seperate words.
int split(char line[]) 
{
    line[strlen(line) - 1] = 0;
    int i = 0;

    char *ptr = strtok(line, " ");

    while(ptr != NULL)
	{
		command[i] = ptr;
		ptr = strtok(NULL, " ");

        i++;
	}
    command[i]= (char*) NULL;
    return i;
}

void execute(int commandlength)
{
    //some built in functions.
    if(strcmp(command[0], "exit") == 0)
    {
        exit(0);
    }
    else if(strcmp(command[0], "pid") == 0)
    {
        pid_t pid = getpid();
        printf("%d\n", pid);
        return;
    }
    else if(strcmp(command[0], "ppid") == 0)
    {
        pid_t ppid = getppid();
        printf("%d\n", ppid);
        return;
    }
    else if(strcmp(command[0], "cd") == 0)
    {
        if (command[1] != NULL)
        {
            chdir(command[1]);
        }

        if(command[1] == NULL){
            chdir(getenv("HOME"));
		}
        return;
    }
    else if(strcmp(command[0], "pwd") == 0)
    {

        char pwd[100];
        getcwd(pwd, sizeof(pwd));
        printf("%s\n", pwd);

        return;
    }

    else 
    {
        // Fork before he execl so that the child gets killed not the parent
        int flag = 0;
        int child;

        // Checks to see if the process is in the background
        waitBackground();
        
        // Checks to see if & is used for background process
        if(strcmp(command[commandlength - 1], "&") == 0){
            command[commandlength - 1] = '\0';
            flag = 1;
        }
    
        child = fork(); 
        // kill child if error isn't recognized
        if(child == 0){
            printf(">>>[%d] %s\n" , getpid(), input);
            execvp(command[0], &command[0]);
            perror("Error");
            exit(1);
        }
        
        // Prints child process' exit status
        else{
            int status;

            usleep(1000);
            if(flag == 0){
                waitpid(child, &status, 0); 
                printf(">>>[%d] %s Exit %d\n", child, *command, status);
            }
            else{
                waitBackground();
            }
        }
    }
}

//background executions helper method.
void waitBackground(){
	int status;
	int child2 = waitpid(-1, &status, WNOHANG);
	while(child2 > 0){
		printf(">>>[%d] Exit %d\n", child2, status);
		child2 = waitpid(-1, &status, WNOHANG);
	}
	
}