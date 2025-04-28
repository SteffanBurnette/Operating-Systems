// priority_np_commented.c
// Non-Preemptive Priority scheduling (low number = high priority).
// Outputs per-time-unit CPU execution, waiting queue, completed list,
// then computes Turnaround Time (TAT), Waiting Time (WT), total TAT,
// average WT, and prints a Gantt chart.

#include <stdio.h>
#include <stdlib.h>

#define N 4  // Number of processes

// Process structure definition
typedef struct {
    char pid;    // Process ID (e.g., 'A', 'B', ...)
    int at;      // Arrival Time: when process enters the system
    int bt;      // Burst Time: total CPU time required
    int pr;      // Priority: lower value means higher priority
    int rem;     // Remaining Time: how much CPU time is left
    int ft;      // Finish Time: when process completes execution
} Proc;

int main() {
    // 1) Initialize the array of processes with their attributes
    Proc p[N] = {
        {'A', 1, 4, 1, 4, 0},  // Process A arrives at t=1, needs 4 units, priority 1
        {'B', 2, 2, 2, 2, 0},  // Process B arrives at t=2, needs 2 units, priority 2
        {'C', 4, 2, 4, 2, 0},  // Process C arrives at t=4, needs 2 units, priority 4
        {'D', 5, 3, 3, 3, 0}   // Process D arrives at t=5, needs 3 units, priority 3
    };

    int completed = 0;      // Count of completed processes
    int time = 1;           // Current time unit (starts at 1)
    int curr = -1;          // Index of currently running process (-1 means CPU idle)

    // Buffers to record outputs for each time unit
    char gantt[100];           // Which PID ran at each time index
    char waiting[100][N];      // Waiting queue snapshot at each time
    int wait_count[100] = {0}; // Number of waiting processes per time
    char done[100][N];         // Completed process list snapshot per time
    int done_count[100] = {0}; // Number of completed processes per time

    // Run until all N processes complete
    while (completed < N) {
        // If CPU is free, select next process by highest priority
        if (curr < 0) {
            int best = -1;
            //Checks all processes to find the ready one with lowest pr value
            for (int i = 0; i < N; i++) {
                // Check if process has arrived and is not yet finished
                if (p[i].at <= time && p[i].rem > 0) {
                    // If none chosen yet, or this one has higher priority, pick it
                    if (best < 0 || p[i].pr < p[best].pr) {
                        best = i;
                    }
                }
            }
            curr = best;  // Assign chosen process index (or remain -1 if none ready)
        }

        //Record waiting queue at current time
        int wc = 0;
        for (int i = 0; i < N; i++) {
            // Any ready, unfinished process that is not the current one
            if (i != curr && p[i].at <= time && p[i].rem > 0) {
                waiting[time][wc++] = p[i].pid;
            }
        }
        wait_count[time] = wc;

        //Record completed processes up to current time
        int dc = 0;
        for (int i = 0; i < N; i++) {
            if (p[i].rem == 0) {
                done[time][dc++] = p[i].pid;
            }
        }
        done_count[time] = dc;

        //Execute one time unit on the CPU
        if (curr >= 0) {
            // Mark which process ran at this time
            gantt[time] = p[curr].pid;
            // Decrement its remaining time
            p[curr].rem--;
            // If it just finished, record finish time and free CPU
            if (p[curr].rem == 0) {
                p[curr].ft = time;
                completed++;
                curr = -1;
            }
        } else {
            // No process ready: CPU is idle
            gantt[time] = '-';
        }

        // Move to next time unit
        time++;
    }

    int T = time - 1;  // Total time units used in simulation

    // 3) Print the per-time-unit table header
    printf("Time\tCPU\tWaiting\tCompleted\n");
    // Loop through each time unit and print recorded data
    for (int t = 1; t <= T; t++) {
        // Time index and which PID ran (or idle)
        printf("%d\t %c\t", t, gantt[t]);
        // Print waiting queue or '-' if none
        if (wait_count[t] > 0) {
            for (int i = 0; i < wait_count[t]; i++) {
                printf("%c", waiting[t][i]);
            }
        } else {
            printf("-");
        }
        printf("\t");
        // Print completed list or '-' if none
        if (done_count[t] > 0) {
            for (int i = 0; i < done_count[t]; i++) {
                printf("%c", done[t][i]);
            }
        } else {
            printf("-");
        }
        printf("\n");
    }

    //Compute and display metrics for each process
    int total_tat = 0, total_wt = 0;
    printf("\nProcess\tAT\tBT\tPR\tFT\tTAT\tWT\n");
    for (int i = 0; i < N; i++) {
        int tat = p[i].ft - p[i].at;          // Turnaround Time
        int wt  = tat - p[i].bt;              // Waiting Time
        total_tat += tat;
        total_wt  += wt;
        printf(" %c\t %d\t %d\t %d\t %d\t %d\t %d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].pr,
               p[i].ft, tat, wt);
    }
    // Print total TAT and average WT
    printf("\nTotal Turnaround Time = %d\n", total_tat);
    printf("Average Waiting Time   = %.2f\n", total_wt / (float)N);

    //Draw the Gantt Chart
    printf("\nGantt Chart:\n|");
    for (int t = 1; t <= T; t++) {
        printf("%c|", gantt[t]);
    }
    printf("  (Time 1 → %d)\n", T);

    return 0;  // End of simulation
}
