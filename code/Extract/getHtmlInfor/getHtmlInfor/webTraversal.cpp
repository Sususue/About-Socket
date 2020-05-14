
#include"htmlGet.h"
#include"webTraversal.h"
#include <queue>//暂时先用STL的队列实现
#include<iostream>

using namespace std;

/*单独写一个获取具体课程url的函数采用广度遍历，好吧其实真实情况可能就一层*/
void GetCourseURL(string url) {
	int page = 1;
	while (true) {
		string host, resource;
		SOCKET sock;
		string cookie;
		int schoolId = 9008;//先传固定参数吧，暂时就跑一个学校的所有网站
		//至少有一页（应该是吧）

		if (parseURL(url, host, resource) == false) {
			return;
		}

		sock = connectSock(host.c_str(), 80, "tcp");

		if (sock == SOCKET_ERROR) {
			cout << "fail to connectSock" << endl;
		}

		else {
			if (sendRequestPOST(sock, host, resource, cookie, page, schoolId) == true && page <= 5) {
				string recvStr = recvRequest(sock, url);
				if (recvStr.length() != 0) {
					downLoad(recvStr);
					page++;
					cout << "page:" << page << endl;
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