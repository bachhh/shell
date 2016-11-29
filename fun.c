#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

#define INPUT_BUFFER_SIZE 2038 
char* read_input(){
  char * buffer = malloc(sizeof(char)*INPUT_BUFFER_SIZE);
  size_t size;
  if (getline(&buffer, &size, stdin) == -1){
    printf("Error getline input");
  }
  return buffer;
}


#define POSIX_SHELL_TOKEN_DELIM " \t\n"
#define COMMAND_BUFFER_SIZE 1024 
char** parse_input(char* string){
  char** buffer = malloc(sizeof(char)*COMMAND_BUFFER_SIZE);
  int bufferPosition = 0;
  int bufferSizeMultiplier = 1;
  // Tokenize the string according to POSIX shell standard
  char* tempString = strtok(string, POSIX_SHELL_TOKEN_DELIM);
  while (tempString != NULL) {
    // Dynamic memory alloc
    if (bufferPosition > COMMAND_BUFFER_SIZE*bufferSizeMultiplier) {
      bufferSizeMultiplier++;
      buffer = realloc(buffer, COMMAND_BUFFER_SIZE*bufferSizeMultiplier);
    }
    buffer[bufferPosition] = tempString;
    bufferPosition++;
    tempString = strtok(NULL, POSIX_SHELL_TOKEN_DELIM);
  }
  buffer[bufferPosition] = NULL;
  return buffer;
}


int launch_cmd(char** cmdAndArgs){
  pid_t children_pid;
  int status;
  children_pid = fork();
  if (children_pid == 0){
    execvp(cmdAndArgs[0], cmdAndArgs); 
  }
  else {
    wait(NULL);
  }
  return status;
}




#define PROMPT_SIZE 500
char* directoryPrompt;

int main(int argc, char **argv){
  char* inputLine;
  char** cmdAndArgs;
  int status;
  
  while(1){
      directoryPrompt = malloc(sizeof(char) * PROMPT_SIZE);
      getcwd(directoryPrompt, PROMPT_SIZE); 
      printf("%s>", directoryPrompt);
      inputLine = read_input();
      cmdAndArgs = parse_input(inputLine);
      if(strcmp(cmdAndArgs[0], "cd" ) == 0){
        launch_cd(cmdAndArgs);
      }
      else if (strcmp(cmdAndArgs[0], "exit") == 0){
        break;
      }
      else { 
        chdir(cmdAndArgs[1]);
      }
      free(directoryPrompt);
  }
  return 0;
}
