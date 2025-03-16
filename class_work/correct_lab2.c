#include <errno.h>   // Provides error codes
#include <fcntl.h>   // Defines open() system call options
#include <stdio.h>   // For printf() and perror()
#include <unistd.h>  // For close() and other system calls
#include <stdlib.h> // Needed for malloc(), calloc(), and free()
#include <string.h> //Used for working with strings
#include <sys/types.h>

#define BUFFER_SIZE 256 // Read file in chunks of 256 bytes
#define PATTERN "oldtext"
#define REPLACEMENT "newtext"
#define BUFFER_SIZE (1024 * 1024)  // 1MB
#define CHUNK_SIZE  4096  // 4KB chunks

void search_and_replace(const char *filename) {
    //Opens the specified file 
    int fd = open(filename, O_RDWR);
    if (fd < 0) {
        perror("Error opening file");
        exit(1);
    }
    //the buffer
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read; //the number of bytes to be read
    off_t offset = 0; // Track position in file
    
    //Will loop until you are un able to read 
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        //Used to compare the strings
        char *pos = strstr(buffer, PATTERN);
        if (pos != NULL) {
            // Calculate exact position in file
            off_t position = offset + (pos - buffer);

            // Move file pointer to position of found pattern
            lseek(fd, position, SEEK_SET);

            // Replace with new pattern (must be same length or shorter)
            write(fd, REPLACEMENT, strlen(REPLACEMENT));

            printf("Pattern replaced at position %ld\n", position);
            break; // Stop after first replacement
        }
        offset += bytes_read; // Update offset for next read
    }

    if (bytes_read < 0) {
        perror("Error reading file");
    }

    close(fd);
}


extern int errno;

//Will be used to rotate around a file
//(pos +step) % file_size will get the next postion 
void circular_seek(int fd, int step, int file_size) {
    int pos = 0;
    for (int i = 0; i < 5; i++) { // Move 5 times
        pos = (pos + step) % file_size; // Circular position
        lseek(fd, pos, SEEK_SET);
        write(fd, "*", 1);
    }
}


int main(){ 
   
    /**********
     * Open a File
     * When opening a file you need to ensure that:
     * The file exists
     * There is correct error handling
     * The file is closed once the program is finished its operations
     ***********************************/
    //Returns a positive int if the file opens successfully
    int my_file = open("example.txt", O_RDONLY | O_CREAT); 
    if(my_file == -1){ //returns -1 if the file fails to open
        printf("The Error Number %d\n", errno); //outputs the error number
        perror("The file failed to open"); //Outputs a human readable error message

    }
    //Closing the file
    if(close(my_file) < 0){ //If the value is negative then the file did not close
        perror("The file failed to close"); //Returns a human readable error message
        exit(1);
    }
    printf("The file closed");


    /***********************
     * Read A Files contents and outputs them in the terminal
     * When reading from a file you need to ensure:
     * That the file is opened properly with read permissions
     * That error handling is implemented
     * That a buffer is created to store the contents of the file
     * That you free the buffer once its task is done to prevent data leakage
     * And the file is closed
     ***********************/
    int my_file_rd = open("example.txt", O_RDONLY); 
    if(my_file_rd < 0){
        printf("There was an error when trying to read the file");
        perror("The file could not be read"); //human readable error msg
        exit(1); //Terminates the program
    }
    //Allocating mem for the contents in the file to be stored in the buffer
    char* c = (char*)calloc(100, sizeof(char)); // Allocate buffer
    //read takes the file name, buffer, and buffer size
    //Stores the file contents
    int read_contents = read(my_file_rd, c, 30); 

    c[read_contents] = '\0'; // Null-terminate the string
    printf("Read %d bytes: %s\n", read_contents, c); // Output the content

    free(c); // Free allocated memory
    close(my_file_rd); // Close the file

    /*****************************
     * Writing to a file
     ***************************/
    //Will hold the written content
    int write_content;
    //Opening my file with write permissions
    //int my_file_wr = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    //Will append the data to the file instead of erasing old data
    int my_file_wr = open("example.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);

    if(my_file_wr < 0){
        printf("There was an error writing to the file");
        perror("Writing to file error");
        exit(1); //Terminate the program
    }
    //Writing my new content to the file
    write_content = write(my_file_wr, "This is new appended content\n", strlen("This is new appended content\n"));
    //Closing the file after I am done writing content to it
    close(my_file_wr);
    //Currently this write will erase all existing content and will replace
    //it with the new content being written


    /************************* 
     *Part 2: Using lseek() to interact with a File via the file pointer
    **************************/
    int my_file_csi = open("cuny_csi.txt", O_RDWR | O_CREAT, 0644);
    if (my_file_csi < 0) {
        perror("Error opening file");
        exit(1);
    }

    // Write "Welcome to CUNY-CSI!" at the beginning
    write(my_file_csi, "Welcome to CUNY-CSI!", strlen("Welcome to CUNY-CSI!"));

    // Move file pointer 5 bytes ahead from the beginning
    lseek(my_file_csi, 5, SEEK_SET);
    // Overwrite the next characters with "CSC332-OS"
    write(my_file_csi, "CSC332-OS", strlen("CSC332-OS"));
    
    //Starts from the current postion of the file pointer
    lseek(my_file_csi, 5, SEEK_CUR);
    write(my_file_csi, "New", strlen("New"));

    //Positions the file pointer at the end of the file and goes two points 
    //before the end of the file to start writing the changes
    lseek(my_file_csi, -2, SEEK_END);
    write(my_file_csi,"End", strlen("End"));

    // Close the file
    close(my_file_csi);

    //Circular rotation
    //Opens the file
    int my_file_circular = open("circular.txt", O_RDWR | O_CREAT, 0644);
    if (my_file_circular < 0) {
        perror("Error opening file");
        exit(1);
    }
    
    //Writes to the specified file
    write(my_file_circular, "spheal", 6); // Initial content
    //Invokes the function to retrieve the next postion of the current pointer  
    circular_seek(my_file_circular, 3, 6); // Move in steps of 3 circularly
    //closes the file once completed
    close(my_file_circular);

    //Searching in 1kb file
    search_and_replace("file1kb.txt");
    
    /**
     * Part 3: dup() & dup2()
     */
      //The file that will be duplicated is a file that is open with
      //writing positions
      int my_file_og = open("example.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
      if(my_file_og < 0){
        perror("The file could not open for writing");
        exit(1);
      }
      write(my_file_og, "Original\n", strlen("Original\n"));
      //Am making a duplicate of the file
      int my_file_dup = dup(my_file_og);
      if(my_file_dup < 0){
        perror("The was an error when trying to make the duplicate");
        //Closes the original file since we cant complete the rest of the program
        close(my_file_og);
        exit(1);
      }
      //Am writing to the duplicate file
      write(my_file_dup, "The duplicate\n", strlen("The duplicate\n"));

      dup2(my_file_og, 1) ;
      printf("I will be printed in the file example.txt\n");  

      close(my_file_og);
      close(my_file_dup);

      /**
       * Part 3: ADvanced Question
       */
      
       int fd1 = open("test.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
       int fd2 = dup(fd1);
       write(fd1, "ABC", 3);
       write(fd2, "DEF", 3); // Continues writing after "ABC"
       close(fd1);
       close(fd2);

       //Opens the log file
       //Creates a dupe
       //Outputs the statement into the duplicate file
       //which is the error msg 
       int my_file_er = open("errors.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
       if (my_file_er < 0) {
           perror("Error opening log file");
           exit(1);
       }
       // Redirect stderr to errors.log
       dup2(my_file_er, 1);
       // This error message goes to errors.log instead of the terminal
       fprintf(stderr, "An error occurred: File not found!\n");
       close(my_file_er);


       //Data Pieplining
       //Appearently does not work on windows
       /*
       int pipefd[2];
       if (pipe(pipefd) == -1) {
           perror("Pipe failed");
           exit(1);
       }
   
       pid_t pid = fork();
       if (pid < 0) {
           perror("Fork failed");
           exit(1);
       }
   
       if (pid == 0) { // Child process
           close(pipefd[0]); // Close read end
           dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
           write(STDOUT_FILENO, "Hello from child!\n", 18);
           close(pipefd[1]);
       } else { // Parent process
           close(pipefd[1]); // Close write end
           char buffer[100];
           read(pipefd[0], buffer, 100);
           printf("Parent received: %s", buffer);
           close(pipefd[0]);
       }
*/
     /**
      * Part 4: Memory manipuation with memeset()
      */

      // Allocate a 100-byte buffer of type char
    char *buffer = (char *)malloc(100);
    if (buffer == NULL) { //Ensures that the memory allocated is for chars
        perror("Memory allocation failed");
        exit(1);
    }

    // Fill buffer with 'X'
    //buffer is the pointer to the memory block
    //X is the value
    //100 is the number of bytes to fill
    memset(buffer, 'X', 100);

    // Open file for writing
    int my_file_mem = open("memset_output.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (my_file_mem < 0) {
        perror("Error opening file");
        free(buffer);
        exit(1);
    }

    // Write buffer to file
    write(my_file_mem, buffer, 100);
    close(my_file_mem);

    // Verify by reading the file back
    my_file_mem = open("memset_output.txt", O_RDONLY);
    read(my_file_mem, buffer, 100);
    close(my_file_mem);

    // Print first 20 bytes for verification
    printf("First 20 bytes in file: %.20s\n", buffer);
    //frees the memomory to avoid leaks
    free(buffer);

    /**
     * Part 4: Advanced Questions
     * Implement a program that dynamically allocates a large buffer (1MB)
     */

       // Allocate 1MB buffer of type2 char
    char *my_buffer = (char *)malloc(BUFFER_SIZE);
    if (my_buffer == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    // Fill buffer with a test pattern (0xAA)
    memset(my_buffer, 0xAA, BUFFER_SIZE);

    // Open file for writing
    int fd = open("large_memset_output.bin", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("Error opening file");
        free(my_buffer);
        exit(1);
    }

    // Write the buffer in 4KB chunks
    //keeps track of how many bytes have been written so far.
    size_t written = 0;
    //This loop runs until all bytes in the buffer have been written.
    while (written < BUFFER_SIZE) {
        //This determines how many bytes to write in this iteration
        //If more than CHUNK_SIZE (4KB) remains, write a full 4KB.
        //If less than 4KB remains, write only the remaining bytes.
        size_t bytes_to_write = (BUFFER_SIZE - written) > CHUNK_SIZE ? CHUNK_SIZE : (BUFFER_SIZE - written);
        //Writes to the file by moving the pointer to the next unwritten part.
        write(fd, my_buffer + written, bytes_to_write);

        //Ensures that on the next loop iteration, we write the next chunk..
        written += bytes_to_write;
    }

    close(fd); //closes the file
    free(my_buffer); //frees the buffer to prevent leaks 
    printf("1MB buffer written successfully in chunks!\n");


 








    return 0;
}