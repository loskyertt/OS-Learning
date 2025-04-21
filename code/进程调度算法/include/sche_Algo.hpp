#pragma once

#include <string>
#include <vector>

using namespace std;

struct Process {
  string p_name;
  int arrival_time;
  int serve_time;
  Process(const string name, int x, int y) : p_name(name), arrival_time(x), serve_time(y) {}
};

/* 先来先服务 */
void fcfs(vector<Process> &process_lists);

/* 短作业优先 */
void sjf(vector<Process> &process_lists);

/* 抢占式短作业优先（最短剩余时间优先） */
void srtf(vector<Process> &process_lists);