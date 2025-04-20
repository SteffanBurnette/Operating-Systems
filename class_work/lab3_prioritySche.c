// priority_np.c
// Simulate Non-Preemptive Priority scheduling.
// Only includes <stdio.h> and <stdlib.h>.
// Prints per-time-unit CPU, waiting queue, completed list,
// then computes TAT, WT, total TAT, average WT, and draws a Gantt chart.

#include <stdio.h>
#include <stdlib.h>

#define N 4

typedef struct {
    char pid;    // Process ID
    int at;      // Arrival time
    int bt;      // Burst time
    int pr;      // Priority (low value = high priority)
    int rem;     // Remaining time
    int ft;      // Finish time
} Proc;

int main() {
    // 1) Define the processes
    Proc p[N] = {
        {'A', 1, 4, 1, 4, 0},
        {'B', 2, 2, 2, 2, 0},
        {'C', 4, 2, 4, 2, 0},
        {'D', 5, 3, 3, 3, 0}
    };

    int completed = 0;
    int time = 1;
    int curr = -1;  // index of the running process, or -1 if CPU idle

    // Buffers for output
    char gantt[100];          // which PID ran at each time
    char waiting[100][N];     // waiting queue snapshots
    int wait_count[100] = {0};
    char done[100][N];        // completed list snapshots
    int done_count[100] = {0};

    // 2) Simulation loop
    while (completed < N) {
        // If CPU is free, pick the highest-priority ready process
        if (curr < 0) {
            int best = -1;
            for (int i = 0; i < N; i++) {
                if (p[i].at <= time && p[i].rem > 0) {
                    if (best < 0 || p[i].pr < p[best].pr) {
                        best = i;
                    }
                }
            }
            curr = best;  // may stay -1 if no one is ready
        }

        // Record waiting queue (all ready & unfinished except current)
        int wc = 0;
        for (int i = 0; i < N; i++) {
            if (i != curr && p[i].at <= time && p[i].rem > 0) {
                waiting[time][wc++] = p[i].pid;
            }
        }
        wait_count[time] = wc;

        // Record already completed processes
        int dc = 0;
        for (int i = 0; i < N; i++) {
            if (p[i].rem == 0) {
                done[time][dc++] = p[i].pid;
            }
        }
        done_count[time] = dc;

        // Run CPU for one time unit
        if (curr >= 0) {
            gantt[time] = p[curr].pid;
            p[curr].rem--;
            // If it finishes, record finish time and free the CPU
            if (p[curr].rem == 0) {
                p[curr].ft = time;
                completed++;
                curr = -1;
            }
        } else {
            gantt[time] = '-';  // idle CPU
        }

        time++;
    }

    int T = time - 1;  // total time units used

    // 3) Print per-time-unit table
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

    // 4) Compute and print metrics
    int total_tat = 0, total_wt = 0;
    printf("\nProcess\tAT\tBT\tPR\tFT\tTAT\tWT\n");
    for (int i = 0; i < N; i++) {
        int tat = p[i].ft - p[i].at;
        int wt  = tat - p[i].bt;
        total_tat += tat;
        total_wt  += wt;
        printf(" %c\t %d\t %d\t %d\t %d\t %d\t %d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].pr,
               p[i].ft, tat, wt);
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
