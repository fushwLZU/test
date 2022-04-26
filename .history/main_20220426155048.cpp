
#include <iostream>
#include <vector>
// #include <memory>
// #include "helper1.h"
// #include "helper2.h"

// #include <ctime>
// #include <ratio>
// #include <chrono>
// #include "helper1.cpp"  
#include <thread>
#include <future>
#include <numeric>
#include <chrono>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <windows.h>
#include <chrono>
#include <ratio> 
#include <array>
#include <algorithm> 
#include <memory>
#include <mutex>
#include <deque>
#include <atomic>
// #include <sys/mman.h>
// #include <unistd.h>
#include <sched.h>
using namespace std;




class ThreadPool
{
public:
	ThreadPool(int n);
	~ThreadPool();

	void pushTask(std::function<int(vector<int>&)> task, vector<int>& node);
	void waitAllTask();

private:
	std::vector<std::thread*> threadPool;
	std::deque<std::pair<std::function<int(vector<int>&)>,vector<int>>> taskQueue;
	std::atomic<int> busyCount {0};
  
	bool bStop = false;

	void taskConsumer();
	std::mutex taskQueueMutex;
	std::condition_variable taskQueueCond;
	std::condition_variable taskFinishedCond;
};
ThreadPool::ThreadPool(int n)
{
	for (int i = 0; i < n; i++)
	{
		std::thread *t = new std::thread(&ThreadPool::taskConsumer,this);
		threadPool.push_back(t);
		t->detach();
	}
}

ThreadPool::~ThreadPool()
{
	while (!threadPool.empty())
	{
		std::thread *t = threadPool.back();
		threadPool.pop_back();
		delete t;
	}
}

void ThreadPool::pushTask(std::function<int(vector<int>&)> task, vector<int>& node)
{
	{
		// cout<<"push"<<endl;
		std::lock_guard<std::mutex> guard(taskQueueMutex);
		taskQueue.push_back(std::make_pair(std::move(task),node));
	}
	taskQueueCond.notify_one();
}
void ThreadPool::taskConsumer()
{
	while (!bStop)
	{
		std::unique_lock<std::mutex> lk(taskQueueMutex);
		// cout<<"point..."<<endl;
		taskQueueCond.wait(lk, [&] {return !taskQueue.empty(); });
		busyCount++;
    	// cout << "busycount=" << busyCount<< endl;
		std::function<int(vector<int>&)> task = taskQueue.front().first;
    	vector<int>& nodes = taskQueue.front().second;
		taskQueue.pop_front();
		lk.unlock();
		task(nodes);
		busyCount--;
		taskFinishedCond.notify_one();
	}
}

void ThreadPool::waitAllTask()
{
	std::unique_lock<std::mutex> lk(taskQueueMutex);
	taskFinishedCond.wait(lk, [&] {return taskQueue.empty() && busyCount==0; });//所有任务均已完成
}

class Worker
{
public:
	int run(vector<int>& a);
};

int Worker::run(vector<int>& a)
{
	// cout << sched_getcpu() << endl;
  	cout << a.size() << endl;
	cout << "Worker::run start" << endl;
	Sleep(500);
	cout << "Worker::run end" << endl;
  	return 0;
}
int main()
{
  ThreadPool pool(2);
  Worker worker;
  vector<int> aa({1,2,3});
  vector<int> bb({1,2,3,4});

  auto start = std::chrono::high_resolution_clock::now();

  pool.pushTask((bind(&Worker::run, &worker, aa)),aa);
  pool.pushTask((bind(&Worker::run, &worker, bb)),bb);


  // pool.pushTask( [&](){worker.run(aa); },aa);//1

  Sleep(15000);
//   pool.waitAllTask();

//   pool.pushTask((bind(&Worker::run, &worker, aa)),aa);
//   pool.pushTask((bind(&Worker::run, &worker, bb)),bb);
//   pool.waitAllTask();
//   pool.pushTask((bind(&Worker::run, &worker, aa)),aa);
//   pool.pushTask((bind(&Worker::run, &worker, bb)),bb);
//   pool.waitAllTask();
//   pool.pushTask((bind(&Worker::run, &worker, aa)),aa);
//   pool.pushTask((bind(&Worker::run, &worker, bb)),bb);
//   pool.waitAllTask();


  // std::thread t1 = std::thread(&Worker::run, &worker, 4);
  // std::thread t2 = std::thread(&Worker::run, &worker, 3);
  // t1.join();
  // t2.join();

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double,std::ratio<1,1>> ds = end - start;
  std::chrono::milliseconds d = std::chrono::duration_cast< std::chrono::milliseconds >( ds );
  cout<<d.count()<<"milliseconds"<<endl;
  std::chrono::duration<double,std::ratio<1,1000000>> duration_mcs=std::chrono::duration_cast<std::chrono::duration<double,std::ratio<1,1000000>>> (end-start);  
  std::cout<<duration_mcs.count()<<"microseconds"<<std::endl; 

	//15:50
  
}

