#include <stdio.h>
#include <stdlib.h>

void fcfs();
void sjf_non_preemptive();
void sjf_preemptive();
void priority_non_preemptive();
void priority_preemptive();
void round_robin();

int main() {
    int choice;
    do {
        printf("\nCPU Scheduling Simulator\n");
        printf("1. First-Come, First-Served (FCFS)\n");
        printf("2. Shortest Job First (SJF) - Non-Preemptive\n");
        printf("3. Shortest Job First (SRTF) - Preemptive\n");
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

// First-Come, First-Served (FCFS)
void fcfs() {
    int n, i;
    int bt[20], at[20], wt[20], tat[20], ct[20];
    float avg_wt = 0, avg_tat = 0;

    printf("\n--- First-Come, First-Served (FCFS) Scheduling ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time and burst time for each process:\n");
    for(i = 0; i < n; i++) {
        printf("Process P[%d]:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &at[i]);
        printf("Burst Time: ");
        scanf("%d", &bt[i]);
    }

    // Calculate completion time
    ct[0] = at[0] + bt[0];
    for(i = 1; i < n; i++) {
        if (at[i] > ct[i - 1])
            ct[i] = at[i] + bt[i];
        else
            ct[i] = ct[i - 1] + bt[i];
    }

    // Calculate waiting and turnaround times
    for(i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        avg_wt += wt[i];
        avg_tat += tat[i];
    }
    
   


    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }
    
    float throughput = (float)n / (float)(tat[n - 1]);  
    printf("\nThroughput: %.2f processes per unit time\n", throughput);
    printf("\nAverage Waiting Time: %.2f", avg_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat/n);
    

}

// Shortest Job First (Non-Preemptive)
void sjf_non_preemptive() {
    int n, i, j, min_index, time = 0;
    int at[20], bt[20], ct[20], tat[20], wt[20], completed[20] = {0};
    float total_wt = 0, total_tat = 0;

    printf("\n--- Shortest Job First (Non-Preemptive) ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time and burst time for each process:\n");
    for(i = 0; i < n; i++) {
        printf("Process P[%d]:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &at[i]);
        printf("Burst Time: ");
        scanf("%d", &bt[i]);
    }

    int completed_processes = 0;
    while (completed_processes < n) {
        min_index = -1;

        // Find the process with the shortest burst time that has arrived and is not yet completed
        for (j = 0; j < n; j++) {
            if (completed[j] == 0 && at[j] <= time) {
                if (min_index == -1 || bt[j] < bt[min_index]) {
                    min_index = j;
                }
            }
        }

        if (min_index == -1) {
            time++;
            continue;
        }

        // Process the selected job
        time += bt[min_index];
        ct[min_index] = time;
        tat[min_index] = ct[min_index] - at[min_index];
        wt[min_index] = tat[min_index] - bt[min_index];
        
        total_wt += wt[min_index];
        total_tat += tat[min_index];

        completed[min_index] = 1;
        completed_processes++;
    }

    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }
    
    float throughput = (float)n / (float)(tat[n - 1]);  
    printf("\nThroughput: %.2f processes per unit time\n", throughput);
    printf("\nAverage Waiting Time: %.2f", total_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat/n);
}


// Shortest Job First (Preemptive)
void sjf_preemptive() {
    int n, i, j, time = 0, smallest, count = 0, end;
    int at[20], bt[20], remaining_bt[20], ct[20], tat[20], wt[20];
    float total_wt = 0, total_tat = 0;

    printf("\n--- Shortest Job First (Preemptive) Scheduling ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time and burst time for each process:\n");
    for(i = 0; i < n; i++) {
        printf("Process P[%d]:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &at[i]);
        printf("Burst Time: ");
        scanf("%d", &bt[i]);
        remaining_bt[i] = bt[i];
    }

    remaining_bt[19] = 9999; // large value for comparison
    for(time = 0; count != n; time++) {
        smallest = 19;
        for(i = 0; i < n; i++) {
            if(at[i] <= time && remaining_bt[i] < remaining_bt[smallest] && remaining_bt[i] > 0) {
                smallest = i;
            }
        }
        remaining_bt[smallest]--;
        if(remaining_bt[smallest] == 0) {
            count++;
            end = time + 1;
            ct[smallest] = end;
            tat[smallest] = ct[smallest] - at[smallest];
            wt[smallest] = tat[smallest] - bt[smallest];
            total_wt += wt[smallest];
            total_tat += tat[smallest];
        }
    }

    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }
    
    float throughput = (float)n / (float)(tat[n - 1]);  
    printf("\nThroughput: %.2f processes per unit time\n", throughput);
    printf("\nAverage Waiting Time: %.2f", total_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat/n);
}

// Priority Scheduling (Non-Preemptive)
void priority_non_preemptive() {
    int n, i, j, max_index, time = 0;
    int at[20], bt[20], priority[20], ct[20], wt[20], tat[20], completed[20] = {0};
    float total_wt = 0, total_tat = 0;

    printf("\n--- Priority Scheduling (Non-Preemptive) ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time, burst time, and priority for each process:\n");
    for(i = 0; i < n; i++) {
        printf("Process P[%d]:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &at[i]);
        printf("Burst Time: ");
        scanf("%d", &bt[i]);
        printf("Priority: ");
        scanf("%d", &priority[i]);
    }

    int completed_processes = 0;
    time = 0;

    while (completed_processes < n) {
        max_index = -1;

        // Find the highest priority process that has arrived and is not completed
        for (j = 0; j < n; j++) {
            if (completed[j] == 0 && at[j] <= time) {
                if (max_index == -1 || priority[j] > priority[max_index]) {  // Reverse comparison
                    max_index = j;
                }
            }
        }

        // If no process is ready, increment time to next arrival
        if (max_index == -1) {
            time++;
            continue;
        }

        // Execute the selected process
        time += bt[max_index];
        ct[max_index] = time;
        tat[max_index] = ct[max_index] - at[max_index];
        wt[max_index] = tat[max_index] - bt[max_index];

        total_wt += wt[max_index];
        total_tat += tat[max_index];

        completed[max_index] = 1;
        completed_processes++;
    }

    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, at[i], bt[i], priority[i], ct[i], tat[i], wt[i]);
    }
    
    float throughput = (float)n / (float)(tat[n - 1]);  
    printf("\nThroughput: %.2f processes per unit time\n", throughput);
    printf("\nAverage Waiting Time: %.2f", total_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat/n);
}


// Priority Scheduling (Preemptive)
void priority_preemptive() {
    int n, i, time, largest, remain = 0, end;
    int at[20], bt[20], remaining_bt[20], priority[20], ct[20], wt[20], tat[20];
    float total_wt = 0, total_tat = 0;

    printf("\n--- Priority Scheduling (Preemptive) ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter arrival time, burst time, and priority for each process:\n");
    for(i = 0; i < n; i++) {
        printf("Process P[%d]:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &at[i]);
        printf("Burst Time: ");
        scanf("%d", &bt[i]);
        printf("Priority: ");
        scanf("%d", &priority[i]);
        remaining_bt[i] = bt[i];  // Initialize remaining burst time
    }

    priority[19] = 9999; // Dummy priority for comparison
    for(time = 0; remain != n; time++) {
        largest = -1;

        // Find the process with the highest priority (largest value) that has arrived and has remaining burst time
        for(i = 0; i < n; i++) {
            if(at[i] <= time && remaining_bt[i] > 0) {
                if (largest == -1 || priority[i] > priority[largest]) {  // Reverse comparison
                    largest = i;
                }
            }
        }

        // If no process is ready, skip this time unit
        if(largest == -1) {
            continue;
        }

        remaining_bt[largest]--; // Process the selected job for 1 time unit

        // If process is finished
        if(remaining_bt[largest] == 0) {
            remain++;
            end = time + 1;
            ct[largest] = end;
            tat[largest] = ct[largest] - at[largest];
            wt[largest] = tat[largest] - bt[largest];
            total_wt += wt[largest];
            total_tat += tat[largest];
        }
    }

    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, at[i], bt[i], priority[i], ct[i], tat[i], wt[i]);
    }
    
    float throughput = (float)n / (float)(tat[n - 1]);  
    printf("\nThroughput: %.2f processes per unit time\n", throughput);
    printf("\nAverage Waiting Time: %.2f", total_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat/n);
}


// Round Robin Scheduling
void round_robin() {
    int i, j, n, tq, time = 0, remain, flag = 0;
    int at[10], bt[10], rt[10], wt[10], tat[10], ct[10];
    float total_wt = 0, total_tat = 0;

    printf("\n--- Round Robin (RR) Scheduling ---\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);
    remain = n;

    printf("Enter arrival time and burst time for each process:\n");
    for(i = 0; i < n; i++) {
        printf("Process P[%d]:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &at[i]);
        printf("Burst Time: ");
        scanf("%d", &bt[i]);
        rt[i] = bt[i];
    }

    printf("Enter time quantum: ");
    scanf("%d", &tq);

    for(time = 0, i = 0; remain != 0;) {
        if(rt[i] <= tq && rt[i] > 0) {
            time += rt[i];
            rt[i] = 0;
            flag = 1;
        } else if(rt[i] > 0) {
            rt[i] -= tq;
            time += tq;
        }
        if(rt[i] == 0 && flag == 1) {
            remain--;
            ct[i] = time;
            tat[i] = time - at[i];
            wt[i] = tat[i] - bt[i];
            total_wt += wt[i];
            total_tat += tat[i];
            flag = 0;
        }
        if(i == n - 1) i = 0;
        else if(at[i + 1] <= time) i++;
        else i = 0;
    }

    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for(i = 0; i < n; i++) {
        printf("P[%d]\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, at[i], bt[i], ct[i], tat[i], wt[i]);
    }
    
    float throughput = (float)n / (float)(tat[n - 1]);  
    printf("\nThroughput: %.2f processes per unit time\n", throughput);
    printf("\nAverage Waiting Time: %.2f", total_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat/n);
}
