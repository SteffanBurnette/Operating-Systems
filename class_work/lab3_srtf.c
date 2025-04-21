// Simulate Shortest Remaining Time First scheduling (preemptive).
// Prints per-time-unit CPU, waiting queue, completed list,
// then computes TAT, WT, total TAT, average WT, and draws a Gantt chart.

#include <stdio.h>
#include <stdlib.h>

#define N 4

typedef struct {
    char pid;    // Process ID
    int at;      // Arrival time
    int bt;      // Burst time
    int rem;     // Remaining time
    int ft;      // Finish time
} Proc;

int main() {
    //Processes
    Proc p[N] = {
        {'A', 1, 4, 4, 0},
        {'B', 2, 2, 2, 0},
        {'C', 4, 2, 2, 0},
        {'D', 5, 3, 3, 0}
    };

    int completed = 0;
    int time = 1;

    // Buffers for output
    char gantt[100];           // who ran at each time
    char waiting[100][N];      // waiting queue snapshots
    int wait_count[100] = {0};
    char done[100][N];         // completed list snapshots
    int done_count[100] = {0};

    //Simulation loop
    while (completed < N) {
        //Choosing the ready process with smallest remaining time
        int idx = -1;
        for (int i = 0; i < N; i++) {
            if (p[i].at <= time && p[i].rem > 0) {
                if (idx < 0 || p[i].rem < p[idx].rem) {
                    idx = i;
                }
            }
        }

        //record waiting queue (all ready & unfinished except current)
        int wc = 0;
        for (int i = 0; i < N; i++) {
            if (i != idx && p[i].at <= time && p[i].rem > 0) {
                waiting[time][wc++] = p[i].pid;
            }
        }
        wait_count[time] = wc;

        //record already completed
        int dc = 0;
        for (int i = 0; i < N; i++) {
            if (p[i].rem == 0) {
                done[time][dc++] = p[i].pid;
            }
        }
        done_count[time] = dc;

        //run CPU for one time unit
        if (idx >= 0) {
            gantt[time] = p[idx].pid;
            p[idx].rem--;
            if (p[idx].rem == 0) {
                p[idx].ft = time;
                completed++;
            }
        } else {
            gantt[time] = '-';  // idle
        }

        time++;
    }

    int T = time - 1;  // total time slots used

    //Print per-time-unit table
    printf("Time\tCPU\tWaiting\tCompleted\n");
    for (int t = 1; t <= T; t++) {
        printf("%d\t %c\t", t, gantt[t]);
        if (wait_count[t] > 0) {
            for (int i = 0; i < wait_count[t]; i++)
                printf("%c", waiting[t][i]);
        } else {
            printf("-");
        }
        printf("\t");
        if (done_count[t] > 0) {
            for (int i = 0; i < done_count[t]; i++)
                printf("%c", done[t][i]);
        } else {
            printf("-");
        }
        printf("\n");
    }

    //Compute and print metrics
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

    //Gantt Chart
    printf("\nGantt Chart:\n|");
    for (int t = 1; t <= T; t++) {
        printf("%c|", gantt[t]);
    }
    printf("  (Time 1→%d)\n", T);

    return 0;
}
