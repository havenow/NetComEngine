#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>//原子
#include "CELLTimestamp.hpp"
using namespace std;
//原子操作
mutex m;
const int tCount = 4;
atomic<int> sum = 0;//效率比加锁高
void workFun(int index)
{
	for (int n = 0; n < 20000000; n++)
	{
		//自解锁
		//lock_guard<mutex> lg(m);
		//m.lock();
		//临界区域-开始
		sum++;
		//临界区域-结束
		//m.unlock();
	}
}

int main(int argc, char* argv[])
{
	std::thread t[tCount];
	for (int n = 0; n < tCount; n++)
	{
		t[n] = std::thread(workFun, n);
	}
	CELLTimestamp tTime;
	for (int n = 0; n < tCount; n++)
	{
		t[n].join();
		//t[n].detach();
	}
	cout << tTime.getElapsedTimeInMilliSec() << ", sum= " << sum << endl;
	tTime.update();
	sum = 0;
	for (int n = 0; n < 80000000; n++)
	{
		sum++;
	}
	cout << tTime.getElapsedTimeInMilliSec() << ", sum= " << sum << endl;
	cout << "Hello, main thread." << endl;
	return 0;
}