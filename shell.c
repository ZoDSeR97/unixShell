// Minh Le
// CIS340
// A simple shell program that supports Linux shell commands in addition to the internal commands. This shell that can generate as much processes are needed to handle as user's input

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

char* getIn(FILE *stream, char *buffer, int initial){

	int count = 0;
	char c;

	while((c = getc(stream)) != '\n'){
		buffer[count] = c;
		count += 1;
		if(count >= initial){
			buffer = realloc(buffer,sizeof(char)*(initial + count)); //Need to return new memmory address
			initial += count;
		}
	}
	buffer[count] = '\0';
	return buffer;
}

int main(int argc, char *argv[]){

	int b = 1;
	int status;
	char *path = "";
	int sizePath = 512; //will increment when adding exceed the 512 characters including null-terminated
	path = (char *)malloc(sizeof(char)*sizePath);
	pid_t pid;
	
	int evenPipe[2];
	int oddPipe[2];
	int fd;
	
	while(b){
		int child = 1;
		char *input = malloc(sizeof(char) *10);
		
		//get input
		printf("Shell> ");
		input = getIn(stdin, input, 10);
		int len = strlen(input);
		//printf("len: %d\n", strlen(input));

		//dissect the user input
		char *cmd[len];
		cmd[0] = strtok(input, "|");
		//printf("cmd[%d]: %s\n", 0, cmd[0]);
		for(int i = 1; i < len ; i++){
			if((cmd[i] = strtok(NULL, "|")) != NULL){child++;}
			//printf("cmd[%d]: %s\n", i, cmd[i]);
		}
		char *tokens[len];
		tokens[0] = strtok(cmd[0], " ");
		if(tokens[0] == NULL){continue;}
		//printf("tokens[%d]: %s\n", 0, tokens[0]);
		for(int i = 1; i < len ; i++){
			tokens[i] = strtok(NULL, " ");
			//printf("tokens[%d]: %s\n", i, tokens[i]);
		}
		
		//Find internal command first (built-in) then Linux command
		if(!strcmp(tokens[0], "quit") || !strcmp(tokens[0], "exit")){
			b = 0;
		}else if(strcmp(tokens[0], "path") == 0){
			if(tokens[1] == NULL){
				if(strcmp(path, "") == 0){
					printf("%s\n","Path is empty!");
				}else{printf("%s\n", path);}
			}else if(strcmp(tokens[1], "+") == 0){
				if (tokens[2] != NULL){
					while((strlen(tokens[2]) +strlen(path) +1) > sizePath){
						path = realloc(path, sizeof(char) * (sizePath*2));
						sizePath = sizePath*2;
					}
					if(strcmp(path, "") == 0){
						memcpy(path, tokens[2], strlen(tokens[2])+1);
						strncat(path, ":", 2);
					}else{
						strncat(path, tokens[2], strlen(tokens[2])+1);
						strncat(path, ":", 2);
					}
				}else{printf("%s\n", "Need to input the path to add!");}
			}else if(strcmp(tokens[1], "-") == 0){
				if (tokens[2] != NULL){
					if(strcmp(path, "") == 0){
						printf("%s\n", "Path is already empty!");
					}else{
						char removStr[strlen(tokens[2]) +2];
						memcpy(removStr, tokens[2], strlen(tokens[2])+1);
						strncat(removStr, ":", 2);
						char *str = strstr(path, removStr);
						if(str){
							memmove(str, str+strlen(removStr), strlen(str+strlen(removStr))+1);
						}else{printf("%s\n", "ERROR: Path not found!");}
					}
				}else{printf("%s\n", "Need to input the path to remove!");}
			}else{
				printf("%s\n", "ERROR: Command does not support the argument!");
			}
		}else if(strcmp(tokens[0], "cd") == 0){
			if(tokens[1] == NULL){
				if(chdir("/home") != 0){
					printf("No home directory!");
				}
			}else if(chdir(tokens[1]) != 0){
				perror("ERROR");
			}
		}else{
			//fd = open("user input", O_WRONLY| O_CREAT| O_TRUNCT);
			for(int i = 0; i < child; i++){
				//setup pipe
				if(((i%2) == 0) && (i != (child-1))){
					if((pipe(oddPipe) < 0)){
						perror("Pipe");
						exit(2);
					}
				}else if(((i%2) != 0) && (i != (child-1))){
					if((pipe(evenPipe) < 0)){
						perror("Pipe");
						exit(2);
					}
				}	
				//Give birth to your slave!
				pid = fork();
				if(pid < 0){
					perror("child");
				}else if(pid == 0){
					if(i != 0){
						tokens[0] = strtok(cmd[i], " ");
						//printf("tokens[%d]: %s\n", 0, tokens[0]);
						if(tokens[0] == NULL){
							printf("%s\n", "Error: Missing input!");
							exit(0);
						}
						//printf("tokens[0]: %s\n", tokens[0]);
						for(int j = 1; j < 11 ; j++){
							tokens[j] = strtok(NULL, " ");
							//printf("tokens[%d]: %s\n", j, tokens[j]);
						}
					}
					
					if(((i%2) == 0) && (child > 1)){
						if(i != (child-1)){
							dup2(oddPipe[1], STDOUT_FILENO); // close(1) and dup(pipe[1])
							close(oddPipe[1]);
							close(oddPipe[0]);						
						} 
						if(i != 0){
							dup2(evenPipe[0], STDIN_FILENO);
							close(evenPipe[1]);
							close(evenPipe[0]);
						}
					}else if(((i%2) != 0) && (child > 1)){
						dup2(oddPipe[0], STDIN_FILENO);
						close(oddPipe[0]);
						close(oddPipe[1]);
						if(i != (child-1)){
							dup2(evenPipe[1], STDOUT_FILENO);
							close(evenPipe[0]);
							close(evenPipe[1]);
						}
					}
					
					for(int j = 0; tokens[j] != NULL; j++){
						if(!strcmp(tokens[j], ">")){
							if(tokens[j+1] == NULL){
								printf("%s\n", "Error: No output file!");
								exit(1);
							}else{
								if((fd = open(tokens[j+1], O_WRONLY| O_CREAT| O_TRUNC)) < 0){
									perror(tokens[j+1]);
									exit(1);
								}
								dup2(fd, STDOUT_FILENO);
								close(fd);
								tokens[j] = NULL;
								tokens[j+1] = NULL;
							}
						}else if(!strcmp(tokens[j], "<")){
							if(tokens[j+1] == NULL){
								printf("%s\n", "Error: No input file!");
								exit(1);
							}else{
								if((fd = open(tokens[j+1], O_RDONLY)) < 0){
									perror(tokens[j+1]);
									exit(1);
								}
								dup2(fd, STDIN_FILENO);
								close(fd);
								tokens[j] = NULL;
								tokens[j+1] = NULL;
							}
						}
					}
					
					//avoid hurting path string by making a copy of it
					char cpyPath[strlen(path)+1];
					memcpy(cpyPath, path, strlen(path)+1);
					char *entry = strtok(cpyPath, ":");	
					free(path);
					while(entry != NULL){
						//Combine command into a path
						char exeFile[strlen(entry) +strlen(tokens[0]) +2];
						memcpy(exeFile, entry, strlen(entry)+1);
						strncat(exeFile, "/", 2);
						strncat(exeFile, tokens[0], strlen(tokens[0])+1);
						
						execv(exeFile, tokens);
						entry = strtok(NULL, ":");
					}
					printf("%s\n", "ERROR: Command not found! (check path or command)");
					free(input);
					exit(0);
				}
				if((i%2) == 0){ //Odd child > 1
					if(i != 0){close(evenPipe[0]);}
					if(i != (child-1)){close(oddPipe[1]);}
				}else if((i%2) != 0){ //Even child > 1
					close(oddPipe[0]);
					if(i != (child-1)){close(evenPipe[1]);}
				}
				wait(&status);
				//printf("pid: %d\n", pid);
			}
		}
		free(input);
	}
	
	//Let it go
	free(path);
	
	return 0;

}

//MY FAMOUS DEBUG LINE: printf("GOT HERE!\n");
