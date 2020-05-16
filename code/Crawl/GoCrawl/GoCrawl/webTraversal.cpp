
#include"htmlGet.h"
#include"webTraversal.h"
#include "getInfor.h"
#include<iostream>

using namespace std;

/*����дһ����ȡ����γ�url�ĺ������ù�ȱ������ð���ʵ��ʵ������ܾ�һ��*/
void GetCourseURL(string url, string school, string schoolid) {
	int page = 1;
	string host, resource;
	SOCKET sock;
	string cookie;
	
	//while (school.sizeQ() != 0 && schoolid.sizeQ() != 0) {
		Myqueue courseUrl;

		//char filename[30] = "./data/classOfHUST.json";
		char filename[30] = "./data/classOf";
		strcat(filename,school.c_str());
		strcat(filename, ".json");

		FILE* fp = NULL;
		fp = fopen("CourseURL.txt", "wb");
		fclose(fp);
		fp = NULL;

		cJSON* root;
		root = cJSON_CreateArray();

		while (true) {
			if (parseURL(url, host, resource) == false) {
				return;
			}
			sock = connectSock(host.c_str(), 80, "tcp");

			if (sock == SOCKET_ERROR) {
				cout << "fail to connectSock" << endl;
			}

			else {
				//��������ҳ
				//if (sendRequestPOST(sock, host, resource, cookie, page, schoolId) == true && page <= 2) {
				//��ʱ��ÿ��ѧУ���ֻ��ȡ��ҳ
				if (sendRequestPOST(sock, host, resource, school, page, schoolid.c_str()) == true && page <= 2) {
					string recvStr = recvRequest(sock, url);
					if (recvStr.length() != 0) {
						//�ð������������ȡurl�ĺ���
						getURLFromFile(recvStr, &courseUrl);
						getClassInfor(&courseUrl, root,school);
						downLoad(recvStr, "CourseURL.txt");
						cout << "--------------------------" << endl;
						cout << "page:" << page << endl;
						cout << "--------------------------" << endl;
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
		FILE* fp1 = NULL;
		fp1 = fopen(filename, "wb");
		if (fp1) {
			//fwrite(cJSON_Print(root), sizeof(char), strlen(cJSON_Print(root)), fp);
			if (fprintf(fp1, "%s", cJSON_Print(root)) != 0) {
				cout << "write" << endl;
			}
			fclose(fp1);
			fp1 = NULL;
		}
		else {
			cout << "fail wo open the file" << endl;
		}
		cJSON_Delete(root);
		//page = 1;//����page

		/*school.popQ();
		schoolid.popQ();*/
	//}
}


/*���ļ��л�ȡ�γ�URL*/
void getURLFromFile(const string& recvStr, Myqueue* courseUrl) {
	int index = 0;
	int indexEnd = 0;
	string str = "\0";
	int temp = 0;


	do {
		index = recvStr.find("\"id\":", index);
		if (index != -1) {
			index += strlen("\"id\":");

			indexEnd = recvStr.find(",", index);
			str = recvStr.substr(index, indexEnd - index);
			(*courseUrl).pushQ(str);//ָ���γ̵�id����
			//����tags��Ϊnullʱ���ֻ���
			index = recvStr.find("}", indexEnd);

		}

	} while (index != -1);

}

/*��ÿγ���Ϣ*/
void getClassInfor(Myqueue* courseUrl, cJSON* root, string school) {
	//string url = "https://www.icourse163.org/course/HUST-";
	string url = "https://www.icourse163.org/course/"+school+"-";

	while ((*courseUrl).sizeQ() != 0) {
		cJSON* rootData;
		rootData = cJSON_CreateObject();//����һ������
		connnctCourse(url + (*courseUrl).frontQ(), rootData);
		(*courseUrl).popQ();
		cJSON_AddItemToArray(root, rootData);

	}
}

/*���ӿγ���ҳ*/
void connnctCourse(string url, cJSON* root) {
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

				writeInfor(&oneCourse, root);//д���ļ�
			}


		}

		closesocket(sock);

	}
}
