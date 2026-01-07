#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>

using namespace std;

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
    bool ioBound;
    string queueHistory;
    bool finished;
    bool isQueued;
};

int main() {
    int n, baseQuantum;

    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter base time quantum: ";
    cin >> baseQuantum;

    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "\nP" << i + 1 << " Arrival: "; cin >> p[i].arrivalTime;
        cout << "P" << i + 1 << " Burst: "; cin >> p[i].burstTime;
        cout << "P" << i + 1 << " Priority(1-6): "; cin >> p[i].priority;
        cout << "P" << i + 1 << " IO Bound (1=Yes, 0=No): "; cin >> p[i].ioBound;

        p[i].remainingTime = p[i].burstTime;
        p[i].finished = false;
        p[i].isQueued = false;
        p[i].queueHistory = "";
    }

    queue<int> Q1, Q2, Q3;
    int time = 0, completed = 0;

    while (completed < n) {
        // 1. Arrival Check
        for (int i = 0; i < n; i++) {
            if (!p[i].finished && !p[i].isQueued && p[i].arrivalTime <= time) {
                int q;
                if (p[i].priority <= 2) { q = 1; Q1.push(i); }
                else if (p[i].priority <= 4) { q = 2; Q2.push(i); }
                else { q = 3; Q3.push(i); }
                
                p[i].queueHistory += "Q" + string(1, q + '0'); 
                p[i].isQueued = true;
            }
        }

        int idx = -1, quantum = 0, currentQ = 0;

        // 2. Queue Selection
        if (!Q1.empty()) { idx = Q1.front(); Q1.pop(); quantum = baseQuantum; currentQ = 1; }
        else if (!Q2.empty()) { idx = Q2.front(); Q2.pop(); quantum = baseQuantum + 2; currentQ = 2; }
        else if (!Q3.empty()) { idx = Q3.front(); Q3.pop(); quantum = baseQuantum + 4; currentQ = 3; }
        else {
            time++; // Idle
            continue;
        }

        // 3. Execution
        int exec = min(p[idx].remainingTime, quantum);
        p[idx].remainingTime -= exec;
        time += exec;

        // 4. Post-execution Logic
        if (p[idx].remainingTime == 0) {
            p[idx].finished = true;
            completed++;
            p[idx].turnaroundTime = time - p[idx].arrivalTime;
            p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;
        } else {
            if (p[idx].ioBound) {
                if (p[idx].priority > 1) p[idx].priority--;
                int nq;
                if (p[idx].priority <= 2) { nq = 1; Q1.push(idx); }
                else if (p[idx].priority <= 4) { nq = 2; Q2.push(idx); }
                else { nq = 3; Q3.push(idx); }
                p[idx].queueHistory += "->Q" + string(1, nq + '0');
            } else {
                int nq = (currentQ == 1) ? 2 : 3;
                p[idx].queueHistory += "->Q" + string(1, nq + '0');
                if (nq == 2) Q2.push(idx); else Q3.push(idx);
            }
        }
    }

    // Output
    cout << "\nPID\tAT\tWT\tTAT\tHistory\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].pid << "\t" 
             << p[i].arrivalTime << "\t" 
             << p[i].waitingTime << "\t" 
             << p[i].turnaroundTime << "\t" 
             << p[i].queueHistory << endl;
    }

    return 0;
}    