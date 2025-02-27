/*
 * This program creates one parent process and two child processes.
 * The first child executes "ls -F" to list directory contents and writes the output
 * to a pipe. The second child reads from that pipe and executes "nl" to number the lines.
 * The parent process waits for both children to finish. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];  // Array to hold the two ends of the pipe

    // Create a one-direction pipe for communication between child processes
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create the first child process
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid1 == 0) {
        // In first child process: execute "ls -F"

        // Close the read end of the pipe; not needed here
        close(pipefd[0]);

        // Redirect standard output to the pipe's write end
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        // Close the original write end after duplicating
        close(pipefd[1]);

        // Execute ls -F using execlp
        execlp("ls", "ls", "-F", (char *) NULL);
        // If execlp returns, an error occurred
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }

    // Create the second child process
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid2 == 0) {
        // In second child process: execute "nl"

        // Close the write end of the pipe; not needed here
        close(pipefd[1]);

        // Redirect standard input to the pipe's read end
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        // Close the original read end after duplicating
        close(pipefd[0]);

        // Execute nl using execlp
        execlp("nl", "nl", (char *) NULL);
        // If execlp returns, an error occurred
        perror("execlp nl");
        exit(EXIT_FAILURE);
    }

    // In parent process:
    // Close both ends of the pipe; the parent doesn't need them
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both child processes to finish
    wait(NULL);
    wait(NULL);

    return 0;
}
