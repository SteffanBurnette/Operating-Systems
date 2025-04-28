// waiting time - how long a process sits in the queue
// Turnaround time- time from arrival to completion
// Burst time - The overall cpu time a process needs
// gnatt chart - A bar chart that shows how processes are scheduled over time
// preemptive scheduling - The os can interrupt a process at any time and swap for another process
// nonpreemptive scheduling - The cpu cannot interrupt the process once it starts
// fcfs - fifo queue (nonpreemeptive)


#include <stdio.h>

#define N 4 // Number of processes

// Define the structure for Process
typedef struct {
    char pid;     // Process ID (A, B, C, D, etc.)
    int at;       // Arrival Time
    int bt;       // Burst Time
    int ct;       // Completion Time
    int tat;      // Turnaround Time
    int wt;       // Waiting Time
} Proc;

// Function to calculate Turnaround Time and Completion Time
void findTurnAroundTime(Proc p[], int n) {
    // Completion Time for the first process is its Arrival Time + Burst Time
    p[0].ct = p[0].at + p[0].bt;

    // Calculate Completion Time for other processes based on the previous completion time
    for (int i = 1; i < n; i++) {
        p[i].ct = p[i - 1].ct + p[i].bt;
    }

    // Calculate Turnaround Time (TAT) = Completion Time - Arrival Time
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
    }
}

// Function to calculate Waiting Time (WT)
void findWaitingTime(Proc p[], int n) {
    // Calculate Waiting Time (WT) = Turnaround Time - Burst Time
    for (int i = 0; i < n; i++) {
        p[i].wt = p[i].tat - p[i].bt;

        // Ensure that Waiting Time is not negative
        if (p[i].wt < 0) {
            p[i].wt = 0; // Set WT to 0 if it is negative (i.e., no waiting time)
        }
    }
}

// Function to calculate and display average times
void findAvgTime(Proc p[], int n) {
    int total_wt = 0, total_tat = 0;

    // Calculate Waiting Time and Turnaround Time
    findWaitingTime(p, n);
    findTurnAroundTime(p, n);

    // Display process details (Process ID, Arrival Time, Burst Time, Waiting Time, Turnaround Time, Completion Time)
    printf("\nProcesses   Arrival Time   Burst time   Waiting time   Turn around time   Completion time\n");
    for (int i = 0; i < n; i++) {
        total_wt += p[i].wt;
        total_tat += p[i].tat;
        printf("   %c         %d            %d           %d               %d               %d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].wt, p[i].tat, p[i].ct);
    }

    // Calculate and display average waiting time and turnaround time
    float avg_wt = (float)total_wt / n;
    float avg_tat = (float)total_tat / n;
    printf("Average waiting time = %f\n", avg_wt);
    printf("Average turn around time = %f\n", avg_tat);
}

int main() {
    // Define the process data (Process ID, Arrival Time, Burst Time)
    Proc p[N] = {
        {'A', 1, 4, 0, 0, 0},  // Process A arrives at t=1, needs 4 units
        {'B', 2, 2, 0, 0, 0},  // Process B arrives at t=2, needs 2 units
        {'C', 4, 2, 0, 0, 0},  // Process C arrives at t=4, needs 2 units
        {'D', 5, 3, 0, 0, 0}   // Process D arrives at t=5, needs 3 units
    };

    // Call the function to calculate and display average times
    findAvgTime(p, N);

    return 0;
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