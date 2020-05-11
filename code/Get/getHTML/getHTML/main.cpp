#include"htmlGet.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS//����gethostbyname��inet_addr����
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>
#include "htmlGet.h"


#pragma comment(lib, "ws2_32.lib")

using namespace std;



int main() {
	WSADATA wsadata;
	//����WSAStartup()��������ʼ��socket�⣬����0��ʾ�ɹ��������ʾһ����ֵ
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		cout << "����winsocket��̬��ʧ��" << endl;
		return 1;
	}
	string host, resource;
	const string url = "http://qiming.hust.edu.cn";//ȷ��url������ȷ

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
	closesocket(clientSock); //�ر��׽���
	WSACleanup(); //�ͷ��׽�����Դ

	return 0;
}