#include"htmlGet.h"
#include"getInfor.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS//消除gethostbyname和inet_addr报错
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>


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
	//const string url = "http://qiming.hust.edu.cn";
	//const string url = "http://www.icourse163.org/university/HUST";

	//const string url = "https://www.icourse163.org/course/HUST-1450829165";//大学体育,无参考资料，json大纲
	//const string url = "https://www.icourse163.org/course/HUST-1450326327";//工程，有参考资料
	//const string url = "https://www.icourse163.org/course/HUST-1207043813";//数电
	//const string url = "https://www.icourse163.org/course/HUST-1207043812";//C语言

	const string url = "https://www.icourse163.org/course/ZJU-1450024180";//试一个别的学校的

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
		if (pos != string::npos) {
			recvStr = recvStr.substr(pos, recvStr.length() - pos);
			downLoad(recvStr);

			//获取html中信息
			getSchoolName(recvStr);//学校名称
			getCourseName(recvStr);//课程名称
			getCourseTime(recvStr);//开课时间
			getStudentNum(recvStr);//已参加人数
			getCourseDescribe(recvStr);//课程概述
			getCourseAim(recvStr);//授课目标
			getContent(recvStr);//课程大纲
			getReference(recvStr);//参考资料
			
		}
	}


	closesocket(clientSock); //关闭套接字
	WSACleanup(); //释放套接字资源

	return 0;
}