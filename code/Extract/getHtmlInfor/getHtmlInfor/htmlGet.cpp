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
	//fp = fopen("./try.html", "wb");
	fp = fopen("./request.txt", "ab");
	if (fp) {
		fwrite(recvStr.c_str(), sizeof(char), recvStr.length(), fp);
		fclose(fp);
		fp = NULL;
	}
	else {
		cout << "fail wo open the file" << endl;
	}

}

/*发送POST请求*/
bool sendRequestPOST(SOCKET sock, const string& host, const string& resource, const string& cookie, const int page, const int schoolId) {

	char sSchoolId[8], sPage[3], sLen[10];
	sprintf(sSchoolId, "%d", schoolId);//转换学校id
	sprintf(sPage, "%d", page);//转换页数
	string sendData = "schoolId=";
	sendData += sSchoolId;
	sendData += "&p=";
	sendData += sPage;
	sendData += "&psize=20&type=1&courseStatus=30";
	int len = sendData.length();
	cout << "len:" << len << endl;
	sprintf(sLen, "%d", len);
	/*由于这个url要求post请求方式,为了获取课程的url*/
	string http = "POST " + resource + " HTTP/1.1\r\n"//注意这个地方GET后和HTTP前都有空格
		+ "HOST: " + host + "\r\n"//主机
		+ "user-agent: " + "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36" + "\r\n"
		+ "content-length: " + sLen + "\r\n"//发送内容的长度
		+ "edu-script-token: afa9409b3371479f8b4c6825c16e6485\r\n"
		+ "content-type: application/x-www-form-urlencoded\r\n"
		+ "origin: https://www.icourse163.org\r\n"
		//cookie一定要加，只需要改变最后等号后的数字
		+ "cookie: EDUWEBDEVICE=5cc43b1c6be3410d9d026dea219370ae; __utmz=63145271.1570507353.37.2.utmcsr=baidu|utmccn=(organic)|utmcmd=organic; hasVolume=true; bpmns=1; __yadk_uid=xO1HmCxrPaqx42dWqkZys0AZvI9sHlQs; videoResolutionType=1; __utma=63145271.872302309.1564057816.1585637934.1589002596.77; videoVolume=0.8; videoRate=1.75; WM_NI=M5gLG8YoaeGyljSYwvHt3YDhslmvJb6LxIyQMqjJtbdRTMF3FFEpZ01ouRpsZ2stKk9yGi%2FuugEFrkt6jXZSRRzelr2xp0yl47FbapI3IzjBCGVroNgf2DCysWKAbBYXM2U%3D; WM_TID=S0F0l%2B%2FWhIBBABVRAVY84Q3SErDMwtf8; WM_NIKE=9ca17ae2e6ffcda170e2e6eea3d250b2edaed1b247a7868eb3c55a939b9eaff55d86878c92dc7de987fd97b12af0fea7c3b92a8feba9adc6349bb1af86c673b0b69dbaf67e8cacfdbac764b1b4a397c96af694b8aafc5b8fafa6abf76aa88e9caec539869a8b91d93cf3b1bbb3f3799aeebd95ca61fba7898aae7fb7b98ba7f161b8eaa6b9d959928eb686ef7af2a8c0d1fc68f8e98a83e47c8bed879be94a81999a93ee46a5bafea5c273b18985a2e14a9b8883d3d837e2a3; hb_MA-A976-948FFA05E931_source=www.baidu.com; NTESSTUDYSI=afa9409b3371479f8b4c6825c16e6485; STUDY_INFO=18328270896|11|1148692370|1589441951719; STUDY_SESS=\"rxk8qABE7O + 1L8p / bmbKftRzFH8cW4n459Is + gsA13GiXaKgjM5buuZ / k42Y2HcsxTm4Ml4Hmap5uvwRV + mCJJYbi / kFGq3 + 1KcpF0nu9LMPsCvu7MTfnkvTnhQMJFjg61DquyE + eYl52 + / 2Ukw0gwuG6vY2bbARv0dwSKRX7cVMjz6VkrIeGLJw1Cj6D7 + g\"; STUDY_PERSIST=\"u2T1SpCIce1B / 5d0UXWGevU2esrmd / P2zD6eZLMBbuFN77uPfqahAgS76jLTRR1tEnO + mo8ZhuC7P7HldFihwa7M3eHnIaM91YbPs8aSGApZwSgAmRwwQNJ1toZHMWMMqmbHHwq6ETVsY + ELRAmGRK8b5urJ7CFAWj4uBA7 + xiqVYl6y14R0cFTVUMMwrgWa0JaRtMrIAs26DsSrhOFhWYZBO5bgjj8YI7fTd3n5kBzkt1ON + LxfwujHmWWvahsv8WQLi3xTJ45sq / acjsEWiA == \"; NETEASE_WDA_UID=1148692370#|#1540782303004; Hm_lvt_77dc9a9d49448cf5e629e5bebaa5500b=1589420776,1589438477,1589438684,1589441953; Hm_lpvt_77dc9a9d49448cf5e629e5bebaa5500b=1589469524\r\n"
		+ "referer: https://www.icourse163.org/university/HUST\r\n"
		+ "Connection: close\r\n\r\n"
		+ sendData + "\r\n\r\n";//发送的内容，为from data的内容
		//发送内容中的p=1，代表页数

	if (http.size() == send(sock, &http[0], http.size(), 0)) {

		return true;
	}
	else {
		return false;
	}

}
