#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(void) {
    const char *filename = "example.txt";
    int fd;
    ssize_t bytes;
    char buffer[BUFFER_SIZE];

    /* --- Opening a File --- */
    /* Open the file for read and write. If it doesn't exist, create it with 0644 permissions.
       O_TRUNC is used to clear the file if it already exists. */
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    /* --- Writing to a File --- */
    const char *text = "Hello, world!\n";
    bytes = write(fd, text, strlen(text));
    if (bytes == -1) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* --- Reading from a File --- */
    /* Reset the file offset to the beginning so we can read what we just wrote */
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking to beginning of file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    bytes = read(fd, buffer, BUFFER_SIZE - 1);
    if (bytes == -1) {
        perror("Error reading from file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes] = '\0';  // Null-terminate the string
    printf("File content:\n%s", buffer);

    /* --- Closing a File --- */
    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    /* --- Appending Data to a File --- */
    /* Reopen the file in read/write mode with the O_APPEND flag so that writes add data at the end */
    fd = open(filename, O_RDWR | O_APPEND);
    if (fd == -1) {
        perror("Error opening file for appending");
        exit(EXIT_FAILURE);
    }

    const char *appendText = "This text is appended.\n";
    bytes = write(fd, appendText, strlen(appendText));
    if (bytes == -1) {
        perror("Error appending to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Read the file content after appending */
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking to beginning of file after appending");
        close(fd);
        exit(EXIT_FAILURE);
    }

    bytes = read(fd, buffer, BUFFER_SIZE - 1);
    if (bytes == -1) {
        perror("Error reading file after appending");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes] = '\0';
    printf("File content after appending:\n%s", buffer);

    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    return 0;
}
