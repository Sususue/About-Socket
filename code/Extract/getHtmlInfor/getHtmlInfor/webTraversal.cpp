
#include"htmlGet.h"
#include"webTraversal.h"
#include <queue>//��ʱ����STL�Ķ���ʵ��
#include<iostream>

using namespace std;

/*����дһ����ȡ����γ�url�ĺ������ù�ȱ������ð���ʵ��ʵ������ܾ�һ��*/
void GetCourseURL(string url) {
	int page = 1;
	while (true) {
		string host, resource;
		SOCKET sock;
		string cookie;
		int schoolId = 9008;//�ȴ��̶������ɣ���ʱ����һ��ѧУ��������վ
		//������һҳ��Ӧ���ǰɣ�

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