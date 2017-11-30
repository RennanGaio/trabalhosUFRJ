
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

#define MAX_NUMBER_OF_COMMANDS 100
#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

void run_bg_command (const char** commands, int size){
	printf("lul");
}


void run_command(const char** commands, int size){
  //função cd
	if (strcmp(commands[0], "cd") == 0){
		char * home = getenv("HOME");
		const char *arg = commands[1];
		if (size==1) chdir(home);
		else chdir(arg);
	} 

  //função quit 
  else if(strcmp(commands[0], "quit") == 0){
     exit(1);
  }
  // else if(strcmp(commands[0], "dir") == 0)

  //todas as outras funções, incluindo dir e pwd
  else {
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

int main(int argc, char *argv[]) {
  char *cmd;
  char line[MAX_LENGTH];
  char cwd[1024];
  // char str[80] = "pip install python &";

  // cmd=strtok(str, DELIMS);
  // while( cmd != NULL ) {
  //     printf( " %s\n", cmd );
  //   
  //     cmd = strtok(NULL, DELIMS);
  // }
  while (1) {
  	getcwd(cwd, sizeof(cwd));
    printf("<3 [%s]- ", cwd);
    if (!fgets(line, MAX_LENGTH, stdin)) break;

    cmd=strtok(line, DELIMS);
    int count=0;
    const char *commands[MAX_NUMBER_OF_COMMANDS];

  	while( cmd != NULL ) {
     	commands[count] = cmd;
    
      	cmd = strtok(NULL, DELIMS);
      	count++;
 	}

 	// for (int i = 0; i < count; ++i)
 	// {
 	// 	printf("%s\n", commands[i]);
 	// }
 	if (count==0){
 		continue;
 	}
 	if (strcmp(commands[count-1],"&")==0){
 		run_bg_command(commands, count);
 	}
 	else{
 		run_command(commands, count);
 	}
  }



  //   // Parse and execute command
  //   if ((cmd = strtok(line, DELIMS))) {
  //     // Clear errors
  //     errno = 0;

  //     if (strcmp(cmd, "cd") == 0) {
  //       char *arg = strtok(0, DELIMS);

  //       if (!arg) fprintf(stderr, "cd missing argument.\n");
  //       else chdir(arg);

  //     } else if (strcmp(cmd, "exit") == 0) {
  //       break;

  //     } else system(line);

  //     if (errno) perror("Command failed");
  //   }
  // }

  return 0;
}
