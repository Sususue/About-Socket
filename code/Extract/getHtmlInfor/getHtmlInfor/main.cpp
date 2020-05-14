#include"htmlGet.h"
#include"getInfor.h"
#include"webTraversal.h"
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
	//const string url = "http://qiming.hust.edu.cn";//启明学院网站
	//const string url = "http://www.icourse163.org/university/HUST";//华科慕课网站
	//慕课网站url后面一般是 /学校英文缩写-区分课程的数字

	//尝试单页面提取信息
	const string url = "https://www.icourse163.org/course/HUST-1450829165";//大学体育,无参考资料，json大纲
	//const string url = "https://www.icourse163.org/course/HUST-1450326327";//工程，有参考资料
	//const string url = "https://www.icourse163.org/course/HUST-1207043813";//数电
	//const string url = "https://www.icourse163.org/course/HUST-1207043812";//C语言

	//const string url = "https://www.icourse163.org/course/ZJU-1450024180";//试一个别的学校的

	//终于找到了哈哈哈哈……    返回的是json格式的，在里面找课程的url
	//const string url = "https://www.icourse163.org/web/j/courseBean.getCourseListBySchoolId.rpc?csrfKey=afa9409b3371479f8b4c6825c16e6485";


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
		//int pos = 0;
		//pos = recvStr.find('<', 0);
		//if (pos != string::npos) {
			//recvStr = recvStr.substr(pos, recvStr.length() - pos);


			//downLoad(recvStr);
			
			////单个页面提取信息
			//ClassInfor oneCourse;
			//initClassInfor(&oneCourse);//初始化
			////获取html中信息
			//getSchoolName(recvStr, &oneCourse);//学校名称
			//getCourseName(recvStr, &oneCourse);//课程名称
			//getCourseTime(recvStr, &oneCourse);//开课时间
			//getStudentNum(recvStr, &oneCourse);//已参加人数
			//getCourseDescribe(recvStr, &oneCourse);//课程概述
			//getCourseAim(recvStr, &oneCourse);//授课目标
			//getContent(recvStr, &oneCourse);//课程大纲
			//getReference(recvStr, &oneCourse);//参考资料

			////保存为json文件
			//writeInfor(&oneCourse);
			
		//}
	}

	GetCourseURL("https://www.icourse163.org/web/j/courseBean.getCourseListBySchoolId.rpc?csrfKey=afa9409b3371479f8b4c6825c16e6485");

	closesocket(clientSock); //关闭套接字
	WSACleanup(); //释放套接字资源

	return 0;
}