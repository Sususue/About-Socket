#ifndef _HTMLGET_H_
#define _HTMLGET_H_

#define _WINSOCK_DEPRECATED_NO_WARNINGS//����gethostbyname��inet_addr����
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>


#pragma comment(lib, "ws2_32.lib")

//using std::string;

/*����URL

url:��Ҫ������url
host:�õ��������host
resource:�õ���host�ĺ�벿��

����:true�ɹ�����/false����ʧ��
*/
bool parseURL(const std::string& url, std::string& host, std::string& resource);

/*�����׽��ֲ���������

host:����
port:�˿ں�
transport:tcp/udp

����:�����õ��׽���
*/
SOCKET connectSock(const char* host, const int port, const char* transport);

/*��������*/
bool sendRequest(SOCKET sock, const std::string& host, const std::string& resource);

/*��������*/
std::string recvRequest(SOCKET sock, const std::string& url);

/*д���ļ�

recvAtr:����ҳ���յ����ַ�

*/
void downLoad(const std::string& recvStr, const std::string& filename);

/*����POST����*/
bool sendRequestPOST(SOCKET sock, const std::string& host, const std::string& resource, const std::string& schoolname, const int page, const std::string schoolId);

/*���ӿγ���ҳ*/
std::string connectHtml(std::string url);

#endif 
