
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
	char filename[30] = "./data/classOfHUST.json";

	FILE* fp = NULL;
	fp = fopen("CourseURL.txt", "wb");
	fclose(fp);
	fp = NULL;

	cJSON* root;
	root = cJSON_CreateArray();

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
			//if (sendRequestPOST(sock, host, resource, cookie, page, schoolId) == true && page <= 2) {
			//��ʱֻ��ǰ��ҳ�����һҳ��tags����Ϊnull,��ɸ���
			if (sendRequestPOST(sock, host, resource, cookie, page, schoolId) == true && page <= 4) {
				string recvStr = recvRequest(sock, url);
				if (recvStr.length() != 0) {
					//�ð������������ȡurl�ĺ���
					getURLFromFile(recvStr, courseUrl);
					getClassInfor(courseUrl, root);
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

		

		/*fclose(fp);
		fp = NULL;*/
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

}
/*���ļ��л�ȡ�γ�URL*/
void getURLFromFile(const string& recvStr, queue<string>& courseUrl) {
	int index = 0;
	int indexEnd = 0;
	//cout << recvStr[index] << endl;
	string str = "\0";
	int temp = 0;

	/*FILE* fp = NULL;
	fp = fopen("./�γ�url.txt", "wb");
	fclose(fp);
	fp = NULL;*/

	do {
		index = recvStr.find("\"id\":", index);
		if (index != -1) {
			index += strlen("\"id\":");

			indexEnd = recvStr.find(",", index);
			str = recvStr.substr(index, indexEnd - index);
			courseUrl.push(str);//ָ���γ̵�id����


			//fp = fopen("./�γ�url.txt", "ab");//�����Բ���
			//if (fp) {
			//	fwrite(str.c_str(), sizeof(char), str.length(), fp);
			//	fclose(fp);
			//	fp = NULL;
			//}
			//else {
			//	cout << "fail wo open the file" << endl;
			//}
			
			index = indexEnd;

		}

	} while (index != -1);

}

/*��ÿγ���Ϣ*/
void getClassInfor(queue<string>& courseUrl, cJSON* root) {
	string url = "https://www.icourse163.org/course/HUST-";
	//int qLen = courseUrl.size();
	//cJSON* root;
	//root = cJSON_CreateArray();
	while (courseUrl.size() != 0) {
		cJSON* rootData;
		rootData = cJSON_CreateObject();//����һ������
		connnctCourse(url + courseUrl.front(), rootData);
		courseUrl.pop();
		cJSON_AddItemToArray(root, rootData);

	}
	//FILE* fp = NULL;
	//fp = fopen(filename, "ab");
	//if (fp) {
	//	//fwrite(cJSON_Print(root), sizeof(char), strlen(cJSON_Print(root)), fp);
	//	if (fprintf(fp, "%s", cJSON_Print(root)) != 0) {
	//		cout << "write" << endl;
	//	}
	//	fclose(fp);
	//	fp = NULL;
	//}
	//else {
	//	cout << "fail wo open the file" << endl;
	//}
	//cJSON_Delete(root);
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
