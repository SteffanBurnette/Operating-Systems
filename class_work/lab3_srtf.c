// Simulate Shortest Remaining Time First scheduling (preemptive).
// Includes only <stdio.h> and <stdlib.h>.
// Outputs per-time-unit CPU execution, waiting queue, completed list,
// then computes Turnaround Time (TAT), Waiting Time (WT), total TAT,
// average WT, and prints a Gantt chart

#include<stdio.h>
int main()
{
    int bt[20],p[20],wt[20],tat[20],i,j,n,total=0,totalT=0,pos,temp;
    float avg_wt,avg_tat;
    printf("Enter number of process:");
    scanf("%d",&n);
 
    printf("\nEnter Burst Time:\n");
    for(i=0;i<n;i++)
    {
        printf("p%d:",i+1);
        scanf("%d",&bt[i]);
        p[i]=i+1;
    }
 
    //sorting of burst times
    //sort the processes based on their burst time
    for(i=0;i<n;i++) //loops through all burst times
    {
        pos=i;
        for(j=i+1;j<n;j++) //compares each process's burst time with the others
        {
            if(bt[j]<bt[pos])
                pos=j;
        }
 
        temp=bt[i];
        bt[i]=bt[pos];
        bt[pos]=temp;
 
        temp=p[i];
        p[i]=p[pos];
        p[pos]=temp;
    }
 
    wt[0]=0;
 
    //finding the waiting time of all the processes
    //used to reorder the processes based on their burst times in ascending order
    //outer loop - iterates through all processes 
    for(i=1;i<n;i++)
    {
        wt[i]=0;
        for(j=0;j<i;j++) //
             //individual WT by adding BT of all previous completed processes
            wt[i]+=bt[j];
 
        //total waiting time
        total+=wt[i];   
    }
 
    //average waiting time
    avg_wt=(float)total/n;  
 
    printf("\nProcess\t Burst Time \tWaiting Time\tTurnaround Time");
    for(i=0;i<n;i++)
    {
        //turnaround time of individual processes
        tat[i]=bt[i]+wt[i]; 
 
        //total turnaround time
        totalT+=tat[i];      
        printf("\np%d\t\t %d\t\t %d\t\t\t%d",p[i],bt[i],wt[i],tat[i]);
    }
 
    //average turnaround time
    avg_tat=(float)totalT/n;     
    printf("\n\nAverage Waiting Time=%f",avg_wt);
    printf("\nAverage Turnaround Time=%f",avg_tat);
}
