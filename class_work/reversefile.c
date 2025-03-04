#include <stdio.h>


int main(){
FILE *fptr;

//Creates the file if it doesnt exist
fptr = fopen("myfile.txt", "w");


fclose(fptr);

return 0;
}