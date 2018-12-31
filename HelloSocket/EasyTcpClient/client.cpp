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
			printf("�˳�cmdThread�߳�...\n");
			break;
		}
		else
		{
			printf("��֧�ֵ�����...\n");
		}
	}
}


const int cCount = 1000;// FD_SETSIZE - 1; �ͻ�������
const int tCount = 4;//�����߳�����
EasyTcpClient* client[cCount];//�ͻ�������

void sendThread(int id)
{
	//4���߳� ID 1~4
	int c = cCount / tCount;
	int begin = (id - 1) * c;
	int end = id * c;

	for (int n = begin; n < end; n++)
	{
		client[n] = new EasyTcpClient();
	}
	for (int n = begin; n < end; n++)
	{
		client[n]->Connect("127.0.0.1", 4567);
		printf("Connect=%d\n", n);
	}

	Login login;
	strcpy(login.userName, "lyd");
	strcpy(login.passWord, "lydmm");
	while (g_bRun)
	{
		for (int n = begin; n < end; n++)
		{
			client[n]->SendData(&login);
			//client[n]->OnRun();
		}
	}

	for (int n = begin; n < end; n++)
	{
		client[n]->Close();
		delete client[n];
	}
}

int main(int argc, char** argv)
{
	//����UI�߳�
	std::thread t1(cmdThread);
	t1.detach();

	//���������߳�
	for (int n = 0; n < tCount; n++)
	{
		std::thread t1(sendThread, n+1);
		t1.detach();
	}

	while (g_bRun)
		Sleep(100);
	printf("�ͻ����˳����������...\n");
	return 0;
}