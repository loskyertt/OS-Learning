#include "sche_Algo.hpp"
#include <iostream>

void test_FCFS() {
  vector<Process> process_lists;
  process_lists.push_back(Process("p2", 2, 4));
  process_lists.push_back(Process("p3", 4, 1));
  process_lists.push_back(Process("p1", 0, 7));
  process_lists.push_back(Process("p4", 5, 4));

  cout << "先来先服务：" << endl;
  fcfs(process_lists);
}

void test_SJF() {
  vector<Process> process_lists;
  process_lists.push_back(Process("p2", 2, 4));
  process_lists.push_back(Process("p3", 4, 1));
  process_lists.push_back(Process("p1", 0, 7));
  process_lists.push_back(Process("p4", 5, 4));

  std::cout << "短作业优先（非抢占式）：" << std::endl;
  sjf(process_lists);
}

void test_SRTF() {
  vector<Process> process_lists;
  process_lists.push_back(Process("p2", 2, 4));
  process_lists.push_back(Process("p3", 4, 1));
  process_lists.push_back(Process("p1", 0, 7));
  process_lists.push_back(Process("p4", 5, 4));

  std::cout << "最短剩余时间优先/短作业优先（抢占式）：" << std::endl;
  srtf(process_lists);
}

int main() {
  test_FCFS();

  cout << "\n-----------------------------\n"
       << endl;

  test_SJF();

  cout << "\n-----------------------------\n"
       << endl;

  test_SRTF();
}