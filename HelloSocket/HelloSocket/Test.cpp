
#define  WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>

int main(int argc, char** argv)
{
	//����Windows socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	///
	/*--------------------------------------------
	��socket API��������TCP�����
		1������һ��socket
		2���󶨽��ܿͻ������ӵĶ˿� bind
		3����������˿� listen
		4���ȴ����ܿͻ������� accept
		5����ͻ��˷���һ������ send
		6���ر�socket closesocket
	*/

	/*--------------------------------------------
	��socket API��������TCP�ͻ��˶�
		1������һ��socket
		2�����ӷ����� connect
		3�����շ�������Ϣ recv
		4���ر�socket closesocket
	*/



	///
	//���Windows socket����
	WSACleanup();
	return 0;
}