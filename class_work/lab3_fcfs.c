// waiting time - how long a process sits in the queue
// Turnaround time- time from arrival to completion
// Burst time - The overall cpu time a process needs
// gnatt chart - A bar chart that shows how processes are scheduled over time
// preemptive scheduling - The os can interrupt a process at any time and swap for another process
// nonpreemptive scheduling - The cpu cannot interrupt the process once it starts
// fcfs - fifo queue (nonpreemeptive)


/*
 * FCFS Scheduling Program in C
 */
 
 #include <stdio.h>
 int main()
 {
     int pid[15];
     int bt[15];
     int n;
     //Takes in the number of process from the user
     printf("Enter the number of processes: ");
     scanf("%d",&n);
     
     //Takes in the process id from the end users
     printf("Enter process id of all the processes: ");
     for(int i=0;i<n;i++)
     {
         scanf("%d",&pid[i]);
     }
    
     //Takes in the burst times of all the processes
     printf("Enter burst time of all the processes: ");
     for(int i=0;i<n;i++)
     {
         scanf("%d",&bt[i]);
     }
  
     int i, wt[n];
     wt[0]=0; //Initial wait time is 0 for the first process
  
     //for calculating waiting time of each process
     for(i=1; i<n; i++)
     {
         wt[i]= bt[i-1]+ wt[i-1];
     }
  
     printf("Process ID     Burst Time     Waiting Time     TurnAround Time\n");
     float twt=0.0; //total wait time
     float tat= 0.0;  //total turn around time
     for(i=0; i<n; i++)
     {
         printf("%d\t\t", pid[i]);
         printf("%d\t\t", bt[i]);
         printf("%d\t\t", wt[i]);
  
         //calculating and printing turnaround time of each process
         printf("%d\t\t", bt[i]+wt[i]);
         printf("\n");
  
         //for calculating total waiting time
         twt += wt[i];
  
         //for calculating total turnaround time
         tat += (wt[i]+bt[i]);
     }
     float att,awt;
  
     //for calculating average waiting time
     awt = twt/n; //divide by the number of processes
  
     //for calculating average turnaround time
     att = tat/n;
     printf("Avg. waiting time= %f\n",awt);
     printf("Avg. turnaround time= %f",att);
 }


/**
 * 
 * 
The four scheduling algorithms have different performance characteristics:
FCFS: FCFS is a simple algorithm that executes processes in the order they arrive. It has a low overhead but can lead to poor performance if there are processes with long burst times.
RR: RR is a time-sharing algorithm that allocates a fixed time quantum to each process. It provides good responsiveness and fairness but can lead to high overhead due to context switching.
SRTF: SRTF is a preemptive algorithm that executes the process with the shortest remaining time first. It provides good performance and responsiveness but can lead to high overhead due to context switching.
Priority Scheduling: Priority scheduling executes processes based on their priority. It provides good performance for high-priority processes but can lead to starvation for low-priority processes.
The choice of algorithm depends on the specific requirements of the system. For example:
Interactive systems: RR or SRTF may be suitable for interactive systems where responsiveness is important.
Batch systems: FCFS or Priority Scheduling may be suitable for batch systems where throughput is important.
Real-time systems: Priority Scheduling may be suitable for real-time systems where predictability and reliability are important.
In conclusion, each scheduling algorithm has its strengths and weaknesses, and the choice of algorithm depends on the specific requirements of the system.
 */