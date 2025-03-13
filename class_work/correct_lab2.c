#include <errno.h>   // Provides error codes
#include <fcntl.h>   // Defines open() system call options
#include <stdio.h>   // For printf() and perror()
#include <unistd.h>  // For close() and other system calls


extern int errno;




int main(){ 
   
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



    return 0;
}