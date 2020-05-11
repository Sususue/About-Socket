#define _WINSOCK_DEPRECATED_NO_WARNINGS//����gethostbyname��inet_addr����������ǰ��

#include "winsock2.h"
#include <iostream>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

#define THELEN 10
#define MYPORT 2020
#define BUF_SIZE 255

using namespace std;

/*
1.ȷ��������IP��ַ��˿ں�
2.�����׽���
3.���䱾�ض˵��ַ
4.���ӷ����
5.��ѭӦ�ò�Э�����ͨ��
6.�ر�����
*/

/*�����׽��ֲ�����*/
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

	/*����Э�����ָ��*/
	pProtoent = getprotobyname(transport);
	if (pProtoent == NULL) {//ʧ��
		cout << "fail to get protocol " << transport << endl;
	}
	/*ָ��sockect����*/
	if (strcmp(transport, "udp") == 0) {
		type = SOCK_DGRAM;//����udp
	}
	else {
		type = SOCK_STREAM;//����tcp
	}

	/*2.�����׽���*/
	s = socket(PF_INET, type, pProtoent->p_proto);
	if (s == INVALID_SOCKET) {
		cout << "fail to create socket" << endl;
	}
	else {
		cout << "create the socket!" << endl;
	}
	/*3.���䱾�ض˵��ַ��ϵͳ���*/

	/*4.���ӷ����*/
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

/*������Ϣ*/
void receiveM(SOCKET* clientSock) {
	char recvBuf[BUF_SIZE];
	SOCKET newSock;
	SOCKET sock = *(SOCKET*)clientSock;
	while (true) {
		ZeroMemory(recvBuf, BUF_SIZE);
		newSock = recv(sock, recvBuf, BUF_SIZE, 0);
		if (newSock == SOCKET_ERROR) {
			cout << "fail to recv!" << endl;
			closesocket(sock); //�ر��׽���
			WSACleanup();
			exit(0);
		}
		else {
			if (strcmp(recvBuf, "0") == 0) {
				cout << endl << "������ѹر�" << endl;
			}
			else {
				cout << endl << "���յ�����˴��ص���Ϣ��" << recvBuf << endl;
				cout << "-------------------------------------" << endl;
			}

		}
	}


}
/*������Ϣ*/
void sendM(SOCKET* clientSock) {
	char sendBuf[BUF_SIZE];
	int ret;
	SOCKET sock = *(SOCKET*)clientSock;
	cout << "��Ҫ�˳������뵥��0" << endl;
	cout << "-------------------------------------" << endl;
	while (true) {
		ZeroMemory(sendBuf, BUF_SIZE);
		cout << "������Ҫ�����˷��͵���Ϣ��";
		cin >> sendBuf;
		cout << "------------------------------" << endl;
		ret = send(sock, sendBuf, strlen(sendBuf), 0);
		if (ret == SOCKET_ERROR) {
			cout << "fail to send!" << endl;
		}
		if (strcmp(sendBuf, "0") == 0) {//������Ϊ0ʱ���˳�
			cout << "------------------------------" << endl;
			cout << "finished!" << endl;
			closesocket(sock); //�ر��׽���
			WSACleanup();
			exit(0);
		}
	}

}
int main() {
	char host[10] = "localhost";

	WSADATA wsadata;
	//����WSAStartup()��������ʼ��socket�⣬����0��ʾ�ɹ��������ʾһ����ֵ
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		cout << "����winsocket��̬��ʧ��" << endl;
		return 1;
	}

	SOCKET clientSock, newSock;
	clientSock = connectSock(host, MYPORT, "tcp");
	if (clientSock != SOCKET_ERROR) {
		cout << "Connect!" << endl;
		cout << "------------------------------" << endl;
		//�����������߳�
		_beginthread((void(*)(void*))receiveM, 0, &clientSock);
		_beginthread((void(*)(void*))sendM, 0, &clientSock);

		Sleep(INFINITE);	//ʹ������������
	}
	closesocket(clientSock); //�ر��׽���
	WSACleanup(); //�ͷ��׽�����Դ

	return 0;
}