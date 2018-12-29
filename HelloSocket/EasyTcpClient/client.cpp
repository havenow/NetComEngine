#include "EasyTcpClient.hpp"
#include <thread>

bool g_bRun = true;
void cmdThread()
{
	while (true)
	{
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit"))
		{
			g_bRun = false;
			printf("退出cmdThread线程...\n");
			break;
		}
		else
		{
			printf("不支持的命令...\n");
		}
	}
}

int main(int argc, char** argv)
{
	const int cCount = 10000;// FD_SETSIZE - 1;
	EasyTcpClient* client[cCount];

	for (int n = 0; n < cCount; n++)
	{
		if (!g_bRun)
		{
			return 0;
		}
		client[n] = new EasyTcpClient();
	}
	for (int n = 0; n < cCount; n++)
	{
		if (!g_bRun)
		{
			return 0;
		}
		client[n]->Connect("127.0.0.1", 4567);
		printf("Connect=%d\n", n);
	}

	//启动UI线程
	std::thread t1(cmdThread);
	t1.detach();

	Login login;
	strcpy(login.userName, "lyd");
	strcpy(login.passWord, "lydmm");
	while (g_bRun)
	{
		for (int n = 0; n < cCount; n++)
		{
			client[n]->SendData(&login);
			//client[n]->OnRun();
		}
	}

	for (int n = 0; n < cCount; n++)
	{
		client[n]->Close();
		delete client[n];
	}

	printf("客户端退出，任务结束...\n");
	return 0;
}