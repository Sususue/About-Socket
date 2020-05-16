
#include"htmlGet.h"
#include"webTraversal.h"
#include "getInfor.h"


#include<iostream>

using namespace std;

/*单独写一个获取具体课程url的函数采用广度遍历，好吧其实真实情况可能就一层*/
void GetCourseURL(string url) {
	int page = 1;
	string host, resource;
	SOCKET sock;
	string cookie;
	int schoolId = 9008;//先传固定参数吧，暂时就跑一个学校的所有网站
	queue<string> courseUrl;
	char filename[30] = "./data/classOfHUST.json";

	FILE* fp = NULL;
	fp = fopen("CourseURL.txt", "wb");
	fclose(fp);
	fp = NULL;

	cJSON* root;
	root = cJSON_CreateArray();

	while (true) {


		//至少有一页（应该是吧）

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
			//暂时只读前四页，最后一页的tags都不为null,造成干扰
			if (sendRequestPOST(sock, host, resource, cookie, page, schoolId) == true && page <= 4) {
				string recvStr = recvRequest(sock, url);
				if (recvStr.length() != 0) {
					//好吧这才是真正获取url的函数
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
/*从文件中获取课程URL*/
void getURLFromFile(const string& recvStr, queue<string>& courseUrl) {
	int index = 0;
	int indexEnd = 0;
	//cout << recvStr[index] << endl;
	string str = "\0";
	int temp = 0;

	/*FILE* fp = NULL;
	fp = fopen("./课程url.txt", "wb");
	fclose(fp);
	fp = NULL;*/

	do {
		index = recvStr.find("\"id\":", index);
		if (index != -1) {
			index += strlen("\"id\":");

			indexEnd = recvStr.find(",", index);
			str = recvStr.substr(index, indexEnd - index);
			courseUrl.push(str);//指代课程的id进队


			//fp = fopen("./课程url.txt", "ab");//看看对不对
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

/*获得课程信息*/
void getClassInfor(queue<string>& courseUrl, cJSON* root) {
	string url = "https://www.icourse163.org/course/HUST-";
	//int qLen = courseUrl.size();
	//cJSON* root;
	//root = cJSON_CreateArray();
	while (courseUrl.size() != 0) {
		cJSON* rootData;
		rootData = cJSON_CreateObject();//创建一个对象
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
