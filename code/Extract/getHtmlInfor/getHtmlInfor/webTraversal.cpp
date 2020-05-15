
#include"htmlGet.h"
#include"webTraversal.h"
#include "getInfor.h"

#include<iostream>

using namespace std;

/*����дһ����ȡ����γ�url�ĺ������ù�ȱ������ð���ʵ��ʵ������ܾ�һ��*/
void GetCourseURL(string url) {
	int page = 1;
	string host, resource;
	SOCKET sock;
	string cookie;
	int schoolId = 9008;//�ȴ��̶������ɣ���ʱ����һ��ѧУ��������վ
	queue<string> courseUrl;
	char filename[30] = "./classOfHUST.txt";

	FILE* fp = NULL;
	fp = fopen(filename, "wb");
	fclose(fp);
	fp = NULL;

	while (true) {


		//������һҳ��Ӧ���ǰɣ�

		if (parseURL(url, host, resource) == false) {
			return;
		}

		sock = connectSock(host.c_str(), 80, "tcp");

		if (sock == SOCKET_ERROR) {
			cout << "fail to connectSock" << endl;
		}

		else {
			//��������ҳ
			if (sendRequestPOST(sock, host, resource, cookie, page, schoolId) == true && page <= 2) {
			//if (sendRequestPOST(sock, host, resource, cookie, page, schoolId) == true && page <= 5) {
				string recvStr = recvRequest(sock, url);
				if (recvStr.length() != 0) {
					//�ð������������ȡurl�ĺ���
					getURLFromFile(recvStr, courseUrl);
					getClassInfor(courseUrl, filename);
					downLoad(recvStr, "CourseURL.txt");
					cout << "page:" << page << endl;
					page++;
					Sleep(200);
				}
				else {
					break;
				}
			}
			else {
				break;
			}

		}
		closesocket(sock);
	}

}
/*���ļ��л�ȡ�γ�URL*/
void getURLFromFile(const string& recvStr, queue<string>& courseUrl) {
	int index = 0;
	int indexEnd = 0;
	//cout << recvStr[index] << endl;
	string str = "\0";

	FILE* fp = NULL;
	fp = fopen("./�γ�url.txt", "wb");
	fclose(fp);
	fp = NULL;

	do {
		index = recvStr.find("\"id\":", index);
		if (index != -1) {
			index += strlen("\"id\":");

			indexEnd = recvStr.find(",", index);
			str = recvStr.substr(index, indexEnd - index);
			courseUrl.push(str);//ָ���γ̵�id����


			fp = fopen("./�γ�url.txt", "ab");//�����Բ���
			if (fp) {
				fwrite(str.c_str(), sizeof(char), str.length(), fp);
				fclose(fp);
				fp = NULL;
			}
			else {
				cout << "fail wo open the file" << endl;
			}
			index = indexEnd;
		}

	} while (index != -1);

}

/*��ÿγ���Ϣ*/
void getClassInfor(queue<string>& courseUrl, char* filename) {
	string url = "https://www.icourse163.org/course/HUST-";
	//int qLen = courseUrl.size();
	while (courseUrl.size() != 0) {
		connnctCourse(url + courseUrl.front(), filename);
		courseUrl.pop();
	}
}

/*���ӿγ���ҳ*/
void connnctCourse(string url, char* filename) {
	string host, resource;
	SOCKET sock;

	if (parseURL(url, host, resource) == false) {
		return;
	}

	sock = connectSock(host.c_str(), 80, "tcp");

	if (sock == SOCKET_ERROR) {
		cout << "fail to connectSock" << endl;
	}
	else {
		if (sendRequest(sock, host, resource) == true) {
			string recvStr = recvRequest(sock, url);
			if (recvStr.length() != 0) {
				ClassInfor oneCourse;
				initClassInfor(&oneCourse);//��ʼ��
				//��ȡhtml����Ϣ
				getSchoolName(recvStr, &oneCourse);//ѧУ����
				getCourseName(recvStr, &oneCourse);//�γ�����
				getCourseTime(recvStr, &oneCourse);//����ʱ��
				getStudentNum(recvStr, &oneCourse);//�Ѳμ�����
				getCourseDescribe(recvStr, &oneCourse);//�γ̸���
				getCourseAim(recvStr, &oneCourse);//�ڿ�Ŀ��
				getContent(recvStr, &oneCourse);//�γ̴��
				getReference(recvStr, &oneCourse);//�ο�����

				writeInfor(&oneCourse, filename);//д���ļ�
			}


		}

		closesocket(sock);

	}
}
