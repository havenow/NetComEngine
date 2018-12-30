#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>//ԭ��
#include "CELLTimestamp.hpp"
using namespace std;
//ԭ�Ӳ���
mutex m;
const int tCount = 4;
atomic<int> sum = 0;//Ч�ʱȼ�����
void workFun(int index)
{
	for (int n = 0; n < 20000000; n++)
	{
		//�Խ���
		//lock_guard<mutex> lg(m);
		//m.lock();
		//�ٽ�����-��ʼ
		sum++;
		//�ٽ�����-����
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