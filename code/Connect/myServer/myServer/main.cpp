#include<WinSock2.h>
#include<iostream>
#include<string.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")//��Ӷ�̬���ӿ⣿

#define THELEN 10
#define MYPORT 2020
#define BUF_SIZE 255

using namespace std;
/*
1.�����׽���
2.�󶨶˿ں�
3.����Ϊ��������ģʽ
4.��accept�����������󣬽�������
5.���������û����󣬹��첢������Ӧ��ע��ÿ�ζ�Ҫ�����µ��׽��֣�
6.��ɺ�ر����ӣ�����3���߽���
*/


SOCKET toSock(const int port, const char* transport, int theLength) {
	//struct servent* pServent;
	struct protoent* pProtoent;
	struct sockaddr_in sin;
	SOCKET s;
	int type;//�׽��ֵ�����

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;//IPv4
	sin.sin_addr.s_addr = INADDR_ANY;//��Ҫ�þ��Ե�ַ
	sin.sin_port = htons(port);

	/*����ָ�������ָ��*/
	//pServent = getservbyport(htons(port),transport);//port�������ֽ���
	//if (pServent==NULL){
	//	cout << "fail to get service " << port << endl;
	//}

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

	/*1.�����׽���*/
	s = socket(PF_INET, type, pProtoent->p_proto);
	if (s == INVALID_SOCKET) {
		cout << "fail to create socket" << endl;
	}
	else {
		cout << "create the socket!" << endl;
	}
	/*2.���׽��ֶ˵��ַ*/
	if (bind(s, (struct sockaddr*) & sin, sizeof(sin)) == SOCKET_ERROR) {
		cout << "fail to bind " << endl;
	}
	else {
		cout << "bind succeed!" << endl;
	}
	/*3.����Ϊ��������ģʽ*/
	if (type == SOCK_STREAM && listen(s, theLength) == SOCKET_ERROR) {
		cout << "fail to listen on the port " << port << endl;
	}
	else {
		cout << "Set listen!" << endl;
	}

	return s;

}

/*���պͷ�����Ϣ*/
void receiveAndSend(SOCKET* newSock) {
	char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];//������Ϣ

	int ret;
	/*5.���������û����󣬹��첢������Ӧ��ע��ÿ�ζ�Ҫ�����µ��׽��֣�*/
	SOCKET sock = *(SOCKET*)newSock;
	while (true)
	{
		ZeroMemory(recvBuf, BUF_SIZE);//��ջ���
		ZeroMemory(sendBuf, BUF_SIZE);
		//���տͻ�������
		ret = recv(sock, recvBuf, BUF_SIZE, 0);

		if (ret == SOCKET_ERROR)
		{
			cout << "fail to recv!" << endl;
			break;
		}
		if (recvBuf[0] == '0' && strlen(recvBuf) == 1) {//�����յ�һ��0���˳�ѭ�����ر�����
			cout << "finished!" << endl;
			closesocket(sock); //�ر��׽���
			WSACleanup();
			exit(0);
		}
		cout << endl << "�ͻ��˷�������Ϣ: " << recvBuf << endl;

		strcpy_s(sendBuf, "Server:");
		strcat_s(sendBuf, recvBuf);
		cout << "��ͻ��˷�����Ϣ: " << sendBuf << endl;
		cout << "------------------------------" << endl;

		send(sock, sendBuf, strlen(sendBuf), 0);//���޸ĺ����Ϣ���͸��ͻ���


	}
}

/*������Ϣ*/
void sendM(SOCKET* newSock) {
	char sendBuf[BUF_SIZE];
	int ret;
	SOCKET sock = *(SOCKET*)newSock;
	cout << "��Ҫ�˳������뵥��0" << endl;
	cout << "-------------------------------------" << endl;
	while (true) {
		ZeroMemory(sendBuf, BUF_SIZE);
		cout << "������Ҫ��ͻ��˷��͵���Ϣ��";
		cin >> sendBuf;
		cout << "------------------------------" << endl;
		ret = send(sock, sendBuf, strlen(sendBuf), 0);
		if (ret == SOCKET_ERROR) {
			cout << "fail to send!" << endl;
		}
		if (strcmp(sendBuf, "0") == 0) {//������Ϊ0ʱ���˳�
			cout << "------------------------------" << endl;
			cout << "Close!" << endl;
			closesocket(sock); //�ر��׽���
			WSACleanup();
			exit(0);
		}
	}

}

int main() {
	struct sockaddr_in clientSin;
	WSADATA wsadata;
	//����WSAStartup()��������ʼ��socket�⣬����0��ʾ�ɹ��������ʾһ����ֵ
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		cout << "����winsocket��̬��ʧ��" << endl;
		return 1;
	}
	SOCKET mySock, newSock;
	mySock = toSock(MYPORT, "tcp", THELEN);//�����׽��ֲ�����Ϊ����״̬
	int sockaddrLen = sizeof(sockaddr);
	/*4.��accept�����������󣬽�������*/
	newSock = accept(mySock, (struct sockaddr*) & clientSin, &sockaddrLen);
	if (newSock == INVALID_SOCKET) {
		cout << "fail to accept!" << endl;
		closesocket(mySock);	//�ر��׽���		
		WSACleanup();
		return 1;
	}
	else {
		cout << "accept!" << endl;
		char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];//������Ϣ

		/*5.���������û����󣬹��첢������Ӧ��ע��ÿ�ζ�Ҫ�����µ��׽��֣�*/
		//�����������߳�
		_beginthread((void(*)(void*))receiveAndSend, 0, &newSock);
		_beginthread((void(*)(void*))sendM, 0, &newSock);

		Sleep(INFINITE);	//ʹ������������

	}

	//�˳�
	closesocket(mySock);	//�ر��׽���
	closesocket(newSock);	//�ر��׽���
	WSACleanup();
	return 0;
}