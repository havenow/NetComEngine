
#define  WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>

int main(int argc, char** argv)
{
	//启动Windows socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	///
	/*--------------------------------------------
	用socket API建立简易TCP服务端
		1、建立一个socket
		2、绑定接受客户端连接的端口 bind
		3、监听网络端口 listen
		4、等待接受客户端连接 accept
		5、向客户端发送一条数据 send
		6、关闭socket closesocket
	*/

	/*--------------------------------------------
	用socket API建立简易TCP客户端端
		1、建立一个socket
		2、连接服务器 connect
		3、接收服务器信息 recv
		4、关闭socket closesocket
	*/



	///
	//清除Windows socket环境
	WSACleanup();
	return 0;
}