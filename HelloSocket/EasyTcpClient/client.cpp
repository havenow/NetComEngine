
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

enum CMD
{
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_ERROR
};

struct DataHeader
{
	short dataLength;
	short cmd;
};

//DataPackage
struct Login
{
	char userName[32];
	char passWord[32];
};

struct LoginResult
{
	int result;
};

struct Logout
{
	char userName[32];
};

struct LogoutResult
{
	int result;
};

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
		else if (0 == strcmp(cmdBuf, "login"))
		{
			Login login = { "lyd", "lydmm" };
			DataHeader dh = { sizeof(login), CMD_LOGIN};
			//5�����������������
			send(_sock, (const char*)&dh, sizeof(dh), 0);
			send(_sock, (const char*)&login, sizeof(login), 0);
			//���շ��������ص�����
			DataHeader retHeader = {};
			LoginResult loginRet = {};
			recv(_sock, (char*)&retHeader, sizeof(retHeader),0);
			recv(_sock, (char*)&loginRet, sizeof(loginRet), 0);
			printf("LoginResult %d\n", loginRet.result);
		}
		else if (0 == strcmp(cmdBuf, "logout"))
		{
			Logout logout = {"lyd" };
			//5�����������������
			DataHeader dh = { sizeof(logout), CMD_LOGOUT };
			send(_sock, (const char*)&dh, sizeof(dh), 0);
			send(_sock, (const char*)&logout, sizeof(logout), 0);
			//���շ��������ص�����
			DataHeader retHeader = {};
			LogoutResult logoutRet = {};
			recv(_sock, (char*)&retHeader, sizeof(retHeader), 0);
			recv(_sock, (char*)&logoutRet, sizeof(logoutRet), 0);
			printf("LogoutResult %d\n", logoutRet.result);
		}
		else
		{
			printf("�յ���֧�ֵ��������������...\n");
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