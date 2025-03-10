#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024
#include <time.h>
#include <sys/wait.h>

/* -------------------------------------------
   Part 2: File Positioning with lseek 
   ------------------------------------------- */

// Part 2 Basic: Open a file, write initial text, reposition, and overwrite part of it.
void part2_basic() {
    printf("\n--- Part 2 Basic: File Positioning ---\n");
    const char *filename = "part2.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file (part2_basic)");
        exit(EXIT_FAILURE);
    }
    
    // Write initial text.
    char initText[] = "Welcome to CUNY-CSI!";
    if (write(fd, initText, strlen(initText)) == -1) {
        perror("Error writing initial text");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    // Move the file pointer 5 bytes from the beginning.
    if (lseek(fd, 5, SEEK_SET) == -1) {
        perror("Error using lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    // Overwrite the next characters with "CSC332-OS".
    char overwriteText[] = "CSC332-OS";
    if (write(fd, overwriteText, strlen(overwriteText)) == -1) {
        perror("Error overwriting text");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    close(fd);
    printf("Part2 Basic: Completed modifications in '%s'.\n", filename);
}

// Advanced 1: Search for a pattern ("old") in a 1KB file and replace it with ("new").
void part2_search_replace() {
    printf("\n--- Part 2 Advanced 1: Search and Replace ---\n");
    #define FILE_SIZE 1024
    #define PATTERN "old"
    #define REPLACE "new"
    
    const char *filename = "search_replace.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file (search_replace)");
        exit(EXIT_FAILURE);
    }
    
    // Write sample content that includes the pattern.
    char sampleText[] = "This is the old content in a 1KB file. The word old will be replaced.";
    if (write(fd, sampleText, strlen(sampleText)) == -1) {
        perror("Error writing sample text");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    // Read up to 1KB from the file.
    char buffer[FILE_SIZE];
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking to beginning (search_replace)");
        close(fd);
        exit(EXIT_FAILURE);
    }
    ssize_t bytes = read(fd, buffer, FILE_SIZE);
    if (bytes == -1) {
        perror("Error reading file (search_replace)");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    // Look for the pattern.
    char *pos = strstr(buffer, PATTERN);
    if (pos != NULL) {
        off_t offset = pos - buffer;
        if (lseek(fd, offset, SEEK_SET) == -1) {
            perror("Error seeking in file (search_replace)");
            close(fd);
            exit(EXIT_FAILURE);
        }
        if (write(fd, REPLACE, strlen(REPLACE)) == -1) {
            perror("Error writing replacement text");
            close(fd);
            exit(EXIT_FAILURE);
        }
        printf("Search/Replace: Pattern replaced successfully in '%s'.\n", filename);
    } else {
        printf("Search/Replace: Pattern not found in '%s'.\n", filename);
    }
    close(fd);
}

// Advanced 2: Demonstrate SEEK_SET, SEEK_CUR, and SEEK_END.
void part2_seek_demo() {
    printf("\n--- Part 2 Advanced 2: lseek Demonstration ---\n");
    const char *filename = "demo.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening demo file");
        exit(EXIT_FAILURE);
    }
    
    // Write sample data.
    char data[] = "0123456789";
    if (write(fd, data, strlen(data)) == -1) {
        perror("Error writing demo data");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    off_t pos;
    pos = lseek(fd, 2, SEEK_SET);
    printf("SEEK_SET: File pointer is at offset %ld\n", (long)pos);
    
    pos = lseek(fd, 3, SEEK_CUR);
    printf("SEEK_CUR: File pointer is at offset %ld\n", (long)pos);
    
    pos = lseek(fd, -2, SEEK_END);
    printf("SEEK_END: File pointer is at offset %ld\n", (long)pos);
    
    close(fd);
}

// Advanced 3: Circular file pointer movement.
void part2_circular() {
    printf("\n--- Part 2 Advanced 3: Circular File Pointer Movement ---\n");
    const char *filename = "circular.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file for circular movement");
        exit(EXIT_FAILURE);
    }
    
    // Write known content to file.
    char content[] = "ABCDEFGHIJ";
    if (write(fd, content, strlen(content)) == -1) {
        perror("Error writing content (circular)");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1) {
        perror("Error determining file size (circular)");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    // For demonstration, move pointer 3 bytes ahead (wrap around using modulo).
    off_t current = 0; // starting at beginning
    off_t newPos = (current + 3) % fileSize;
    if (lseek(fd, newPos, SEEK_SET) == -1) {
        perror("Error in circular lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Circular: File pointer moved to offset %ld in '%s'.\n", (long)newPos, filename);
    
    close(fd);
}

/* -------------------------------------------
   Part 3: Duplicating File Descriptors (dup, dup2)
   ------------------------------------------- */

// Part 3 Basic: Use dup() to duplicate a file descriptor and write using both.
void part3_dup() {
    printf("\n--- Part 3 Basic: dup() Demonstration ---\n");
    const char *filename = "dup_test.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file (dup)");
        exit(EXIT_FAILURE);
    }
    
    char text1[] = "Original text.\n";
    if (write(fd, text1, strlen(text1)) == -1) {
        perror("Error writing original text");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    int dup_fd = dup(fd);
    if (dup_fd == -1) {
        perror("Error duplicating file descriptor");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    char text2[] = "Text written using duplicated descriptor.\n";
    if (write(dup_fd, text2, strlen(text2)) == -1) {
        perror("Error writing with duplicated descriptor");
        close(fd);
        close(dup_fd);
        exit(EXIT_FAILURE);
    }
    
    close(fd);
    close(dup_fd);
    printf("Part3 dup(): Completed writing to '%s'.\n", filename);
}

// Advanced: Redirect stderr to a file using dup2().
void part3_redirect_stderr() {
    printf("\n--- Part 3 Advanced: Redirect stderr using dup2() ---\n");
    const char *filename = "stderr_redirect.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file for stderr redirection");
        exit(EXIT_FAILURE);
    }
    
    if (dup2(fd, STDERR_FILENO) == -1) {
        perror("Error redirecting stderr");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    // Write an error message; it will go to the file.
    fprintf(stderr, "This error message is redirected to '%s'.\n", filename);
    
    close(fd);
    printf("Part3 dup2(): Redirected stderr to '%s'.\n", filename);
}

// Advanced: Pipe data between two processes using dup2().
void part3_pipe_dup2() {
    printf("\n--- Part 3 Advanced: Pipe Data Using dup2() ---\n");
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error in fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) { // Child process.
        close(pipefd[1]); // Close write end.
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("Child dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
        // Execute a command that reads from standard input.
        execlp("wc", "wc", "-l", NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else { // Parent process.
        close(pipefd[0]); // Close read end.
        char msg[] = "Line1\nLine2\nLine3\n";
        if (write(pipefd[1], msg, strlen(msg)) == -1) {
            perror("Error writing to pipe");
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        wait(NULL);
        printf("Part3 pipe/dup2(): Data piped to child process (wc -l).\n");
    }
}

/* -------------------------------------------
   Part 4: Memory Operations with memset
   ------------------------------------------- */

// Part 4 Basic: Allocate 100-byte buffer, fill with 'X', write to file.
void part4_basic() {
    printf("\n--- Part 4 Basic: memset() on a 100-byte Buffer ---\n");
    const char *filename = "memset_test.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file (memset basic)");
        exit(EXIT_FAILURE);
    }
    
    char buffer[100];
    memset(buffer, 'X', sizeof(buffer));
    
    if (write(fd, buffer, sizeof(buffer)) == -1) {
        perror("Error writing buffer");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    close(fd);
    printf("Part4 Basic: Wrote 100 bytes of 'X' to '%s'.\n", filename);
}

// Advanced: Allocate a 1MB buffer, fill with memset() and manual loop, and write to file.
void part4_advanced() {
    printf("\n--- Part 4 Advanced: 1MB Buffer with memset() and Manual Loop ---\n");
    #define LARGE_BUFFER_SIZE (1024 * 1024)
    const char *filename = "large_buffer.txt";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file (large buffer)");
        exit(EXIT_FAILURE);
    }
    
    char *buffer = malloc(LARGE_BUFFER_SIZE);
    if (!buffer) {
        perror("Error allocating memory");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    clock_t start = clock();
    memset(buffer, 'A', LARGE_BUFFER_SIZE);
    clock_t end = clock();
    double memset_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("memset() fill time: %f seconds\n", memset_time);
    
    if (write(fd, buffer, LARGE_BUFFER_SIZE) == -1) {
        perror("Error writing large buffer");
        free(buffer);
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    start = clock();
    for (size_t i = 0; i < LARGE_BUFFER_SIZE; i++) {
        buffer[i] = 'B';
    }
    end = clock();
    double loop_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Manual loop fill time: %f seconds\n", loop_time);
    
    free(buffer);
    close(fd);
    printf("Part4 Advanced: Wrote 1MB buffer to '%s'.\n", filename);
}

int main(void) {
    const char *filename = "example.txt";
    int fd;
    ssize_t bytes;
    char buffer[BUFFER_SIZE];

    // Open the File 
    /* Open the file for read and write. If it doesn't exist, create it.
       O_TRUNC is used to clear the file if it already exists. */
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("There was an error when trying to open the file");
        exit(EXIT_FAILURE);
    }

    // Write to a File  
    const char *text = "Hello, world!\n";
    bytes = write(fd, text, strlen(text));
    if (bytes == -1) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Readfrom a File  
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

    //Close a File 
    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    //Append Data to a File
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

    // Read the file content after appending
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


    /* Part 2: File Positioning with lseek */
    part2_basic();
    part2_search_replace();
    part2_seek_demo();
    part2_circular();

    /* Part 3: Duplicating File Descriptors */
    part3_dup();
    part3_redirect_stderr();
    part3_pipe_dup2();

    /* Part 4: Memory Operations with memset */
    part4_basic();
    part4_advanced();

    return 0;
}
