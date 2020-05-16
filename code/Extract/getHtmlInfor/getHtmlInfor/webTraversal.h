#ifndef _WEBTRAVERSAL_H_
#define _WEBTRAVERSAL_H_

#include <string>
#include <queue>//��ʱ����STL�Ķ���ʵ��
#include "cJSON.h"

/*����дһ����ȡ����γ�url�ĺ������ù�ȱ������ð���ʵ��ʵ������ܾ�һ��*/
void GetCourseURL(std::string url);

/*���ļ��л�ȡ�γ�URL*/
void getURLFromFile(const std::string& recvStr, std::queue<std::string>& courseUrl);

/*��ÿγ���Ϣ*/
void getClassInfor(std::queue<std::string>& courseUrl, cJSON* root);

/*���ӿγ���ҳ*/
void connnctCourse(std::string url, cJSON* root);

#endif