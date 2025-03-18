#include <stdio.h>      // Include standard I/O library (not used but commonly included)
#include <fcntl.h>      // Include file control options for open()
#include <unistd.h>     // Include system call functions like read(), write(), lseek(), and close()
#include <sys/stat.h>   // Include stat() for getting file information
#include <stdlib.h>     // Include standard library for exit()

#define BUF_SIZE 12  // Define the buffer size for reading and writing

// Function to reverse a block of data using pointer arithmetic 
//Takes in the buffer being used and the size of the file(bytes)
void reverse_buffer(char *buffer, ssize_t len) { //ssize_t signed long int, can return -1, holds bits
    char *start = buffer;         // Pointer to the start of the buffer
    char *end = buffer + len - 1; // Pointer to the end of the buffer

    // Swap characters from start to end to reverse the buffer content
    while (start < end) {
        char temp = *start;  // Store the character at the start pointer
        *start = *end;       // Swap start with end
        *end = temp;         // Assign stored character to the end pointer
        start++;             // Move the start pointer forward
        end--;               // Move the end pointer backward
    }
}

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments are provided
    if (argc != 3) {
        write(STDERR_FILENO, "Usage: ./reverse_file source_file destination_file\n", 52);
        return 1;
    }

    // Open the source file in read-only mode
    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {  // Check if file opening failed
        write(STDERR_FILENO, "Error opening source file\n", 26);
        return 1;
    }

    // Get file size using stat()
    //struct stat is a system-defined structure that holds file metadata
    //has to be a struct
    struct stat file_stat;
    if (fstat(src_fd, &file_stat) == -1) {  // Check if stat() call failed
        write(STDERR_FILENO, "Error getting file size\n", 24);
        close(src_fd);  // Close source file before exiting
        return 1;
    }
    //.st_size gives the total number of bytes in the fil
    off_t file_size = file_stat.st_size;  // Store the file size

    // Open or create the destination file with write-only permission
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (dest_fd == -1) {  // Check if file opening/creation failed
        write(STDERR_FILENO, "Error opening destination file\n", 31);
        close(src_fd);  // Close source file before exiting
        return 1;
    }

    char buffer[BUF_SIZE];    // Buffer to store data from the file
    off_t offset = file_size; // Offset to track the current position in the file
    ssize_t bytes_read, bytes_to_read; // Variables to hold read/write sizes

    // Read the file in reverse, block by block
    while (offset > 0) {
        // Determine how many bytes to read (either BUF_SIZE or remaining bytes)
        bytes_to_read = (offset >= BUF_SIZE) ? BUF_SIZE : offset;
        offset -= bytes_to_read;  // Move the offset backward

        // Move the file pointer to the correct position for reading
        if (lseek(src_fd, offset, SEEK_SET) == -1) {
            write(STDERR_FILENO, "Error seeking in source file\n", 29);
            close(src_fd);
            close(dest_fd);
            return 1;
        }

        // Read the block of data from the file
        bytes_read = read(src_fd, buffer, bytes_to_read);
        if (bytes_read == -1) {  // Check if reading failed
            write(STDERR_FILENO, "Error reading from source file\n", 31);
            close(src_fd);
            close(dest_fd);
            return 1;
        }

        // Reverse the block of data
        reverse_buffer(buffer, bytes_read);

        // Write the reversed block to the destination file
        if (write(dest_fd, buffer, bytes_read) == -1) {
            write(STDERR_FILENO, "Error writing to destination file\n", 33);
            close(src_fd);
            close(dest_fd);
            return 1;
        }
    }

    // Close file descriptors to free resources
    close(src_fd);
    close(dest_fd);

    return 0;  // Return success
}
