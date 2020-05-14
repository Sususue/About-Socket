#include"htmlGet.h"
#include"getInfor.h"
#include"webTraversal.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS//����gethostbyname��inet_addr����
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>


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
	//const string url = "http://qiming.hust.edu.cn";//����ѧԺ��վ
	//const string url = "http://www.icourse163.org/university/HUST";//����Ľ����վ
	//Ľ����վurl����һ���� /ѧУӢ����д-���ֿγ̵�����

	//���Ե�ҳ����ȡ��Ϣ
	const string url = "https://www.icourse163.org/course/HUST-1450829165";//��ѧ����,�޲ο����ϣ�json���
	//const string url = "https://www.icourse163.org/course/HUST-1450326327";//���̣��вο�����
	//const string url = "https://www.icourse163.org/course/HUST-1207043813";//����
	//const string url = "https://www.icourse163.org/course/HUST-1207043812";//C����

	//const string url = "https://www.icourse163.org/course/ZJU-1450024180";//��һ�����ѧУ��

	//�����ҵ��˹�����������    ���ص���json��ʽ�ģ��������ҿγ̵�url
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
			
			////����ҳ����ȡ��Ϣ
			//ClassInfor oneCourse;
			//initClassInfor(&oneCourse);//��ʼ��
			////��ȡhtml����Ϣ
			//getSchoolName(recvStr, &oneCourse);//ѧУ����
			//getCourseName(recvStr, &oneCourse);//�γ�����
			//getCourseTime(recvStr, &oneCourse);//����ʱ��
			//getStudentNum(recvStr, &oneCourse);//�Ѳμ�����
			//getCourseDescribe(recvStr, &oneCourse);//�γ̸���
			//getCourseAim(recvStr, &oneCourse);//�ڿ�Ŀ��
			//getContent(recvStr, &oneCourse);//�γ̴��
			//getReference(recvStr, &oneCourse);//�ο�����

			////����Ϊjson�ļ�
			//writeInfor(&oneCourse);
			
		//}
	}

	GetCourseURL("https://www.icourse163.org/web/j/courseBean.getCourseListBySchoolId.rpc?csrfKey=afa9409b3371479f8b4c6825c16e6485");

	closesocket(clientSock); //�ر��׽���
	WSACleanup(); //�ͷ��׽�����Դ

	return 0;
}