#include <stdio.h>


int main(){
FILE *fptr;

//Creates the file if it doesnt exist
//fptr = fopen("myfile.txt", "w");
//fprintf(fptr, "Testing 123");

// Open a file in append mode , will create a file if it doesnt exist
fptr = fopen("filename.txt", "a");

// Append some text to the file
fprintf(fptr, "\nHi everybody!");


fclose(fptr);

return 0;
}