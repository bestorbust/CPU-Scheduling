#include <stdio.h>
#include <stdlib.h>

void fcfs();
void sjf_non_preemptive();
void sjf_preemptive();
void priority_non_preemptive();
void priority_preemptive();
void round_robin();

// Main function
int main() {
    int choice;
    do {
        printf("\nCPU Scheduling Simulator\n");
        printf("1. First-Come, First-Served (FCFS)\n");
        printf("2. Shortest Job First (SJF) - Non-Preemptive\n");
        printf("3. Shortest Job First (SJF) - Preemptive\n");
        printf("4. Priority Scheduling - Non-Preemptive\n");
        printf("5. Priority Scheduling - Preemptive\n");
        printf("6. Round Robin (RR)\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                fcfs();
                break;
            case 2:
                sjf_non_preemptive();
                break;
            case 3:
                sjf_preemptive();
                break;
            case 4:
                priority_non_preemptive();
                break;
            case 5:
                priority_preemptive();
                break;
            case 6:
                round_robin();
                break;
            case 7:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 7);

    return 0;
}

// FCFS Implementation
void fcfs() {
    int n, i;
    int bt[20], wt[20], tat[20];
    float avg_wt = 0, avg_tat = 0;

    printf("\n--- First-Come, First-Served (FCFS) Scheduling ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter burst time for each process:\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]: ", i + 1);
        scanf("%d", &bt[i]);
    }

    // Waiting time calculation
    wt[0] = 0;
    for(i = 1; i < n; i++) {
        wt[i] = wt[i - 1] + bt[i - 1];
    }

    // Turnaround time calculation
    for(i = 0; i < n; i++) {
        tat[i] = wt[i] + bt[i];
        avg_wt += wt[i];
        avg_tat += tat[i];
    }

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]\t%d\t\t%d\t\t%d\n", i + 1, bt[i], wt[i], tat[i]);
    }
    printf("\nAverage Waiting Time: %.2f", avg_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat/n);
}

// SJF Non-Preemptive Implementation
void sjf_non_preemptive() {
    int n, bt[20], p[20], wt[20], tat[20], i, j, pos, temp;
    float total_wt = 0, total_tat = 0;

    printf("\n--- Shortest Job First (Non-Preemptive) Scheduling ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter burst time for each process:\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]: ", i + 1);
        scanf("%d", &bt[i]);
        p[i] = i + 1;
    }

    // Sorting burst time in ascending order using selection sort
    for(i = 0; i < n; i++) {
        pos = i;
        for(j = i + 1; j < n; j++) {
            if(bt[j] < bt[pos])
                pos = j;
        }

        temp = bt[i];
        bt[i] = bt[pos];
        bt[pos] = temp;

        temp = p[i];
        p[i] = p[pos];
        p[pos] = temp;
    }

    wt[0] = 0;  // Waiting time for first process is 0

    // Waiting time calculation
    for(i = 1; i < n; i++) {
        wt[i] = 0;
        for(j = 0; j < i; j++)
            wt[i] += bt[j];
        total_wt += wt[i];
    }

    // Turnaround time calculation
    for(i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i];
        total_tat += tat[i];
    }

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]\t%d\t\t%d\t\t%d\n", p[i], bt[i], wt[i], tat[i]);
    }
    printf("\nAverage Waiting Time: %.2f", total_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat/n);
}
// SJF Preemptive (Shortest Remaining Time First) Implementation
void sjf_preemptive() {
    int arrival_time[10], burst_time[10], temp[10];
    int i, smallest, count = 0, time, n;
    double wait_time = 0, turnaround_time = 0, end;
    float average_waiting_time, average_turnaround_time;

    printf("\n--- Shortest Job First (Preemptive) Scheduling ---\n");
    printf("Enter number of Processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("Enter arrival time for Process P[%d]: ", i + 1);
        scanf("%d", &arrival_time[i]);
        printf("Enter burst time for Process P[%d]: ", i + 1);
        scanf("%d", &burst_time[i]);
        temp[i] = burst_time[i];
    }

    burst_time[9] = 9999;
    for(time = 0; count != n; time++) {
        smallest = 9;
        for(i = 0; i < n; i++) {
            if(arrival_time[i] <= time && burst_time[i] < burst_time[smallest] && burst_time[i] > 0)
                smallest = i;
        }
        burst_time[smallest]--;

        if(burst_time[smallest] == 0) {
            count++;
            end = time + 1;
            wait_time += end - arrival_time[smallest] - temp[smallest];
            turnaround_time += end - arrival_time[smallest];
        }
    }

    average_waiting_time = wait_time / n;
    average_turnaround_time = turnaround_time / n;

    printf("\nAverage Waiting Time: %.2f", average_waiting_time);
    printf("\nAverage Turnaround Time: %.2f\n", average_turnaround_time);
}

// Priority Non-Preemptive Implementation
void priority_non_preemptive() {
    int bt[20], p[20], wt[20], tat[20], pr[20], i, j, n, pos, temp;
    float total_wt = 0, total_tat = 0;

    printf("\n--- Priority Scheduling (Non-Preemptive) ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("\nEnter burst time for Process P[%d]: ", i + 1);
        scanf("%d", &bt[i]);
        printf("Enter priority for Process P[%d]: ", i + 1);
        scanf("%d", &pr[i]);
        p[i] = i + 1;
    }

    // Sorting based on priority
    for(i = 0; i < n; i++) {
        pos = i;
        for(j = i + 1; j < n; j++) {
            if(pr[j] < pr[pos])
                pos = j;
        }

        temp = pr[i];
        pr[i] = pr[pos];
        pr[pos] = temp;

        temp = bt[i];
        bt[i] = bt[pos];
        bt[pos] = temp;

        temp = p[i];
        p[i] = p[pos];
        p[pos] = temp;
    }

    wt[0] = 0;
    for(i = 1; i < n; i++) {
        wt[i] = 0;
        for(j = 0; j < i; j++)
            wt[i] += bt[j];
        total_wt += wt[i];
    }

    for(i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i];
        total_tat += tat[i];
    }

    printf("\nProcess\tBurst Time\tPriority\tWaiting Time\tTurnaround Time");
    for(i = 0; i < n; i++)
        printf("\nP[%d]\t%d\t\t%d\t\t%d\t\t%d", p[i], bt[i], pr[i], wt[i], tat[i]);

    printf("\n\nAverage Waiting Time: %.2f", total_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat/n);
}

// Priority Preemptive Implementation
void priority_preemptive() {
    int bt[20], p[20], wt[20], tat[20], pr[20], rt[20], i, j, n, time = 0, count = 0, smallest;
    float total_wt = 0, total_tat = 0;

    printf("\n--- Priority Scheduling (Preemptive) ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        printf("\nEnter burst time for Process P[%d]: ", i + 1);
        scanf("%d", &bt[i]);
        printf("Enter priority for Process P[%d]: ", i + 1);
        scanf("%d", &pr[i]);
        p[i] = i + 1;
        rt[i] = bt[i];
    }

    while(count != n) {
        smallest = -1;
        int highest_priority = 99999;

        for(i = 0; i < n; i++) {
            if(rt[i] > 0 && pr[i] < highest_priority) {
                highest_priority = pr[i];
                smallest = i;
            }
        }

        if(smallest == -1)
            break;

        rt[smallest]--;
        time++;

        if(rt[smallest] == 0) {
            count++;
            tat[smallest] = time;
            wt[smallest] = time - bt[smallest];
        }
    }

    for(i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
    }

    printf("\nProcess\tBurst Time\tPriority\tWaiting Time\tTurnaround Time");
    for(i = 0; i < n; i++)
        printf("\nP[%d]\t%d\t\t%d\t\t%d\t\t%d", p[i], bt[i], pr[i], wt[i], tat[i]);

    printf("\n\nAverage Waiting Time: %.2f", total_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat/n);
}

// Round Robin Implementation
void round_robin() {
    int i, j, n, time, remain, flag = 0, time_quantum;
    int wait_time = 0, turnaround_time = 0, at[10], bt[10], rt[10];

    printf("\n--- Round Robin (RR) Scheduling ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    remain = n;

    for(i = 0; i < n; i++) {
        printf("Enter arrival time for Process P[%d]: ", i + 1);
        scanf("%d", &at[i]);
        printf("Enter burst time for Process P[%d]: ", i + 1);
        scanf("%d", &bt[i]);
        rt[i] = bt[i];
    }

    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    printf("\nProcess\t|Turnaround Time|Waiting Time\n");

    for(time = 0, i = 0; remain != 0;) {
        if(rt[i] <= time_quantum && rt[i] > 0) {
            time += rt[i];
            rt[i] = 0;
            flag = 1;
        }
        else if(rt[i] > 0) {
            rt[i] -= time_quantum;
            time += time_quantum;
        }

        if(rt[i] == 0 && flag == 1) {
            remain--;
            printf("P[%d]\t|\t%d\t|\t%d\n", i + 1, time - at[i], time - at[i] - bt[i]);
            wait_time += time - at[i] - bt[i];
            turnaround_time += time - at[i];
            flag = 0;
        }

        if(i == n - 1)
            i = 0;
        else if(at[i + 1] <= time)
            i++;
        else
            i = 0;
    }

    printf("\nAverage Waiting Time: %.2f", (float)wait_time/n);
    printf("\nAverage Turnaround Time: %.2f\n", (float)turnaround_time/n);
}
