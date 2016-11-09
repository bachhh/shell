#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

char* directoryPrompt;

int main(int argc, char **argv){
  int status;
  char* inputLine;
  char** cmdAndArgs;


  set_prompt(directoryPrompt);
  while{
      printf("%s>", directoryPrompt);
      inputLine = read_input();
      cmd = parse_input(inputLine);
      status = launch_cmd(cmd);
  }

  free(directoryPrompt);
  return 0;
}

#define PROMPT_SIZE = 500
void set_prompt(char * buf){
  buf = malloc(sizeof(char) * PROMPT_SIZE);
  int promptSizeMultiplier = 1;
  // Dynamic memory alloc
  while(getcwd(directoryPrompt, PROMPT_SIZE) == NULL){
    i++;
    directoryPrompt = realloc(directoryPrompt, PROMPT_SIZE * promptSizeMultiplier);
  }
}

#define INPUT_BUFFER_SIZE 500
char* read_input(){
  char * buffer = malloc(sizeof(char)*INPUT_BUFFER_SIZE);
  size_t size;
  // Getline already does dynamic memory management
  if (getline(&buffer, &size, stdin) == -1){
    printf("Error getline input");
  }
  return buffer;
}


#define POSIX_SHELL_TOKEN_DELIM = " \t\n"
#define COMMAND_BUFFER_SIZE = 500
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
