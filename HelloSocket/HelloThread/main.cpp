#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m;
void workFun(int index)
{
	for (int n = 0; n < 40; n++)
	{
		m.lock();
		//临界区域-开始
		cout << index << "Hello, other thread." << n << endl;
		//临界区域-结束
		m.unlock();
	}
}

int main(int argc, char* argv[])
{
	std::thread t[4];
	for (int n = 0; n < 4; n++)
	{
		t[n] = std::thread(workFun, n);
	}
	for (int n = 0; n < 4; n++)
	{
		t[n].join();
		//t[n].detach();
	}
	for (int n = 0; n < 4; n++)
		cout << "Hello, main thread." << endl;
	while (true)
	{
	}
	return 0;
}