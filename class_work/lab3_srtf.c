// Simulate Shortest Remaining Time First scheduling (preemptive).
// Includes only <stdio.h> and <stdlib.h>.
// Outputs per-time-unit CPU execution, waiting queue, completed list,
// then computes Turnaround Time (TAT), Waiting Time (WT), total TAT,
// average WT, and prints a Gantt chart....

#include <stdio.h>
#include <stdlib.h>

#define N 4  // Number of processes

// Process structure holds all necessary attributes
typedef struct {
    char pid;    // Process ID 
    int at;      // Arrival time: when the process enters the system
    int bt;      // Burst time: total CPU time required
    int rem;     // Remaining time: how much CPU time is left
    int ft;      // Finish time: recorded at end of last executed slot
} Proc;

int main() {
    // Initialize process array with arrival, burst, and IDs
    Proc p[N] = {
        {'A', 1, 4, 4, 0},  // PID A, arrives t=1, needs 4 units
        {'B', 2, 2, 2, 0},  // PID B, arrives t=2, needs 2 units
        {'C', 4, 2, 2, 0},  // PID C, arrives t=4, needs 2 units
        {'D', 5, 3, 3, 0}   // PID D, arrives t=5, needs 3 units
    };

    int completed = 0;   // Counter for number of processes finished
    int time = 1;        // Simulation time starts at 1

    // Buffers to record the scheduling history
    char gantt[100];            // Which PID ran at each time unit
    char waiting[100][N];       // Snapshot of waiting queue per time
    int wait_count[100] = {0};  // Counts of waiting processes per time
    char done[100][N];          // Snapshot of completed list per time
    int done_count[100] = {0};  // Counts of completed processes per time

    //Main simulation loop: run until all processes have completed
    while (completed < N) {  // Repeat each time unit
        //Choose the ready process with smallest remaining time
        int idx = -1;
        for (int i = 0; i < N; i++) {  // Loop through all processes
            // Only consider processes that have arrived and are not finished
            if (p[i].at <= time && p[i].rem > 0) {
                // If none selected yet, or this one has smaller remaining time
                if (idx < 0 || p[i].rem < p[idx].rem) {
                    idx = i;  // Select this process
                }
            }
        }

        //Record waiting queue: all ready & unfinished except the one running
        int wc = 0;
        for (int i = 0; i < N; i++) {  // Build list of waiting processes
            if (i != idx && p[i].at <= time && p[i].rem > 0) {
                waiting[time][wc++] = p[i].pid;  // Add PID to waiting array
            }
        }
        wait_count[time] = wc;  //Store waiting count for this time unit

        //Record completed list up to current time
        int dc = 0;
        for (int i = 0; i < N; i++) {  // Check all processes for completion
            if (p[i].rem == 0) {
                done[time][dc++] = p[i].pid;  // Add finished PID
            }
        }
        done_count[time] = dc;  //Store completed count for this time unit

        //Execute one time unit on the CPU
        if (idx >= 0) {
            gantt[time] = p[idx].pid;  // Log PID
            p[idx].rem--;              // Decrement remaining time
            if (p[idx].rem == 0) {
                // Record finish at end of this time slot
                p[idx].ft = time + 1;
                completed++;            // Increment finished count
            }
        } else {
            gantt[time] = '-';  // Idle if no process ready
        }

        time++;  // Advance simulation time
    }

    int T = time - 1;  // Total number of time units simulated

    //Print per-time-unit scheduling table
    printf("Time\tCPU\tWaiting\tCompleted\n");
    for (int t = 1; t <= T; t++) {  // Print each time slot
        printf("%d\t %c\t", t, gantt[t]);
        if (wait_count[t] > 0) {  // Waiting queue
            for (int i = 0; i < wait_count[t]; i++) {
                printf("%c", waiting[t][i]);
            }
        } else {
            printf("-");
        }
        printf("\t");
        if (done_count[t] > 0) {  // Completed list
            for (int i = 0; i < done_count[t]; i++) {
                printf("%c", done[t][i]);
            }
        } else {
            printf("-");
        }
        printf("\n");
    }

    //Compute and display performance metrics
    int total_tat = 0;
    int total_wt  = 0;
    printf("\nProcess\tAT\tBT\tFT\tTAT\tWT\n");
    for (int i = 0; i < N; i++) {  // Calculate metrics per process
        int tat = p[i].ft - p[i].at;  // Turnaround Time
        int wt  = tat - p[i].bt;      // Waiting Time
        total_tat += tat;
        total_wt  += wt;
        printf(" %c\t %d\t %d\t %d\t %d\t %d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ft, tat, wt);
    }
    printf("\nTotal Turnaround Time = %d\n", total_tat);
    printf("Average Waiting Time   = %.2f\n", total_wt / (float)N);

    //Draw the Gantt chart for visual timeline
    printf("\nGantt Chart:\n|");
    for (int t = 1; t <= T; t++) {  // Loop Gantt slots
        printf("%c|", gantt[t]);
    }
    printf("  (Time 1→%d)\n", T);

    return 0;  
}
