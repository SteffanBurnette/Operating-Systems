// round_robin.c
// Simulate Round Robin scheduling (quantum = 1)
// Prints per-time-unit CPU, waiting queue, completed list,
// then computes TAT, WT, total TAT, average WT, and draws a Gantt chart.

#include <stdio.h>
#include <stdlib.h>

#define N 4
#define QUANTUM 1

typedef struct {
    char pid;        // Process ID
    int at;          // Arrival time (starts at 1)
    int bt;          // Original burst time
    int rem;         // Remaining burst time
    int ft;          // Finish time
} Proc;

int main() {
    //The processes
    Proc p[N] = {
        {'A', 1, 4, 4, 0},
        {'B', 2, 2, 2, 0},
        {'C', 4, 2, 2, 0},
        {'D', 5, 3, 3, 0}
    };

    // Queue for ready processes (store indices)
    int queue[N*10], front = 0, back = 0;
    int completed = 0;
    int time = 1;   // current time (starts at 1)
    
    // For printing & metrics
    char gantt[100];      // who ran at each time
    char waiting[100][N]; // snapshots of waiting queue
    int wait_count[100] = {0};
    char done[100][N];    // snapshots of completed processes
    int done_count[100] = {0};

    
    while (completed < N) {
        // Enqueue any newly arrived processes
        for (int i = 0; i < N; i++) {
            if (p[i].at == time) {
                queue[back++] = i;
            }
        }

        // Pick next to run (if any)
        int curr = -1;
        if (front < back) {
            curr = queue[front++];
        }

        // Record waiting queue
        int wc = 0;
        for (int i = front; i < back; i++) {
            waiting[time][wc++] = p[ queue[i] ].pid;
        }
        wait_count[time] = wc;

        // Record already completed
        int dc = 0;
        for (int i = 0; i < N; i++) {
            if (p[i].rem == 0)
                done[time][dc++] = p[i].pid;
        }
        done_count[time] = dc;

        // Run the CPU for one quantum
        if (curr >= 0) {
            gantt[time] = p[curr].pid;
            p[curr].rem--;
            // If finished, mark finish time
            if (p[curr].rem == 0) {
                p[curr].ft = time;
                completed++;
            } else {
                // not finished → re‑enqueue
                queue[back++] = curr;
            }
        } else {
            gantt[time] = '-';  // idle CPU
        }

        time++;
    }

    int T = time - 1;  // total time slots used

    // 3) Print per-time-unit table
    printf("Time\tCPU\tWaiting\tCompleted\n");
    for (int t = 1; t <= T; t++) {
        printf("%d\t %c\t", t, gantt[t]);
        if (wait_count[t]) {
            for (int i = 0; i < wait_count[t]; i++)
                printf("%c", waiting[t][i]);
        } else {
            printf("-");
        }
        printf("\t");
        if (done_count[t]) {
            for (int i = 0; i < done_count[t]; i++)
                printf("%c", done[t][i]);
        } else {
            printf("-");
        }
        printf("\n");
    }

    //Metrics
    int total_tat = 0;
    int total_wt  = 0;
    printf("\nProcess\tAT\tBT\tFT\tTAT\tWT\n");
    for (int i = 0; i < N; i++) {
        int tat = p[i].ft - p[i].at;
        int wt  = tat - p[i].bt;
        total_tat += tat;
        total_wt  += wt;
        printf(" %c\t %d\t %d\t %d\t %d\t %d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ft, tat, wt);
    }
    printf("\nTotal Turnaround Time = %d\n", total_tat);
    printf("Average Waiting Time   = %.2f\n", total_wt / (float)N);

    // 5) Gantt Chart
    printf("\nGantt Chart:\n|");
    for (int t = 1; t <= T; t++) {
        printf("%c|", gantt[t]);
    }
    printf("  (Time 1→%d)\n", T);

    return 0;
}
