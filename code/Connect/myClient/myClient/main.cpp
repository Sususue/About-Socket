#define _WINSOCK_DEPRECATED_NO_WARNINGS//消除gethostbyname和inet_addr报错，放在最前面

#include "winsock2.h"
#include <iostream>
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

	char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];//缓存信息
	int ret;
	
	while (clientSock!= SOCKET_ERROR) {
		ZeroMemory(sendBuf, BUF_SIZE);
		cout << "若要退出请输入单个0" << endl;
		cout << "-------------------------------------" << endl;
		cout << "请输入要向服务端发送的信息：";
		cin >> sendBuf;
		ret = send(clientSock, sendBuf, strlen(sendBuf), 0);
		if (ret == SOCKET_ERROR) {
			cout << "fail to send!" << endl;
			closesocket(clientSock);//关闭套接字
			WSACleanup();			//释放套接字资源;
			return 1;
		}
		if (strcmp(sendBuf, "0") == 0) {
			cout << "------------------------------" << endl;
			cout << "finished!" << endl;
			break;
		}
		ZeroMemory(recvBuf, BUF_SIZE);
		newSock = recv(clientSock, recvBuf, BUF_SIZE, 0);
		if (newSock == SOCKET_ERROR) {
			cout << "fail to recv!" << endl;
			closesocket(clientSock);//关闭套接字
			WSACleanup();			//释放套接字资源;
			return 1;
		}
		cout << "接收到服务端传回的信息：" << recvBuf << endl;
		cout << "-------------------------------------" << endl;
	}
	closesocket(clientSock); //关闭套接字
	WSACleanup(); //释放套接字资源

	return 0;
}