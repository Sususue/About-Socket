#include<WinSock2.h>
#include<iostream>
#include<string.h>

#pragma comment(lib, "ws2_32.lib")//添加动态链接库？

#define THELEN 10
#define MYPORT 2020
#define BUF_SIZE 255
using namespace std;
/*
1.建立套接字
2.绑定端口号
3.设置为被动监听模式
4.用accept接收连接请求，建立连接
5.反复接受用户请求，构造并发送响应（注意每次都要创建新的套接字）
6.完成后关闭连接，返回3或者结束
*/


SOCKET toSock(const int port, const char* transport, int theLength) {
	//struct servent* pServent;
	struct protoent* pProtoent;
	struct sockaddr_in sin;
	SOCKET s;
	int type;//套接字的类型

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;//IPv4
	sin.sin_addr.s_addr = INADDR_ANY;//不要用绝对地址
	sin.sin_port = htons(port);

	/*返回指定服务的指针*/
	//pServent = getservbyport(htons(port),transport);//port用网络字节序
	//if (pServent==NULL){
	//	cout << "fail to get service " << port << endl;
	//}

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

	/*1.创建套接字*/
	s = socket(PF_INET, type, pProtoent->p_proto);
	if (s == INVALID_SOCKET) {
		cout << "fail to create socket" << endl;
	}
	else {
		cout << "create the socket!" << endl;
	}
	/*2.绑定套接字端点地址*/
	if (bind(s, (struct sockaddr*) & sin, sizeof(sin)) == SOCKET_ERROR) {
		cout << "fail to bind " << endl;
	}
	else {
		cout << "bind succeed!" << endl;
	}
	/*3.设置为被动监听模式*/
	if (type == SOCK_STREAM && listen(s, theLength) == SOCKET_ERROR) {
		cout << "fail to listen on the port " << port << endl;
	}
	else {
		cout << "Set listen!" << endl;
	}

	return s;

}



int main() {
	struct sockaddr_in clientSin;
	WSADATA wsadata;
	//调用WSAStartup()函数来初始化socket库，返回0表示成功，否则表示一个常值
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		cout << "加载winsocket动态库失败" << endl;
		return 1;
	}
	SOCKET mySock, newSock;
	mySock = toSock(MYPORT, "tcp", THELEN);//创建套接字并设置为监听状态
	int sockaddrLen = sizeof(sockaddr);
	/*4.用accept接收连接请求，建立连接*/
	newSock = accept(mySock, (struct sockaddr*) & clientSin, &sockaddrLen);
	if (newSock == INVALID_SOCKET) {
		cout << "fail to accept!" << endl;
		closesocket(mySock);	//关闭套接字		
		WSACleanup();
		return 1;
	}
	else {
		cout << "accept!" << endl;
		char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];//缓存信息

		int ret;
		/*5.反复接受用户请求，构造并发送响应（注意每次都要创建新的套接字）*/
		while (true)
		{
			ZeroMemory(recvBuf, BUF_SIZE);//清空缓存
			ZeroMemory(sendBuf, BUF_SIZE);
			//接收客户端数据
			ret = recv(newSock, recvBuf, BUF_SIZE, 0);

			if (ret == SOCKET_ERROR)
			{
				cout << "fail to recv!" << endl;
				closesocket(mySock);	//关闭套接字
				closesocket(newSock);	//关闭套接字		
				WSACleanup();			//释放套接字资源;
				return 1;
			}
			if (recvBuf[0] == '0' && strlen(recvBuf) == 1) {//若接收到一个0，退出循环，关闭连接
				cout << "finished!" << endl;
				break;
			}
			cout << "客户端发送了信息: " << recvBuf << endl;

			strcpy_s(sendBuf, "Server:");
			strcat_s(sendBuf, recvBuf);
			cout << "向客户端返回信息: " << sendBuf << endl;
			cout << "------------------------------" << endl;

			send(newSock, sendBuf, strlen(sendBuf), 0);//将修改后的信息发送给客户端


		}
		/*6.完成后关闭连接，返回3或者结束*/

	}

	//退出
	closesocket(mySock);	//关闭套接字
	closesocket(newSock);	//关闭套接字
	WSACleanup();
	return 0;
}