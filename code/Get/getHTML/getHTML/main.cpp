#include"htmlGet.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS//消除gethostbyname和inet_addr报错
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>
#include "htmlGet.h"


#pragma comment(lib, "ws2_32.lib")

using namespace std;



int main() {
	WSADATA wsadata;
	//调用WSAStartup()函数来初始化socket库，返回0表示成功，否则表示一个常值
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		cout << "加载winsocket动态库失败" << endl;
		return 1;
	}
	string host, resource;
	const string url = "http://qiming.hust.edu.cn";//确保url输入正确

	//const string url = "http://www.icourse163.org/university/HUST";
	if (parseURL(url, host, resource) == false) {
		return 1;
	}

	SOCKET clientSock;
	clientSock = connectSock(host.c_str(), 80, "tcp");
	if (clientSock == SOCKET_ERROR) {
		cout << "fail to connectSock" << endl;
	}
	else if (sendRequest(clientSock, host, resource) == true) {
		string recvStr = recvRequest(clientSock, url);
		int pos = 0;
		pos = recvStr.find('<', 0);
		if (pos!= string::npos) {
			recvStr = recvStr.substr(pos, recvStr.length()-pos);
			downLoad(recvStr);
		}
		cout << url << " finished" << endl;
	}
	closesocket(clientSock); //关闭套接字
	WSACleanup(); //释放套接字资源

	return 0;
}