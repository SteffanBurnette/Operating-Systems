#include <stdio.h>
#include <stdlib.h>

//Must be placed at the top of the main function so that it knows that it exists
void greeting(char name[]) {
    printf("Grettings %s", name);
}

//Returns a double data type
double mySq(int num){
    return num * num;
}

int main(){

     char name[] = "Alex";
     char greetName[] = "Tuff";
     
     printf("Your name is %s",name);
     printf("\nEnter your age: \n");

     /*
     int my_age;
     scanf("%d", &my_age);
     printf("Your age is: %d", my_age);
     */

     /*
     char myNewname[30];
     printf("Please enter the new name to be read by fgets: \n");
     fgets(myNewname, 30, stdin);
     printf("The new name entered by fgets is: %s", myNewname);
    */

    int myarr[] = {2, 3, 4,5};
    printf("This is myarr[0] output: %d \n", myarr[0]); //Outputs the first element
    printf("This is myarr output: %d \n", myarr); //Outputs the memory address
    greeting(greetName);


    printf("This is the mySq function output: %f \n", mySq(2));


    if(1 >= 0 && 2 >= 1){
        printf("Condition 1 \n");
    }
    else{
        printf("Failed \n");
    }



    char letter = 'A';
    switch(letter){
        case 'A':
        printf("The output is A \n");
        break;
        case 'B':
        printf("The output is B \n");
        break;
        case 'C':
        printf("The output is C \n");

    }


    int age = 30;
    printf("This is the memory address of age: %p", &age);

    int * pAge = &age;



    return 0;
}


