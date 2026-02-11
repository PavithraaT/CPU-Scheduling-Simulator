#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
};

void printGantt(vector<int> gantt) {
    cout << "\nGantt Chart:\n ";

    for (int i = 0; i < gantt.size(); i++)
        cout << "------";
    cout << "\n|";

    for (int p : gantt) {
        if (p == -1)
            cout << " Idle|";
        else
            cout << "  P" << p << "  |";
    }

    cout << "\n ";
    for (int i = 0; i < gantt.size(); i++)
        cout << "------";

    cout << "\n0";
    for (int i = 1; i <= gantt.size(); i++)
        cout << setw(6) << i;

    cout << "\n";
}

void printResults(vector<Process> &p, int totalTime) {
    double totalWT = 0, totalTAT = 0;

    cout << "\nPID\tAT\tBT\tCT\tWT\tTAT\n";
    for (auto &pr : p) {
        cout << "P" << pr.pid << "\t"
             << pr.arrival << "\t"
             << pr.burst << "\t"
             << pr.completion << "\t"
             << pr.waiting << "\t"
             << pr.turnaround << "\n";

        totalWT += pr.waiting;
        totalTAT += pr.turnaround;
    }

    double avgWT = totalWT / p.size();
    double avgTAT = totalTAT / p.size();

    int totalBurst = 0;
    for (auto &pr : p)
        totalBurst += pr.burst;

    double cpuUtil = (totalTime == 0) ? 0 : ((double)totalBurst / totalTime) * 100;
    double throughput = (totalTime == 0) ? 0 : (double)p.size() / totalTime;

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << avgWT;
    cout << "\nAverage Turnaround Time: " << avgTAT;
    cout << "\nCPU Utilization: " << cpuUtil << "%";
    cout << "\nThroughput: " << throughput << " processes/unit time\n";
}

vector<Process> inputProcesses(int n, bool randomGen = false) {
    vector<Process> p(n);

    if (randomGen) {
        srand(time(0));
        for (int i = 0; i < n; i++) {
            p[i].pid = i + 1;
            p[i].arrival = rand() % 5;
            p[i].burst = rand() % 9 + 1;
            p[i].remaining = p[i].burst;
        }
    } else {
        for (int i = 0; i < n; i++) {
            p[i].pid = i + 1;

            do {
                cout << "Process " << i + 1 << " Arrival Time (>=0): ";
                cin >> p[i].arrival;
            } while (p[i].arrival < 0);

            do {
                cout << "Process " << i + 1 << " Burst Time (>0): ";
                cin >> p[i].burst;
            } while (p[i].burst <= 0);

            p[i].remaining = p[i].burst;
        }
    }
    return p;
}

void FCFS(vector<Process> p) {
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.arrival < b.arrival;
    });

    int time = 0;
    vector<int> gantt;

    for (auto &pr : p) {
        while (time < pr.arrival) {
            gantt.push_back(-1);
            time++;
        }

        for (int i = 0; i < pr.burst; i++) {
            gantt.push_back(pr.pid);
            time++;
        }

        pr.completion = time;
        pr.turnaround = pr.completion - pr.arrival;
        pr.waiting = pr.turnaround - pr.burst;
    }

    printGantt(gantt);
    printResults(p, time);
}

void SJF(vector<Process> p) {
    int time = 0, completed = 0, n = p.size();
    vector<int> gantt;

    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0 && p[i].burst < minBurst) {
                minBurst = p[i].burst;
                idx = i;
            }
        }

        if (idx == -1) {
            gantt.push_back(-1);
            time++;
        } else {
            for (int i = 0; i < p[idx].burst; i++) {
                gantt.push_back(p[idx].pid);
                time++;
            }

            p[idx].remaining = 0;
            p[idx].completion = time;
            p[idx].turnaround = time - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            completed++;
        }
    }

    printGantt(gantt);
    printResults(p, time);
}

void SRTF(vector<Process> p) {
    int time = 0, completed = 0, n = p.size();
    vector<int> gantt;

    while (completed < n) {
        int idx = -1;
        int minRemaining = 1e9;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0 && p[i].remaining < minRemaining) {
                minRemaining = p[i].remaining;
                idx = i;
            }
        }

        if (idx == -1) {
            gantt.push_back(-1);
            time++;
        } else {
            gantt.push_back(p[idx].pid);
            p[idx].remaining--;
            time++;

            if (p[idx].remaining == 0) {
                p[idx].completion = time;
                p[idx].turnaround = time - p[idx].arrival;
                p[idx].waiting = p[idx].turnaround - p[idx].burst;
                completed++;
            }
        }
    }

    printGantt(gantt);
    printResults(p, time);
}

void RoundRobin(vector<Process> p, int quantum) {
    int time = 0, completed = 0, n = p.size();
    queue<int> q;
    vector<int> gantt;
    vector<bool> visited(n, false);

    while (completed < n) {

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && !visited[i]) {
                q.push(i);
                visited[i] = true;
            }
        }

        if (q.empty()) {
            gantt.push_back(-1);
            time++;
            continue;
        }

        int idx = q.front();
        q.pop();

        int runTime = min(quantum, p[idx].remaining);

        for (int i = 0; i < runTime; i++) {
            gantt.push_back(p[idx].pid);
            time++;
            p[idx].remaining--;

            for (int j = 0; j < n; j++) {
                if (p[j].arrival <= time && !visited[j]) {
                    q.push(j);
                    visited[j] = true;
                }
            }
        }

        if (p[idx].remaining > 0)
            q.push(idx);
        else {
            p[idx].completion = time;
            p[idx].turnaround = time - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            completed++;
        }
    }

    printGantt(gantt);
    printResults(p, time);
}

int main() {
    int choice, n;

    cout << "===== Advanced CPU Scheduling Simulator =====\n";

    while (true) {
        cout << "\n1. FCFS\n";
        cout << "2. SJF (Non-Preemptive)\n";
        cout << "3. SRTF (Preemptive SJF)\n";
        cout << "4. Round Robin\n";
        cout << "5. Compare All Algorithms\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 6) break;

        cout << "Number of processes: ";
        cin >> n;

        int mode;
        cout << "1. Manual Input\n2. Random Generate\nChoice: ";
        cin >> mode;

        vector<Process> processes = inputProcesses(n, mode == 2);

        if (choice == 1)
            FCFS(processes);
        else if (choice == 2)
            SJF(processes);
        else if (choice == 3)
            SRTF(processes);
        else if (choice == 4) {
            int quantum;
            cout << "Enter Time Quantum: ";
            cin >> quantum;
            RoundRobin(processes, quantum);
        }
        else if (choice == 5) {
            cout << "\n======= COMPARISON MODE =======\n";
            cout << "\n--- FCFS ---\n";
            FCFS(processes);

            cout << "\n--- SJF ---\n";
            SJF(processes);

            cout << "\n--- SRTF ---\n";
            SRTF(processes);

            int quantum;
            cout << "\nEnter Time Quantum for Round Robin: ";
            cin >> quantum;

            cout << "\n--- Round Robin ---\n";
            RoundRobin(processes, quantum);
        }
        else
            cout << "Invalid choice!\n";
    }

    cout << "Exiting...\n";
    return 0;
}
