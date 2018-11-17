
#define  WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>

int main(int argc, char** argv)
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	///


	///

	WSACleanup();
	return 0;
}