
#include"htmlGet.h"
#include"webTraversal.h"
#include "getInfor.h"
#include<iostream>

using namespace std;

/*单独写一个获取具体课程url的函数采用广度遍历，好吧其实真实情况可能就一层*/
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
				//先试试两页
				//if (sendRequestPOST(sock, host, resource, cookie, page, schoolId) == true && page <= 2) {
				//暂时定每个学校最多只读取两页
				if (sendRequestPOST(sock, host, resource, school, page, schoolid.c_str()) == true && page <= 2) {
					string recvStr = recvRequest(sock, url);
					if (recvStr.length() != 0) {
						//好吧这才是真正获取url的函数
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
		//page = 1;//重置page

		/*school.popQ();
		schoolid.popQ();*/
	//}
}


/*从文件中获取课程URL*/
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
			(*courseUrl).pushQ(str);//指代课程的id进队
			//避免tags后不为null时出现混乱
			index = recvStr.find("}", indexEnd);

		}

	} while (index != -1);

}

/*获得课程信息*/
void getClassInfor(Myqueue* courseUrl, cJSON* root, string school) {
	//string url = "https://www.icourse163.org/course/HUST-";
	string url = "https://www.icourse163.org/course/"+school+"-";

	while ((*courseUrl).sizeQ() != 0) {
		cJSON* rootData;
		rootData = cJSON_CreateObject();//创建一个对象
		connnctCourse(url + (*courseUrl).frontQ(), rootData);
		(*courseUrl).popQ();
		cJSON_AddItemToArray(root, rootData);

	}
}

/*连接课程网页*/
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
				initClassInfor(&oneCourse);//初始化
				//获取html中信息
				getSchoolName(recvStr, &oneCourse);//学校名称
				getCourseName(recvStr, &oneCourse);//课程名称
				getCourseTime(recvStr, &oneCourse);//开课时间
				getStudentNum(recvStr, &oneCourse);//已参加人数
				getCourseDescribe(recvStr, &oneCourse);//课程概述
				getCourseAim(recvStr, &oneCourse);//授课目标
				getContent(recvStr, &oneCourse);//课程大纲
				getReference(recvStr, &oneCourse);//参考资料

				writeInfor(&oneCourse, root);//写入文件
			}


		}

		closesocket(sock);

	}
}
