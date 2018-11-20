
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};

struct DataHeader
{
	short dataLength;
	short cmd;
};

//DataPackage
struct Login : public DataHeader
{
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};

struct LoginResult : public DataHeader
{
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};

struct Logout : public DataHeader
{
	Logout()
	{
		dataLength = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};

struct LogoutResult : public DataHeader
{
	LogoutResult()
	{
		dataLength = sizeof(LogoutResult);
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
	int result;
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
		DataHeader header = {};
		//5�����տͻ�������
		int nLen = recv(_cSock, (char*)&header, sizeof(DataHeader), 0);
		if (nLen <=0)
		{
			printf("�ͻ������˳����������...\n");
			break;
		}
		
		switch (header.cmd)
		{
		case CMD_LOGIN:
		{
			Login login = {};
			recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
			printf("�յ����CMD_LOGIN�����ݳ��ȣ�%d, userName=%s, PassWord=%s\n", login.dataLength, login.userName, login.passWord);
			//�����ж��û��������Ƿ���ȷ�Ĺ���

			LoginResult ret;
			send(_cSock, (char*)&ret, sizeof(LoginResult), 0);
		}
			break;
		case CMD_LOGOUT:
		{
			Logout logout = {};
			recv(_cSock, (char*)&logout + sizeof(DataHeader), sizeof(Logout) - sizeof(DataHeader), 0);
			printf("�յ����CMD_LOGOUT�����ݳ��ȣ�%d, userName=%s\n", logout.dataLength, logout.userName);
			//�����ж��û��������Ƿ���ȷ�Ĺ���

			LogoutResult ret;
			send(_cSock, (char*)&ret, sizeof(LogoutResult), 0);
		}
			break;
		default:
		{
			header.cmd = CMD_ERROR;
			header.dataLength = 0;
			send(_cSock, (char*)&header, sizeof(DataHeader), 0);
		}
			break;
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