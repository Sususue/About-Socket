#include"htmlGet.h"
#include"getInfor.h"
#include"webTraversal.h"
#include"MyQueue.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS//消除gethostbyname和inet_addr报错
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

/*从学校页面找出各个学校的url进行访问
  访问具体学校，从html中获取学校id，
  利用学校id访问学校的课程
*/
int main() {

	WSADATA wsadata;
	//调用WSAStartup()函数来初始化socket库，返回0表示成功，否则表示一个常值
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		cout << "加载winsocket动态库失败" << endl;
		return 1;
	}
	//慕课网站url后面一般是 /学校英文缩写-区分课程的数字
	/*string school[5];
	string schoolid[5];
	initSchool(school,schoolid);*/

	Myqueue schoolUrl;//存学校url
	Myqueue schoolid;//存学校id
	Myqueue school;//存学校英文简称

	string firstUrl = "http://www.icourse163.org/university/view/all.htm";
	string recvStr = connectHtml(firstUrl);//获取返回的内容
	findurl(recvStr, &schoolUrl, &school);//找到url存进队列,为节省时间，暂时人为控制只存5个学校

	string id;
	while (schoolUrl.sizeQ() != 0) {//找到所有学校的id
		recvStr = connectHtml("https://www.icourse163.org/" + schoolUrl.frontQ());
		id = findid(recvStr);
		schoolid.pushQ(id);
		schoolUrl.popQ();
	}

	
	
	//获取各个学校的信息
	string url = "https://www.icourse163.org/web/j/courseBean.getCourseListBySchoolId.rpc?csrfKey=afa9409b3371479f8b4c6825c16e6485" ;
	int cnt = 0;
	while (school.sizeQ() != 0 && schoolid.sizeQ() != 0) {
		if (cnt < 1) {
			threadUse* one = new threadUse(url, school.frontQ(), schoolid.frontQ());
			_beginthread((void(*)(void*))oneToGet, 0, one);//单独开一个线程
		}
		else {
			GetCourseURL(url, school.frontQ(), schoolid.frontQ());
		}
		school.popQ();
		schoolid.popQ();
		cnt++;
	}
	

	WSACleanup(); //释放套接字资源

	return 0;
}

void oneToGet(threadUse* one) {

	string url = (*(threadUse*)one).url;
	string school = (*(threadUse*)one).school;
	string id = (*(threadUse*)one).id;
	GetCourseURL(url, school, id);
}
