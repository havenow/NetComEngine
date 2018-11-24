
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_NEW_USER_JOIN,
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

struct NewUserJoin : public DataHeader
{
	NewUserJoin()
	{
		dataLength = sizeof(NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		sock = 0;
	}
	int sock;
};

int processor(SOCKET _cSock)
{
	//缓冲区
	char szRecv[4096] = {};
	//5、接收客户端数据
	int nLen = recv(_cSock, szRecv, sizeof(DataHeader), 0);
	DataHeader* header = (DataHeader*)szRecv;
	if (nLen <= 0)
	{
		printf("与服务器断开连接，任务结束...\n");
		return -1;
	}

	switch (header->cmd)
	{
	case CMD_LOGIN_RESULT:
	{
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		LoginResult* loginResult = (LoginResult*)szRecv;
		printf("收到服务端消息: CMD_LOGIN_RESULT，数据长度：%d\n", loginResult->dataLength);
	}
	break;
	case CMD_LOGOUT_RESULT:
	{
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		LogoutResult* logoutResult = (LogoutResult*)szRecv;
		printf("收到服务端消息: CMD_LOGOUT_RESULT，数据长度：%d\n", logoutResult->dataLength);
	}
	break;
	case CMD_NEW_USER_JOIN:
	{
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		NewUserJoin* userJoin = (NewUserJoin*)szRecv;
		printf("收到服务端消息: CMD_NEW_USER_JOIN，数据长度：%d\n", userJoin->dataLength);
	}
	break;
	}
}

int main(int argc, char** argv)
{
	//启动Windows socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	//用socket API建立简易TCP客户端端
	//1、建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock)
	{
		printf("错误，建立socket失败...\n");
	} 
	else
	{
		printf("建立socket成功...\n");
	}
	//2、连接服务器 connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);//host to net unsinged short
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)(&_sin), sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		printf("错误，连接服务器失败...\n");
	}
	else
	{
		printf("连接服务器成功...\n");
	}
	while (true)
	{
		fd_set fdReads;
		FD_ZERO(&fdReads);
		FD_SET(_sock, &fdReads);
		timeval t = { 0, 0 };
		int ret = select(_sock, &fdReads, nullptr, nullptr, &t);
		if (ret < 0)
		{
			printf("select任务结束1...\n");
			break;
		}
		if (FD_ISSET(_sock, &fdReads))
		{
			FD_CLR(_sock, &fdReads);

			if (-1 == processor(_sock))
			{
				printf("select任务结束2...\n");
				break;
			}
		}
		printf("空闲时间处理其他业务...\n");
		Login login;
		strcpy(login.userName, "lyd");
		strcpy(login.passWord, "lydmm");
		send(_sock, (const char*)&login, sizeof(Login), 0);
		//Sleep(1000);
	}
	
	//7、关闭socket closesocket
	closesocket(_sock);

	//清除Windows socket环境
	WSACleanup();
	printf("客户端退出，任务结束...\n");
	getchar();
	return 0;
}