
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

struct DataPackage
{
	int age;
	char name[32];
};

int main(int argc, char** argv)
{
	//����Windows socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);


	//��socket API��������TCP�����
	//1������һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _sock)
	{
		printf("���󣬽���socketʧ��...\n");
	}
	else
	{
		printf("����socket�ɹ�...\n");
	}
	//2���󶨽��ܿͻ������ӵĶ˿� bind
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);//host to net unsinged short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;//inet_addr("127.0.0.1");
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in)))
	{
		printf("���󣬰�����˿�ʧ��...\n");
	}
	else
	{
		printf("������˿ڳɹ�...\n");
	}
	//3����������˿� listen
	if (SOCKET_ERROR == listen(_sock, 5))
	{
		printf("���󣬼�������˿�ʧ��...\n");
	} 
	else
	{
		printf("��������˿ڳɹ�...\n");
	}
	//4���ȴ����ܿͻ������� accept
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	//char msgBuf[] = "Hello, I'm Server.";

	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock)
	{
		printf("���󣬽��ܵ���Ч�ͻ���SOCKET...\n");
	}
	printf("�¿ͻ��˼��룺socket = %d, IP = %s\n", (int)_cSock, inet_ntoa(clientAddr.sin_addr));

	char _recvBuf[128] = {};
	while (true)
	{
		//5�����տͻ�������
		int nLen = recv(_cSock, _recvBuf, 128, 0);
		if (nLen <=0)
		{
			printf("�ͻ������˳����������...\n");
			break;
		}
		printf("�յ����%s\n", _recvBuf);
		//6����������
		if (0 == strcmp(_recvBuf, "getInfo"))
		{
			DataPackage dp = {80, "С��"};
			//7����ͻ��˷���һ������ send
			send(_cSock, (const char*)&dp, sizeof(DataPackage), 0);
		}
		else
		{
			char msgBuf[] = "???.";
			//7����ͻ��˷���һ������ send
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
	}
	//	8���ر�socket closesocket
	closesocket(_sock);


	//���Windows socket����
	WSACleanup();
	printf("������˳����������...\n");
	getchar();
	return 0;
}