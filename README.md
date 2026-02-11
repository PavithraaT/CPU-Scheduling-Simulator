# CPU-Scheduling-Simulator
C++ project simulating FCFS, SJF, SRTF, and Round Robin CPU scheduling algorithms.


**Language:** C++  
**Domain:** Operating Systems, CPU Scheduling  

---

## Overview
This project simulates and compares **four CPU scheduling algorithms** commonly taught in Operating Systems:

- **First-Come, First-Served (FCFS)**  
- **Shortest Job First (SJF – Non-preemptive)**  
- **Shortest Remaining Time First (SRTF – Preemptive SJF)**  
- **Round Robin (RR)**

It is designed as a **menu-driven simulator**, supporting manual or random process input, dynamic Gantt chart visualization, and calculation of key performance metrics.

---

## Features
- Interactive menu with options to select algorithms or compare all.  
- Supports **manual input** or **randomly generated processes**.  
- Handles **arrival time**, **burst time**, and **time quantum** for RR.  
- Dynamic **Gantt chart visualization** including idle CPU periods.  
- Calculates performance metrics:
  - Average Waiting Time (AWT)  
  - Average Turnaround Time (TAT)  
  - CPU Utilization  
  - Throughput (processes/unit time)  
- Comparison mode to evaluate multiple algorithms side by side.  
- Robust **input validation** and edge-case handling.

