
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

#define MAX_NUMBER_OF_COMMANDS 100
#define MAX_WORD_LENGHT 50
#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

void run_command(char** commands, int size){
  //função cd
	if (strcmp(commands[0], "cd") == 0){
		char * home = getenv("HOME");
		const char *arg = commands[1];
		if (size==1) chdir(home);
		else if (chdir(arg)==-1){
			printf("%s : %s\n", strerror(errno), commands[1]);
		}
	} 

  //função quit 
  else if(strcmp(commands[0], "quit") == 0){
     exit(EXIT_SUCCESS);
  }
  // else if(strcmp(commands[0], "dir") == 0)

  //todas as outras funções, incluindo dir e pwd
  else {
  	 if (strcmp(commands[0], "environ") == 0){
  	 	commands[0] ="printenv";
  	 }
  	 else if (strcmp(commands[0], "help") == 0){
  	 	commands[0] ="bash";
  	 	commands[1]="-c";
  	 	commands[2]= "help";
  	 	size=3;
  	 }
  	 
     pid_t  pid; int    status;
     char *cmdExec = commands[0];
     char *argv[size+1];
     int command;

     //organiza os comandos para serem chamados pela função execvp
     for(command = 0; command < size; command++){
        argv[command] = commands[command];
     }
     argv[size] = NULL;
   
     //faz um fork
     if ((pid = fork()) < 0) exit(0);
     //executa os comandos usando execvp
     else if (pid == 0) {  
        if(execvp(cmdExec, argv) < 0) exit(1);
     }
     else {                                  /* for the parent:      */
            while (wait(&status) != pid)       /* wait for completion  */
                 ;
      }

  }   
     
}
void run_bg_command (char** commands, int size){
	pid_t  pid;
	if ((pid = fork()) == 0){
		run_command(commands, size);
	}
	else{
		printf("%d\n", pid);

	}
}

int main(int argc, char *argv[]) {
  char *cmd;
  char line[MAX_LENGTH];
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  char bash[100] = "BASH=";
  strcat(bash,cwd);
  strcat(bash,"/myshell");
  putenv(bash);

  while (1) {
  	getcwd(cwd, sizeof(cwd));
    printf("%s:%s <3- ", (getpwuid(getuid()))->pw_name ,cwd);
    if (!fgets(line, MAX_LENGTH, stdin)) break;

    cmd=strtok(line, DELIMS);
    int count=0;
    char *commands[MAX_NUMBER_OF_COMMANDS];

  	while( cmd != NULL ) {
     	commands[count] = cmd;
    
      	cmd = strtok(NULL, DELIMS);
      	count++;
 	}

 	if (count==0){
 		continue;
 	}
 	if (strcmp(commands[count-1],"&")==0){
 		commands[count-1]=NULL;
 		run_bg_command(commands, count);
 	}
 	else{
 		run_command(commands, count);
 	}
  }

  return 0;
}
