#ifndef _EasyTcpClient_hpp_
#define _EasyTcpClient_hpp_

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
	#include <windows.h>
	#include <WinSock2.h>
#else
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <string.h>

	#define SOCKET int
	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR            (-1)
#endif

#include <stdio.h>
#include "MessageHeader.hpp"

class EasyTcpClient
{
public:
	EasyTcpClient()
	{
		_sock = INVALID_SOCKET;
	}

	virtual ~EasyTcpClient()
	{
		Close();
#ifdef _WIN32
		//清除Windows socket环境
		WSACleanup();
#endif
	}

	//初始化socket
	void InitSocket()
	{
#ifdef _WIN32
		//启动Windows socket 2.x环境
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		WSAStartup(ver, &dat);
#endif
		//1、建立一个socket
		if (INVALID_SOCKET != _sock)
		{
			printf("<socket=%d>关闭旧连接...\n", _sock);
			Close();
		}
		_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == _sock)
		{
			printf("错误，建立socket失败...\n");
		}
		else
		{
			//printf("建立socket=<%d>成功...\n", _sock);
		}
	}

	//连接服务器
	int Connect(const char* ip, unsigned short port)
	{
		if (INVALID_SOCKET == _sock)
		{
			InitSocket();
		}
		//2、连接服务器 connect
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);//host to net unsinged short
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
		_sin.sin_addr.s_addr = inet_addr(ip);//服务器的ip地址；如果客户端在虚拟机中运行的，注意ip地址
#endif
		//printf("<socket=%d>正在连接服务器<%s : %d>...\n", _sock, ip, port);
		int ret = connect(_sock, (sockaddr*)(&_sin), sizeof(sockaddr_in));
		if (SOCKET_ERROR == ret)
		{
			printf("<socket=%d>错误，连接服务器<%s : %d>失败...\n", _sock, ip, port);
		}
		else
		{
			//printf("<socket=%d>连接服务器<%s : %d>成功...\n", _sock, ip, port);
		}
		return ret;
	}

	//关闭socket
	void Close()
	{
		if (_sock != INVALID_SOCKET)
		{
#ifdef _WIN32
			//关闭socket closesocket
			closesocket(_sock);
			//清除Windows socket环境
			//WSACleanup();
#else
			close(_sock);
#endif
			_sock = INVALID_SOCKET;
		}
	}

	bool isRun()
	{
		return _sock != INVALID_SOCKET;
	}

	//处理网络消息
	int _nCount = 0;
	bool OnRun()
	{
		if (isRun())
		{
			fd_set fdReads;
			FD_ZERO(&fdReads);
			FD_SET(_sock, &fdReads);
			timeval t = { 0, 0 };
			int ret = select(_sock + 1, &fdReads, nullptr, nullptr, &t);
			//printf("select ret=%d count=%d\n", ret, _nCount++);
			if (ret < 0)
			{
				printf("<socket=%d>select任务结束1...\n", _sock);
				Close();
				return false;
			}
			if (FD_ISSET(_sock, &fdReads))
			{
				FD_CLR(_sock, &fdReads);

				if (-1 == RecvData(_sock))
				{
					printf("<socket=%d>select任务结束2...\n", _sock);
					Close();
					return false;
				}
			}
			return true;
		}
		return false;
	}

	//缓冲区最小单元
#ifndef RECV_BUFF_SIZE
	#define RECV_BUFF_SIZE 10240
#endif
	//接收缓冲区
	char _szRecv[RECV_BUFF_SIZE] = {};
	//第二缓冲区 消息缓冲区
	char _szMsgBuf[RECV_BUFF_SIZE * 10] = {};
	int _lastPos = 0;
	//接收数据 处理粘包 拆包
	int RecvData(SOCKET cSock)
	{
		//5、接收数据
		int nLen = (int)recv(cSock, _szRecv, RECV_BUFF_SIZE, 0);
		//printf("nLen=%d\n", nLen);
		if (nLen <= 0)
		{
			printf("<socket=%d>与服务器断开连接，任务结束...\n", cSock);
			return -1;
		}
		//将收取到数据拷贝到消息缓冲区
		memcpy(_szMsgBuf + _lastPos, _szRecv, nLen);
		//消息缓冲区的数据尾部位置后移
		_lastPos += nLen;
		//判断消息缓冲区的数据长度大于消息头DataHeader长度
		while (_lastPos >= sizeof(DataHeader))
		{
			//这时就可以知道当前消息的长度
			DataHeader* header = (DataHeader*)_szMsgBuf;
			//判断消息缓冲区的数据长度大于消息长度
			if (_lastPos >= header->dataLength)
			{
				//消息缓冲区剩余未处理数据的长度
				int nSize = _lastPos - header->dataLength;
				//处理网络消息
				OnNetMsg(header);
				//将消息缓冲区剩余未处理数据前移
				memcpy(_szMsgBuf, _szMsgBuf + header->dataLength, nSize);
				//消息缓冲区的数据尾部位置前移
				_lastPos = nSize;
			}
			else
			{
				//息缓冲区剩余数据不够一条完整消息
				break;
			}
		}

		/*DataHeader* header = (DataHeader*)szRecv;
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		OnNetMsg(header);*/
		return 0;
	}

	//响应网络消息
	virtual void OnNetMsg(DataHeader* header)
	{
		switch (header->cmd)
		{
		case CMD_LOGIN_RESULT:
		{
			LoginResult* loginResult = (LoginResult*)header;
			//printf("<socket=%d>收到服务端消息: CMD_LOGIN_RESULT，数据长度：%d\n", _sock, loginResult->dataLength);
		}
		break;
		case CMD_LOGOUT_RESULT:
		{
			LogoutResult* logoutResult = (LogoutResult*)header;
			//printf("<socket=%d>收到服务端消息: CMD_LOGOUT_RESULT，数据长度：%d\n", _sock, logoutResult->dataLength);
		}
		break;
		case CMD_NEW_USER_JOIN:
		{
			NewUserJoin* userJoin = (NewUserJoin*)header;
			//printf("<socket=%d>收到服务端消息: CMD_NEW_USER_JOIN，数据长度：%d\n", _sock, userJoin->dataLength);
		}
		break;
		case CMD_ERROR:
		{
			printf("<socket=%d>收到服务端消息: CMD_ERROR，数据长度：%d\n", _sock, header->dataLength);
		}
		break;
		default:
		{
			printf("<socket=%d>收到未定义消息，数据长度：%d\n", _sock, header->dataLength);
		}
		break;
		}
	}

	//发送数据
	int SendData(DataHeader* header)
	{
		if (isRun() && header)
		{
			return send(_sock, (const char*)header, header->dataLength, 0);
		}
		return SOCKET_ERROR;
	}

private:
	SOCKET _sock;
};

#endif