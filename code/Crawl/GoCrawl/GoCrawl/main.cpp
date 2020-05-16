#include"htmlGet.h"
#include"getInfor.h"
#include"webTraversal.h"
#include"MyQueue.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS//����gethostbyname��inet_addr����
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>
#include <process.h>


#pragma comment(lib, "ws2_32.lib")

using namespace std;

typedef struct threadUse {
	string url;
	string school;
	string id;
	threadUse(string sUrl,string sSchool,string sId) {
		url = sUrl;
		school = sSchool;
		id = sId;
	}
}threadUse;

void oneToGet(threadUse* one);
//void initSchool(string* school, string* schoolid) {
//	school[0] = "PKU";
//	school[1] = "NJU";
//	school[2] = "ZJU";
//	school[3] = "FUDAN";
//	school[4] = "BUAA";
//
//	schoolid[0] = "13001";
//	schoolid[1] = "9002";
//	schoolid[2] = "6001";
//	schoolid[3] = "9003";
//	schoolid[4] = "8011";
//
//}

/*��ѧУҳ���ҳ�����ѧУ��url���з���
  ���ʾ���ѧУ����html�л�ȡѧУid��
  ����ѧУid����ѧУ�Ŀγ�
*/
int main() {

	WSADATA wsadata;
	//����WSAStartup()��������ʼ��socket�⣬����0��ʾ�ɹ��������ʾһ����ֵ
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		cout << "����winsocket��̬��ʧ��" << endl;
		return 1;
	}
	//Ľ����վurl����һ���� /ѧУӢ����д-���ֿγ̵�����
	/*string school[5];
	string schoolid[5];
	initSchool(school,schoolid);*/

	Myqueue schoolUrl;//��ѧУurl
	Myqueue schoolid;//��ѧУid
	Myqueue school;//��ѧУӢ�ļ��

	string firstUrl = "http://www.icourse163.org/university/view/all.htm";
	string recvStr = connectHtml(firstUrl);//��ȡ���ص�����
	findurl(recvStr, &schoolUrl, &school);//�ҵ�url�������,Ϊ��ʡʱ�䣬��ʱ��Ϊ����ֻ��5��ѧУ

	string id;
	while (schoolUrl.sizeQ() != 0) {//�ҵ�����ѧУ��id
		recvStr = connectHtml("https://www.icourse163.org/" + schoolUrl.frontQ());
		id = findid(recvStr);
		schoolid.pushQ(id);
		schoolUrl.popQ();
	}

	
	
	//��ȡ����ѧУ����Ϣ
	string url = "https://www.icourse163.org/web/j/courseBean.getCourseListBySchoolId.rpc?csrfKey=afa9409b3371479f8b4c6825c16e6485" ;
	int cnt = 0;
	while (school.sizeQ() != 0 && schoolid.sizeQ() != 0) {
		if (cnt < 1) {
			threadUse* one = new threadUse(url, school.frontQ(), schoolid.frontQ());
			_beginthread((void(*)(void*))oneToGet, 0, one);//������һ���߳�
		}
		else {
			GetCourseURL(url, school.frontQ(), schoolid.frontQ());
		}
		school.popQ();
		schoolid.popQ();
		cnt++;
	}
	

	WSACleanup(); //�ͷ��׽�����Դ

	return 0;
}

void oneToGet(threadUse* one) {

	string url = (*(threadUse*)one).url;
	string school = (*(threadUse*)one).school;
	string id = (*(threadUse*)one).id;
	GetCourseURL(url, school, id);
}
