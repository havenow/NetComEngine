#include <iostream>
#include <thread>
using namespace std;

void workFun(int index)
{
	for (int n = 0; n < 4; n++)
		cout << index << "Hello, other thread." << endl;
}

int main(int argc, char* argv[])
{
	std::thread t[3];
	for (int n = 0; n < 3; n++)
	{
		t[n] = std::thread(workFun, n);
	}
	for (int n = 0; n < 3; n++)
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