// Round Robin scheduling (quantum = 1)
// Prints per-time-unit CPU, waiting queue, completed list,
// then computes TAT, WT, total TAT, average WT, and draws a Gantt chart.
// is Preeemptive

#include<stdio.h>
 
int main()
{
    //Input no of processes
    int  n;
    printf("\nEnter Total Number of Processes:");
    scanf("%d", &n);
    //initialized to store the original burst times, which will be updated as processes are executed.
    int wait_time = 0, ta_time = 0, arr_time[n], burst_time[n], temp_burst_time[n];
    int x = n; //The program enters a loop that continues until all processes are completed (tracked by the variable x)
 
    //Input details of processes
    for(int i = 0; i < n; i++)
    {
        printf("\nEnter Details of Process %d \n", i + 1);
        printf("Arrival Time:  ");
        scanf("%d", &arr_time[i]);
        printf("Burst Time:   ");
        scanf("%d", &burst_time[i]);
        temp_burst_time[i] = burst_time[i];
    }
 
    //Input time slot
    int time_slot;
    printf("\nEnter Time Slot:");
    scanf("%d", &time_slot);
 
    //Total indicates total time
    //counter indicates which process is executed
    int total = 0,  counter = 0, i;
    printf("Process ID       Burst Time       Turnaround Time      Waiting Time\n");
    //The variable i represents the index of the currently executing process. After each execution,
    // the algorithm checks if the next process should be executed based on its arrival time 
    //(arr_time[i+1]) or if it should loop back to the first process.
    //total is the sum of the total time elapsed since the start of the scheduling
    //and will be used to calculate the turnaround time
    for(total=0, i = 0; x!=0; )  
    {  
        // define the conditions
        //if time slot is greater than left burst time, execute process and find burst time.
       // Else if burst time is greater than time slot, execute it up to time slot and again
       // push into the queue.
       //This condition checks whether the remaining burst time for the current process 
       //(temp_burst_time[i]) is less than or equal to the time slot (time_slot) and greater than zero.
        if(temp_burst_time[i] <= time_slot && temp_burst_time[i] > 0)    
        {  
            total = total + temp_burst_time[i]; //The total burst time 
            temp_burst_time[i] = 0;  
            counter=1;  
        }   
        //This condition executes if the process hasn't finished yet (i.e.,
        // the remaining burst time is greater than the time slot).  
        else if(temp_burst_time[i] > 0)  //checks if the process still has remaining burst time, meaning the process hasn't finished execution yet.
        {  
            temp_burst_time[i] = temp_burst_time[i] - time_slot;  
            total  += time_slot;    
        }  
        //
        if(temp_burst_time[i]==0 && counter==1)  
        {  
            x--; //decrement the process no.  
            printf("\nProcess No %d  \t\t %d\t\t\t\t %d\t\t\t %d", i+1, burst_time[i],
                   total-arr_time[i], total-arr_time[i]-burst_time[i]);  
            wait_time = wait_time+total-arr_time[i]-burst_time[i];  
            ta_time += total -arr_time[i];  
            counter =0;     
        } 
        //Move on to the next process 
        if(i==n-1)  
        {  
            i=0;  
        }  
        else if(arr_time[i+1]<=total)  
        {  
            i++;  
        }  
        else  
        {  
            i=0;  
        }  
    }  
    float average_wait_time = wait_time * 1.0 / n;
    float average_turnaround_time = ta_time * 1.0 / n;
    printf("\nAverage Waiting Time:%f", average_wait_time);
    printf("\nAvg Turnaround Time:%f", average_turnaround_time);
    return 0;
}