
#include "EasyTcpClient.hpp"
#include <thread>

void cmdThread(EasyTcpClient* client)
{
	while (true)
	{
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit"))
		{
			client->Close();
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
	EasyTcpClient client1;
	//client1.InitSocket();
	client1.Connect("127.0.0.1", 4567);


	//启动UI线程
	std::thread t1(cmdThread, &client1);
	t1.detach();

	Login login;
	strcpy(login.userName, "lyd");
	strcpy(login.passWord, "lydmm");
	while (client1.isRun())
	{
		client1.OnRun();
		client1.SendData(&login);
	}

	client1.Close();
	printf("客户端退出，任务结束...\n");
	getchar();
	return 0;
}