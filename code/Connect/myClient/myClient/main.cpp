#define _WINSOCK_DEPRECATED_NO_WARNINGS//消除gethostbyname和inet_addr报错，放在最前面

#include "winsock2.h"
#include <iostream>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

#define THELEN 10
#define MYPORT 2020
#define BUF_SIZE 255

using namespace std;

/*
1.确定服务器IP地址与端口号
2.创建套接字
3.分配本地端点地址
4.连接服务端
5.遵循应用层协议进行通信
6.关闭连接
*/

/*创建套接字并连接*/
SOCKET connectSock(const char* host, const int port, const char* transport) {
	struct hostent* pHostent;
	//struct servent* pServent;
	struct protoent* pProtoent;
	struct sockaddr_in sin;
	SOCKET s;
	int type;

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	if (pHostent = gethostbyname(host)) {
		memcpy(&sin.sin_addr, pHostent->h_addr, pHostent->h_length);

	}
	else if (sin.sin_addr.s_addr = inet_addr(host) == INADDR_NONE) {
		cout << "fail to inet_addr" << endl;
	}

	/*返回协议相关指针*/
	pProtoent = getprotobyname(transport);
	if (pProtoent == NULL) {//失败
		cout << "fail to get protocol " << transport << endl;
	}
	/*指定sockect类型*/
	if (strcmp(transport, "udp") == 0) {
		type = SOCK_DGRAM;//采用udp
	}
	else {
		type = SOCK_STREAM;//采用tcp
	}

	/*2.创建套接字*/
	s = socket(PF_INET, type, pProtoent->p_proto);
	if (s == INVALID_SOCKET) {
		cout << "fail to create socket" << endl;
	}
	else {
		cout << "create the socket!" << endl;
	}
	/*3.分配本地端点地址，系统完成*/

	/*4.连接服务端*/
	if (connect(s, (struct sockaddr*) & sin, sizeof(sin)) == SOCKET_ERROR) {
		cout << "fail to connect" << endl;
		return SOCKET_ERROR;
	}
	else {
		cout << "Connect!" << endl;
		cout << "------------------------------" << endl;
		
	}

	return s;

}

/*接收信息*/
void receiveM(SOCKET* clientSock) {
	char recvBuf[BUF_SIZE];
	SOCKET newSock;
	SOCKET sock = *(SOCKET*)clientSock;
	while (true) {
		ZeroMemory(recvBuf, BUF_SIZE);
		newSock = recv(sock, recvBuf, BUF_SIZE, 0);
		if (newSock == SOCKET_ERROR) {
			cout << "fail to recv!" << endl;
			closesocket(sock); //关闭套接字
			WSACleanup();
			exit(0);
		}
		else {
			if (strcmp(recvBuf, "0") == 0) {
				cout << endl << "服务端已关闭" << endl;
			}
			else {
				cout << endl << "接收到服务端传回的信息：" << recvBuf << endl;
				cout << "-------------------------------------" << endl;
			}

		}
	}


}
/*发送信息*/
void sendM(SOCKET* clientSock) {
	char sendBuf[BUF_SIZE];
	int ret;
	SOCKET sock = *(SOCKET*)clientSock;
	cout << "若要退出请输入单个0" << endl;
	cout << "-------------------------------------" << endl;
	while (true) {
		ZeroMemory(sendBuf, BUF_SIZE);
		cout << "请输入要向服务端发送的信息：";
		cin >> sendBuf;
		cout << "------------------------------" << endl;
		ret = send(sock, sendBuf, strlen(sendBuf), 0);
		if (ret == SOCKET_ERROR) {
			cout << "fail to send!" << endl;
		}
		if (strcmp(sendBuf, "0") == 0) {//当输入为0时，退出
			cout << "------------------------------" << endl;
			cout << "finished!" << endl;
			closesocket(sock); //关闭套接字
			WSACleanup();
			exit(0);
		}
	}

}
int main() {
	char host[10] = "localhost";

	WSADATA wsadata;
	//调用WSAStartup()函数来初始化socket库，返回0表示成功，否则表示一个常值
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		cout << "加载winsocket动态库失败" << endl;
		return 1;
	}

	SOCKET clientSock, newSock;
	clientSock = connectSock(host, MYPORT, "tcp");
	if (clientSock != SOCKET_ERROR) {
		cout << "Connect!" << endl;
		cout << "------------------------------" << endl;
		//创建两个子线程
		_beginthread((void(*)(void*))receiveM, 0, &clientSock);
		_beginthread((void(*)(void*))sendM, 0, &clientSock);

		Sleep(INFINITE);	//使其无限期休眠
	}
	closesocket(clientSock); //关闭套接字
	WSACleanup(); //释放套接字资源

	return 0;
}