
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
		else if (0 == strcmp(cmdBuf, "login"))
		{
			Login login;
			strcpy(login.userName, "lyd");
			strcpy(login.passWord, "lydmm");
			client->SendData(&login);
		}
		else if (0 == strcmp(cmdBuf, "logout"))
		{
			Logout logout;
			strcpy(logout.userName, "lyd");
			client->SendData(&logout);
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

	//EasyTcpClient client2;
	////client.InitSocket();
	//client2.Connect("192.168.71.11", 4567);//MAC OS

	//启动UI线程
	//std::thread t1(cmdThread, &client1);
	//t1.detach();

	//std::thread t2(cmdThread, &client2);
	//t2.detach();

	Login login;
	strcpy(login.userName, "lyd");
	strcpy(login.passWord, "lydmm");
	while (client1.isRun() /*|| client2.isRun()*/)
	{
		client1.OnRun();
		client1.SendData(&login);
		//client2.OnRun();
		//printf("空闲时间处理其他业务...\n");
		
		//Sleep(1000);
	}

	client1.Close();
	//client2.Close();
	printf("客户端退出，任务结束...\n");
	getchar();
	return 0;
}