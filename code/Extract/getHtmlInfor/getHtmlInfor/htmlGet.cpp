#include"htmlGet.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS//����gethostbyname��inet_addr����
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>


#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024000
using namespace std;

/*����URL*/
bool parseURL(const string& url, string& host, string& resource) {

	int index = url.find("://");
	//δ�ҵ�ʱ������npos
	if (index == string::npos) {
		cout << "fail to parseURL" << endl;
		return false;
	}
	index += strlen("://");
	//��index��ʼ��Ѱ��֮��ĵ�һ��/
	int x = url.size();
	int index1 = url.find_first_of('/', index);
	//������û��'/'��ֻ��host
	if (index1 == string::npos) {
		host = url.substr(index, url.size() - index);
		resource = "/";
	}
	else {
		//����վ���ͺ�벿�ֱַ��ȡ
		host = url.substr(index, index1 - index);
		resource = url.substr(index1, url.size() - index1);
	}

	return true;
}

/*�����׽��ֲ���������*/
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
		cout << "IP:" << inet_ntoa(*(struct in_addr*)pHostent->h_addr_list[0]) << endl;
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

	/*�����׽���*/
	s = socket(PF_INET, type, pProtoent->p_proto);
	if (s == INVALID_SOCKET) {
		cout << "fail to create socket" << endl;
	}
	else {
		cout << "create the socket!" << endl;
	}
	/*���䱾�ض˵��ַ��ϵͳ���*/

	/*��������*/
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

/*��������*/
bool sendRequest(SOCKET sock, const string& host, const string& resource) {
	string http = "GET " + resource + " HTTP/1.1\r\n"//ע������ط�GET���HTTPǰ���пո�
		+ "HOST: " + host + "\r\n"//����
		+ "Connection: close\r\n\r\n";
	//�ж��Ƿ��ͳɹ�
	if (http.size() == send(sock, &http[0], http.size(), 0)) {

		return true;
	}
	else {
		return false;
	}
}

/*��������*/
string recvRequest(SOCKET sock, const string& url) {
	char* buf = new char[BUF_SIZE];
	string bufRecv;
	memset(buf, 0, BUF_SIZE);
	int bytesread = 0;
	int ret = 1;//��¼���ν��ն���
	cout << "��ȡ";
	while (ret > 0) {//�п��ܽ��յ������ݿ��ܴ���buf�ĳ��ȣ�����ֻҪret>0��Ҫһֱ����
		ret = recv(sock, buf + bytesread, BUF_SIZE - bytesread, 0);
		if (ret > 0) {
			bytesread += ret;//��¼�ܹ������˶���
		}
		//���յ������ݴ���buf�ĳ��ȣ����·������Ŀռ�
		if (BUF_SIZE - bytesread < 100) {
			cout << endl << "���·���ռ䣡" << endl;
			char* mbuf = new char[2 * BUF_SIZE];
			strcpy(mbuf, buf);
			delete[] buf;//�ͷ�
			buf = mbuf;
		}
		cout << ret << " ";//��ʾÿ�ν�������
	}
	cout << endl;//���պ���
	buf[bytesread] = '\0';//����ַ���������־
	bufRecv.assign(buf, bytesread);

	if (bytesread > 11) {
		if (bufRecv[9] == '2' && bufRecv[10] == '0' && bufRecv[11] == '0') {
			return bufRecv;
		}
		else {
			//cout << "״̬�룺" << bufRecv[9] << bufRecv[10] << bufRecv[11] << endl;
			cout << "���ʴ���" << endl;
			return "";
		}
	}
	else {
		return "";
	}
}

/*д���ļ�*/
void downLoad(const string& recvStr) {
	FILE* fp = NULL;
	fp = fopen("./try.html", "wb");
	if (fp) {
		fwrite(recvStr.c_str(), sizeof(char), recvStr.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail wo open the file" << endl;
	}

}

