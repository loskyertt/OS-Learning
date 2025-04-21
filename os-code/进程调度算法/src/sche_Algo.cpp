#include "sche_Algo.hpp"
#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

/* 先来先服务 */
void fcfs(vector<Process> &process_lists) {
  int n = process_lists.size(); // 进程数量
  if (n == 0)
    return;

  // 按照到达时间升序排序
  sort(process_lists.begin(), process_lists.end(),
       [](const Process &a, const Process &b) {
         return a.arrival_time < b.arrival_time;
       });

  int current_time = 0; // 当前时间

  double total_turnaround_time = 0;
  double total_turnaround_time_weight = 0;
  double total_wait_time = 0;

  for (const auto &p : process_lists) {
    // 如果当前时间小于进程到达时间，CPU需要空闲等待
    if (current_time < p.arrival_time) {
      current_time = p.arrival_time;
    }

    int start_time = current_time; // 进程开始执行的时间
    current_time += p.serve_time;  // 更新当前时间

    double turnaround_time = current_time - p.arrival_time;         // 周转时间
    double turnaround_time_weight = turnaround_time / p.serve_time; // 带权周转时间
    double wait_time = start_time - p.arrival_time;                 // 等待时间

    cout << p.p_name << ": "
         << "周转时间：" << turnaround_time << ", "
         << "带权周转时间：" << turnaround_time_weight << ", "
         << "等待时间：" << wait_time << endl;

    total_turnaround_time += turnaround_time;
    total_turnaround_time_weight += turnaround_time_weight;
    total_wait_time += wait_time;
  }

  cout << endl;

  double average_turnaround_time = total_turnaround_time / n;
  cout << "平均周转时间：" << average_turnaround_time << endl;
  double average_turnaround_time_weight = total_turnaround_time_weight / n;
  cout << "平均带权周转时间：" << average_turnaround_time_weight << endl;
  double average_wait_time = total_wait_time / n;
  cout << "平均等待时间：" << average_wait_time << endl;
}

/* 短作业优先 */
void sjf(vector<Process> &process_lists) {
  int n = process_lists.size(); // 进程数量
  if (n == 0)
    return;

  // 按照到达时间排序
  sort(process_lists.begin(), process_lists.end(),
       [](const Process &a, const Process &b) {
         return a.arrival_time < b.arrival_time;
       });

  int current_time = 0;             // 当前时间
  vector<bool> completed(n, false); // 标记进程是否已完成
  int completed_count = 0;          // 已完成的进程数

  double total_turnaround_time = 0;
  double total_turnaround_time_weight = 0;
  double total_wait_time = 0;

  while (completed_count < n) {
    // 找出当前时间点已到达的、尚未执行的、服务时间最短的进程
    int shortest_job_idx = -1;
    int shortest_job_time = INT_MAX;

    for (int i = 0; i < n; i++) {
      // 如果进程已到达且尚未完成
      if (process_lists[i].arrival_time <= current_time && !completed[i]) {
        // 寻找服务时间最短的进程
        if (process_lists[i].serve_time < shortest_job_time) {
          shortest_job_time = process_lists[i].serve_time;
          shortest_job_idx = i;
        }
      }
    }

    // 如果没有找到可执行的进程（所有已到达的进程都已完成），时间推进到下一个进程的到达时间
    if (shortest_job_idx == -1) {
      int next_arrival_time = INT_MAX;
      for (int i = 0; i < n; i++) {
        if (!completed[i] && process_lists[i].arrival_time < next_arrival_time) {
          next_arrival_time = process_lists[i].arrival_time;
        }
      }
      current_time = next_arrival_time;
      continue;
    }

    // 执行找到的最短作业
    Process &p = process_lists[shortest_job_idx];
    int start_time = current_time; // 进程开始执行的时间
    current_time += p.serve_time;  // 更新当前时间

    double turnaround_time = current_time - p.arrival_time;         // 周转时间
    double turnaround_time_weight = turnaround_time / p.serve_time; // 带权周转时间
    double wait_time = start_time - p.arrival_time;                 // 等待时间

    cout << p.p_name << ": "
         << "周转时间：" << turnaround_time << ", "
         << "带权周转时间：" << turnaround_time_weight << ", "
         << "等待时间：" << wait_time << endl;

    total_turnaround_time += turnaround_time;
    total_turnaround_time_weight += turnaround_time_weight;
    total_wait_time += wait_time;

    completed[shortest_job_idx] = true; // 标记为已完成
    completed_count++;                  // 完成进程数量 +1
  }

  cout << endl;

  double average_turnaround_time = total_turnaround_time / n;
  cout << "平均周转时间：" << average_turnaround_time << endl;

  double average_turnaround_time_weight = total_turnaround_time_weight / n;
  cout << "平均带权周转时间：" << average_turnaround_time_weight << endl;

  double average_wait_time = total_wait_time / n;
  cout << "平均等待时间：" << average_wait_time << endl;
}

/* 抢占式短作业优先（最短剩余时间优先） */
void srtf(vector<Process> &process_lists) {
  int n = process_lists.size();
  if (n == 0)
    return;

  vector<int> remaining_time(n); // 记录各个进程剩余执行时间
  for (int i = 0; i < n; i++) {
    remaining_time[i] = process_lists[i].serve_time;
  }

  vector<int> completion_time(n, 0); // 记录每个进程完成的时间
  vector<bool> completed(n, false);  // 标记进程是否已完成

  int current_time = 0;
  int completed_count = 0;

  while (completed_count < n) {
    // 找出当前时间点，剩余时间最短的进程
    int shortest_job_idx = -1;
    int shortest_job_remaining = INT_MAX;

    for (int i = 0; i < n; i++) {
      // 如果进程已到达且尚未完成
      if (process_lists[i].arrival_time <= current_time && !completed[i]) {
        if (remaining_time[i] < shortest_job_remaining) {
          shortest_job_remaining = remaining_time[i];
          shortest_job_idx = i;
        }
      }
    }

    // 如果没有找到可执行的进程，时间推进到下一个进程的到达时间
    if (shortest_job_idx == -1) {
      int next_arrival_time = INT_MAX;
      for (int i = 0; i < n; i++) {
        if (!completed[i] && process_lists[i].arrival_time < next_arrival_time) {
          next_arrival_time = process_lists[i].arrival_time;
        }
      }
      current_time = next_arrival_time;
      continue;
    }

    // 执行进程一个时间单位
    remaining_time[shortest_job_idx]--;
    current_time++;

    // 如果进程执行完毕
    if (remaining_time[shortest_job_idx] == 0) {
      completed_count++;
      completed[shortest_job_idx] = true;
      completion_time[shortest_job_idx] = current_time;
    }
  }

  // 计算各项指标
  double total_turnaround_time = 0;
  double total_weighted_turnaround_time = 0;
  double total_waiting_time = 0;

  for (int i = 0; i < n; i++) {
    int turnaround_time = completion_time[i] - process_lists[i].arrival_time;
    double weighted_turnaround_time = static_cast<double>(turnaround_time) / process_lists[i].serve_time;
    int wait_time = turnaround_time - process_lists[i].serve_time;

    cout << process_lists[i].p_name << ": "
         << "周转时间：" << turnaround_time << ", "
         << "带权周转时间：" << weighted_turnaround_time << ", "
         << "等待时间：" << wait_time << endl;

    total_turnaround_time += turnaround_time;
    total_weighted_turnaround_time += weighted_turnaround_time;
    total_waiting_time += wait_time;
  }

  cout << endl;

  // 输出平均指标
  double average_turnaround_time = total_turnaround_time / n;
  cout << "平均周转时间：" << average_turnaround_time << endl;

  double average_weighted_turnaround_time = total_weighted_turnaround_time / n;
  cout << "平均带权周转时间：" << average_weighted_turnaround_time << endl;

  double average_waiting_time = total_waiting_time / n;
  cout << "平均等待时间：" << average_waiting_time << endl;
}