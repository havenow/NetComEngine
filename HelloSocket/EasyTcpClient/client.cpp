
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	//����Windows socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	//��socket API��������TCP�ͻ��˶�
	//1������һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock)
	{
		printf("���󣬽���socketʧ��...\n");
	} 
	else
	{
		printf("����socket�ɹ�...\n");
	}
	//2�����ӷ����� connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);//host to net unsinged short
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)(&_sin), sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		printf("�������ӷ�����ʧ��...\n");
	}
	else
	{
		printf("���ӷ������ɹ�...\n");
	}
	while (true)
	{
		//3��������������
		char cmdBuf[128] = {};
		scanf("%s", cmdBuf);
		//4��������������
		if (0 == strcmp(cmdBuf, "exit"))
		{
			printf("�յ�exit����������...\n");
			break;
		}
		else
		{
			//5�����������������
			send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}

		//6�����շ�������Ϣ recv
		char revBuf[128] = {};
		int nLen = recv(_sock, revBuf, 128, 0);
		if (nLen > 0)
		{
			printf("���յ����� %s...\n", revBuf);
		}
	}
	
	//7���ر�socket closesocket
	closesocket(_sock);

	//���Windows socket����
	WSACleanup();
	printf("�ͻ����˳����������...\n");
	getchar();
	return 0;
}