#ifndef _WEBTRAVERSAL_H_
#define _WEBTRAVERSAL_H_

#include <string>
#include"MyQueue.h"//������ʵ�ֵĶ��У���ʱֻ��װstring
#include "cJSON.h"

/*����дһ����ȡ����γ�url�ĺ������ù�ȱ������ð���ʵ��ʵ������ܾ�һ��*/
void GetCourseURL(std::string url, std::string school, std::string schoolid);

/*���ļ��л�ȡ�γ�URL*/
void getURLFromFile(const std::string& recvStr, Myqueue* courseUrl);

/*��ÿγ���Ϣ*/
void getClassInfor(Myqueue* courseUrl, cJSON* root, std::string school);

/*���ӿγ���ҳ*/
void connnctCourse(std::string url, cJSON* root);

#endif