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
		//���Windows socket����
		WSACleanup();
#endif
	}

	//��ʼ��socket
	void InitSocket()
	{
#ifdef _WIN32
		//����Windows socket 2.x����
		WORD ver = MAKEWORD(2, 2);
		WSADATA dat;
		WSAStartup(ver, &dat);
#endif
		//1������һ��socket
		if (INVALID_SOCKET != _sock)
		{
			printf("<socket=%d>�رվ�����...\n", _sock);
			Close();
		}
		_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == _sock)
		{
			printf("���󣬽���socketʧ��...\n");
		}
		else
		{
			printf("����socket=<%d>�ɹ�...\n", _sock);
		}
	}

	//���ӷ�����
	int Connect(const char* ip, unsigned short port)
	{
		if (INVALID_SOCKET == _sock)
		{
			InitSocket();
		}
		//2�����ӷ����� connect
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);//host to net unsinged short
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
		_sin.sin_addr.s_addr = inet_addr(ip);//��������ip��ַ������ͻ���������������еģ�ע��ip��ַ
#endif
		printf("<socket=%d>�������ӷ�����<%s : %d>...\n", _sock, ip, port);
		int ret = connect(_sock, (sockaddr*)(&_sin), sizeof(sockaddr_in));
		if (SOCKET_ERROR == ret)
		{
			printf("<socket=%d>�������ӷ�����<%s : %d>ʧ��...\n", _sock, ip, port);
		}
		else
		{
			printf("<socket=%d>���ӷ�����<%s : %d>�ɹ�...\n", _sock, ip, port);
		}
		return ret;
	}

	//�ر�socket
	void Close()
	{
		if (_sock != INVALID_SOCKET)
		{
#ifdef _WIN32
			//�ر�socket closesocket
			closesocket(_sock);
			//���Windows socket����
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

	//����������Ϣ
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
				printf("<socket=%d>select�������1...\n", _sock);
				Close();
				return false;
			}
			if (FD_ISSET(_sock, &fdReads))
			{
				FD_CLR(_sock, &fdReads);

				if (-1 == RecvData(_sock))
				{
					printf("<socket=%d>select�������2...\n", _sock);
					Close();
					return false;
				}
			}
			return true;
		}
		return false;
	}

	//��������С��Ԫ
#ifndef RECV_BUFF_SIZE
	#define RECV_BUFF_SIZE 10240
#endif
	//���ջ�����
	char _szRecv[RECV_BUFF_SIZE] = {};
	//�ڶ������� ��Ϣ������
	char _szMsgBuf[RECV_BUFF_SIZE * 10] = {};
	int _lastPos = 0;
	//�������� ����ճ�� ���
	int RecvData(SOCKET cSock)
	{
		//5����������
		int nLen = (int)recv(cSock, _szRecv, RECV_BUFF_SIZE, 0);
		//printf("nLen=%d\n", nLen);
		if (nLen <= 0)
		{
			printf("<socket=%d>��������Ͽ����ӣ��������...\n", cSock);
			return -1;
		}
		//����ȡ�����ݿ�������Ϣ������
		memcpy(_szMsgBuf + _lastPos, _szRecv, nLen);
		//��Ϣ������������β��λ�ú���
		_lastPos += nLen;
		//�ж���Ϣ�����������ݳ��ȴ�����ϢͷDataHeader����
		while (_lastPos >= sizeof(DataHeader))
		{
			//��ʱ�Ϳ���֪����ǰ��Ϣ�ĳ���
			DataHeader* header = (DataHeader*)_szMsgBuf;
			//�ж���Ϣ�����������ݳ��ȴ�����Ϣ����
			if (_lastPos >= header->dataLength)
			{
				//��Ϣ������ʣ��δ�������ݵĳ���
				int nSize = _lastPos - header->dataLength;
				//����������Ϣ
				OnNetMsg(header);
				//����Ϣ������ʣ��δ��������ǰ��
				memcpy(_szMsgBuf, _szMsgBuf + header->dataLength, nSize);
				//��Ϣ������������β��λ��ǰ��
				_lastPos = nSize;
			}
			else
			{
				//Ϣ������ʣ�����ݲ���һ��������Ϣ
				break;
			}
		}

		/*DataHeader* header = (DataHeader*)szRecv;
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		OnNetMsg(header);*/
		return 0;
	}

	//��Ӧ������Ϣ
	virtual void OnNetMsg(DataHeader* header)
	{
		switch (header->cmd)
		{
		case CMD_LOGIN_RESULT:
		{
			LoginResult* loginResult = (LoginResult*)header;
			//printf("<socket=%d>�յ��������Ϣ: CMD_LOGIN_RESULT�����ݳ��ȣ�%d\n", _sock, loginResult->dataLength);
		}
		break;
		case CMD_LOGOUT_RESULT:
		{
			LogoutResult* logoutResult = (LogoutResult*)header;
			//printf("<socket=%d>�յ��������Ϣ: CMD_LOGOUT_RESULT�����ݳ��ȣ�%d\n", _sock, logoutResult->dataLength);
		}
		break;
		case CMD_NEW_USER_JOIN:
		{
			NewUserJoin* userJoin = (NewUserJoin*)header;
			//printf("<socket=%d>�յ��������Ϣ: CMD_NEW_USER_JOIN�����ݳ��ȣ�%d\n", _sock, userJoin->dataLength);
		}
		break;
		case CMD_ERROR:
		{
			printf("<socket=%d>�յ��������Ϣ: CMD_ERROR�����ݳ��ȣ�%d\n", _sock, header->dataLength);
		}
		break;
		default:
		{
			printf("<socket=%d>�յ�δ������Ϣ�����ݳ��ȣ�%d\n", _sock, header->dataLength);
		}
		break;
		}
	}

	//��������
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