#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
  printf("Program starting\n");
  char *input = NULL;
  size_t size = 0;

  while (1) {
    printf("Enter programs to run: ");
    getline(&input, &size, stdin);
    // Check for new line in input

    if (input[strlen(input) - 1] == '\n') {
      input[strlen(input) - 1] = '\0';
    }

    // Check for empty input

    if (input[0] == '\0') {
      free(input);
      return 0;
    }

    // Run Program
    pid_t pid = fork();
    if (pid == 0) {
      execl(input, input, NULL);
      exit(1);
    } else {
      waitpid(pid, NULL, 0);
    }
  }

  return 0;
}
