#include"htmlGet.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS//消除gethostbyname和inet_addr报错
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>


#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024000
using namespace std;

/*解析URL*/
bool parseURL(const string& url, string& host, string& resource) {

	int index = url.find("://");
	//未找到时，返回npos
	if (index == string::npos) {
		cout << "fail to parseURL" << endl;
		return false;
	}
	index += strlen("://");
	//从index开始，寻找之后的第一个/
	int x = url.size();
	int index1 = url.find_first_of('/', index);
	//若后面没有'/'则只有host
	if (index1 == string::npos) {
		host = url.substr(index, url.size() - index);
		resource = "/";
	}
	else {
		//将网站名和后半部分分别获取
		host = url.substr(index, index1 - index);
		resource = url.substr(index1, url.size() - index1);
	}

	return true;
}

/*创建套接字并建立连接*/
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

	/*创建套接字*/
	s = socket(PF_INET, type, pProtoent->p_proto);
	if (s == INVALID_SOCKET) {
		cout << "fail to create socket" << endl;
	}
	else {
		cout << "create the socket!" << endl;
	}
	/*分配本地端点地址，系统完成*/

	/*建立连接*/
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

/*发送请求*/
bool sendRequest(SOCKET sock, const string& host, const string& resource) {
	string http = "GET " + resource + " HTTP/1.1\r\n"//注意这个地方GET后和HTTP前都有空格
		+ "HOST: " + host + "\r\n"//主机
		+ "Connection: close\r\n\r\n";
	//判断是否发送成功
	if (http.size() == send(sock, &http[0], http.size(), 0)) {

		return true;
	}
	else {
		return false;
	}
}

/*发起请求*/
string recvRequest(SOCKET sock, const string& url) {
	char* buf = new char[BUF_SIZE];
	string bufRecv;
	memset(buf, 0, BUF_SIZE);
	int bytesread = 0;
	int ret = 1;//记录单次接收多少
	cout << "读取";
	while (ret > 0) {//有可能接收到的数据可能大于buf的长度，所以只要ret>0就要一直接收
		ret = recv(sock, buf + bytesread, BUF_SIZE - bytesread, 0);
		if (ret > 0) {
			bytesread += ret;//记录总共接收了多少
		}
		//接收到的数据大于buf的长度，重新分配更大的空间
		if (BUF_SIZE - bytesread < 100) {
			cout << endl << "重新分配空间！" << endl;
			char* mbuf = new char[2 * BUF_SIZE];
			strcpy(mbuf, buf);
			delete[] buf;//释放
			buf = mbuf;
		}
		cout << ret << " ";//显示每次接收数量
	}
	cout << endl;//接收后换行
	buf[bytesread] = '\0';//添加字符串结束标志
	bufRecv.assign(buf, bytesread);

	if (bytesread > 11) {
		if (bufRecv[9] == '2' && bufRecv[10] == '0' && bufRecv[11] == '0') {
			return bufRecv;
		}
		else {
			//cout << "状态码：" << bufRecv[9] << bufRecv[10] << bufRecv[11] << endl;
			cout << "访问错误" << endl;
			return "";
		}
	}
	else {
		return "";
	}
}

/*写入文件*/
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

