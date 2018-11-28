
#include "EasyTcpServer.hpp"

int main(int argc, char** argv)
{
	EasyTcpServer server;
	//server.InitSocket();
	server.Bind(nullptr, 4567);
	server.Listen(5);

	EasyTcpServer server2;
	//server2.InitSocket();
	server2.Bind(nullptr, 4568);
	server2.Listen(5);

	while (server.isRun() || server2.isRun())
	{
		server.OnRun();
		server2.OnRun();
		//printf("����ʱ�䴦������ҵ��...\n");
	}

	server.Close();
	server2.Close();
	printf("������˳����������...\n");
	getchar();
	return 0;
}